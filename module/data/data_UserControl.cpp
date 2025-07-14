//
// Created by gouzuang on 25-7-6.
//
#include<pch.h>
#include "module/data/data_UserControl.h"

#include <qcryptographichash.h>
#include <qrandom.h>

#include "data_mail.h"

namespace data::UserControl {
    void dropDB() {
        // 兼容 MuSQL 场景，尝试删除数据库表而不是物理文件
        service::DatabaseManager db(service::Path::user());
        if (db.isConnected()) {
            db.executeNonQuery("DROP TABLE IF EXISTS users");
            db.executeNonQuery("DROP TABLE IF EXISTS groups");
            db.executeNonQuery("DROP TABLE IF EXISTS user_groups");
            log(LogLevel::INFO) << "数据库表删除成功 (users, groups, user_groups)";
        } else {
            log(LogLevel::ERR) << "数据库连接失败，无法删除表";
        }
    }

    void buildDB() {
        // 兼容 MuSQL 场景，不检测物理文件，直接尝试建表和创建默认用户
        Login::createUserTable();
        permission::createGroupTable();
        permission::createUserGroupTable();

        // 每次都尝试创建默认组和用户
        QStringList builtInGroupNames = {"Student", "Teacher", "Admin", "System"};
        for (const auto &groupName: builtInGroupNames) {
            auto ids = permission::searchGroupIdByName(groupName);
            if (!ids.isEmpty()) {
                builtInGroupIds.insert(groupName, ids.first());
            } else {
                // 如果组不存在则创建
                permission::createGroup(groupName, groupName + " group");
                auto newIds = permission::searchGroupIdByName(groupName);
                if (!newIds.isEmpty()) {
                    builtInGroupIds.insert(groupName, newIds.first());
                }
            }
        }
    }

    namespace Login {
        void createUserTable() {
            service::DatabaseManager db(service::Path::user());
            if (!db.tableExists("users")) {
                QString createTableQuery = R"(
        CREATE TABLE users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_number TEXT UNIQUE NOT NULL,
            username TEXT NOT NULL,
            password TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            status Text NOT NULL DEFAULT 'Unchecked'
        )
    )";
                db.executeNonQuery(createTableQuery);
            }
        }

        std::expected<int, UserControlError> isUserPasswordValid(const QString &idNumber, const QString &password_) {
            QString password = hashPassword(password_);
            service::DatabaseManager db(service::Path::user());
            log(service::LogLevel::INFO) << "开始验证用户密码: " << idNumber;
            QString query = R"(
                    SELECT id, password, status FROM users WHERE id_number = ? AND status != 'Deleted'
                )";
            auto results = db.executePreparedQueryAndFetchAll(query, {idNumber});

            if (results.isEmpty()) {
                log(service::LogLevel::ERR) << "登录失败。用户不存在或已删除: " << idNumber;
                return std::unexpected(UserControlError::UserNotFound);
            }

            const auto &row = results.first();
            int userId = row["id"].toInt();
            QString storedPassword = row["password"].toString();
            QString status = row["status"].toString();

            bool issys = permission::isUserInGroup(userId, "System");
            if (issys) {
                log(service::LogLevel::ERR) << "登录失败。用户属于系统: " << idNumber;
                return std::unexpected(UserControlError::UserNotFound);
            }

            if (status != "AllRight") {
                log(service::LogLevel::ERR) << "登录失败。用户状态异常: " << idNumber << " status: " << status;
                return std::unexpected(UserControlError::UserNotFound);
            }

            if (storedPassword != password) {
                log(service::LogLevel::ERR) << "登录失败。密码错误: " << idNumber;
                return std::unexpected(UserControlError::IncorrectPassword);
            }

            log(service::LogLevel::INFO) << "登录成功。密码验证成功: " << idNumber;
            currentUserId = userId;
            return userId;
        }

        std::expected<int, UserControlError> createNewUser(const QString &idNumber, const QString &username,
                                                           const QString &password_) {
            QString password = hashPassword(password_);
            service::DatabaseManager db(service::Path::user());
            if (!db.tableExists("users")) {
                log(service::LogLevel::ERR) << "用户表不存在";
                throw std::runtime_error("User table does not exist.");
            }

            auto userId = foundUserIdByIdNumber(idNumber);
            if (userId.has_value()) {
                log(service::LogLevel::INFO) << "用户已存在: " << idNumber;
                return std::unexpected(UserControlError::UserAlreadyExists);
            }

            QString insertQuery = R"(
                INSERT INTO users(id_number, username, password, status)
                VALUES(?, ?, ?, 'Unchecked')
            )";
            if (!db.executePreparedNonQuery(insertQuery, {idNumber, username, password})) {
                log(service::LogLevel::ERR) << "新用户创建失败: " << idNumber;
                throw std::runtime_error("Failed to create new user.");
            }

            log(service::LogLevel::DATA) << "新用户创建成功: " << idNumber;
            auto newUserId = foundUserIdByIdNumber(idNumber);
            if (!newUserId.has_value()) {
                throw std::runtime_error("Failed to retrieve new user ID after creation.");
            }

            data::mail::send_mail(data::mail::systemReservedAccounts["LabNexus团队"],
                                  newUserId.value(), "欢迎使用LabNexus", "您的账号已创建成功，请妥善保管您的账号信息。", "");
            return newUserId.value();
        }

        std::expected<int, UserControlError> createNewUser(const QString &idNumber, const QString &username,
                                                           const QString &password,
                                                           const QString &group) {
            if (group.isEmpty()) {
                log(service::LogLevel::DATA) << "未指定组，用户创建成功但未添加到任何组";
                return createNewUser(idNumber, username, password);
            }

            log(service::LogLevel::DATA) << "查询组ID: " << group;
            auto groupIds = permission::searchGroupIdByName(group);
            if (groupIds.isEmpty()) {
                log(service::LogLevel::ERR) << "组不存在: " << group;
                return std::unexpected(UserControlError::GroupNotFound);
            }
            if (groupIds.size() > 1) {
                log(service::LogLevel::ERR) << "找到多个匹配组: " << group;
                return std::unexpected(UserControlError::MutiResultFound);
            }

            return createNewUser(idNumber, username, password, groupIds.first());
        }

        std::expected<int, UserControlError> createNewUser(const QString &idNumber, const QString &username,
                                                           const QString &password, int groupId) {
            auto newUserResult = createNewUser(idNumber, username, password);
            if (!newUserResult) {
                return std::unexpected(newUserResult.error());
            }
            if (groupId <= 0) {
                log(service::LogLevel::DATA) << "未指定组ID，用户创建成功但未添加到任何组";
                return newUserResult.value();
            }
            log(service::LogLevel::DATA) << "尝试将用户添加到组ID: " << groupId;
            auto addToGroupResult = permission::addUserToGroup(newUserResult.value(), groupId);
            if (!addToGroupResult) {
                log(service::LogLevel::ERR) << "用户 " << newUserResult.value() << " 添加到组ID " << groupId << " 失败";
                return std::unexpected(addToGroupResult.error());
            }
            return newUserResult.value();
        }

        std::expected<int, UserControlError> foundUserIdByIdNumber(const QString &idNumber) {
            service::DatabaseManager db(service::Path::user());
            QString query = "SELECT id FROM users WHERE id_number = ? AND status != 'Deleted'";
            auto results = db.executePreparedQueryAndFetchAll(query, {idNumber});
            if (results.isEmpty()) {
                return std::unexpected(UserControlError::UserNotFound);
            }
            return results.first()["id"].toInt();
        }


        std::expected<bool, UserControlError> deleteUserById(int userId) {
            service::DatabaseManager db(service::Path::user());
            // 检查用户是否存在
            QString checkQuery = R"(
                    SELECT id FROM users WHERE id = ?
                )";
            auto results = db.executePreparedQueryAndFetchAll(checkQuery, {userId});
            if (results.isEmpty()) {
                log(service::LogLevel::INFO) << "删除失败。用户不存在: " << userId;
                return std::unexpected(UserControlError::UserNotFound);
            }
            // 标记用户在 user_groups 中的关联为 Deleted
            QString updateGroupsStatus = R"(
                    UPDATE user_groups SET status = 'Deleted' WHERE user_id = ?
                )";
            db.executePreparedNonQuery(updateGroupsStatus, {userId});
            // 标记用户为 Deleted
            QString updateUserStatus = R"(
                    UPDATE users SET status = 'Deleted' WHERE id = ?
                )";
            if (db.executePreparedNonQuery(updateUserStatus, {userId})) {
                log(service::LogLevel::DATA) << "用户标记为已删除: " << userId;
                return true;
            }
            log(service::LogLevel::ERR) << "标记用户删除失败: " << userId;
            return std::unexpected(UserControlError::DatabaseError);
        }

        std::expected<bool, UserControlError> updateUserPassword(int userId, const QString &password_) {
            QString newPassword = hashPassword(password_);
            service::DatabaseManager db(service::Path::user());
            QString updateQuery = R"(
                UPDATE users
                SET password = ?
                WHERE id = ?
            )";
            if (!db.executePreparedNonQuery(updateQuery, {newPassword, userId})) {
                return std::unexpected(UserControlError::DatabaseError);
            }
            log(service::LogLevel::DATA) << "用户" << userId << "密码更新成功: " << userId;
            return true;
        }

        QMap<QString, QString> batchCreateNewUser(const QList<QString> &idNumber, const int groupId,
                                                  const QString &usernameBase,
                                                  const QString &passwordInput) {
            QMap<QString, QString> result;
            service::DatabaseManager db(service::Path::user());

            // 2. 查询 users 表，找出哪些 idNumber（学工号）已存在及其对应的 id
            QString idNumberListPlaceholder;
            QList<QVariant> idNumberQueryParams;
            for (int i = 0; i < idNumber.size(); ++i) {
                idNumberListPlaceholder += "?";
                idNumberQueryParams.append(idNumber.at(i));
                if (i < idNumber.size() - 1) {
                    idNumberListPlaceholder += ",";
                }
            }

            QString userQuery = QString(R"(
                SELECT id_number, id, username FROM users WHERE id_number IN (%1)
            )").arg(idNumberListPlaceholder);

            auto userResults = db.executePreparedQueryAndFetchAll(userQuery, idNumberQueryParams);

            QSet<QString> existingIdNumbers; // 用于快速查找已存在的学工号
            QMap<QString, int> existingUserIds; // 存储已存在学工号和它们的主键 id
            QMap<QString, QString> existingUsernames; // 存储已存在学工号和它们的 username

            for (const auto &row: userResults) {
                QString currentIdNumber = row["id_number"].toString();
                int userId = row["id"].toInt();
                QString existingUsername = row["username"].toString();

                existingIdNumbers.insert(currentIdNumber);
                existingUserIds.insert(currentIdNumber, userId);
                existingUsernames.insert(currentIdNumber, existingUsername);
            }

            // 3. 遍历输入的 idNumber，处理新用户和现有用户
            for (const QString &currentIdNumber: idNumber) {
                if (!existingIdNumbers.contains(currentIdNumber)) {
                    // **新用户**
                    QString newPassword = passwordInput.isEmpty() ? generateRandomPassword() : passwordInput;
                    QString hashedPassword = hashPassword(newPassword);

                    // 拼接用户名
                    QString finalUsername = usernameBase;
                    if (currentIdNumber.length() >= 4) {
                        finalUsername += currentIdNumber.right(4);
                    } else {
                        finalUsername += currentIdNumber; // 位数不够就直接拼接
                    }

                    // 插入到 users 表
                    QString insertUserQuery = R"(
                        INSERT INTO users (id_number, username, password, created_at, status)
                        VALUES (?, ?, ?, CURRENT_TIMESTAMP, 'AllRight')
                    )";
                    QList<QVariant> insertUserParams = {
                        currentIdNumber, finalUsername, hashedPassword
                    };

                    // 执行插入并尝试获取新生成的用户ID
                    auto insertResult = db.executePreparedInsertAndGetId(insertUserQuery, insertUserParams);
                    if (insertResult != -1) {
                        QString insertUserGroupQuery = R"(
                            INSERT INTO user_groups (user_id, group_id, status)
                            VALUES (?, ?, 'AllRight')
                        )";
                        db.executePreparedQuery(insertUserGroupQuery, {insertResult, groupId});

                        result.insert(currentIdNumber, newPassword); // 返回新用户的生成密码
                    } else {
                        result.insert(currentIdNumber, "注册失败"); // 标记注册失败
                    }
                } else {
                    // **已存在的用户**
                    int userId = existingUserIds.value(currentIdNumber);

                    // 检查用户是否已在目标组中
                    QString checkGroupQuery = R"(
                        SELECT COUNT(*) FROM user_groups ug
                        WHERE ug.user_id = ? AND ug.group_id = ?
                    )";
                    QList<QVariant> checkGroupParams = {userId, groupId};
                    auto groupCheckResults = db.executePreparedQueryAndFetchAll(checkGroupQuery, checkGroupParams);

                    int count = 0;
                    if (!groupCheckResults.isEmpty()) {
                        count = groupCheckResults.first().value("COUNT(*)", 0).toInt();
                    }

                    if (count == 0) {
                        // 用户已存在但不在目标组中，需要增加用户组关系
                        QString insertUserGroupQuery = R"(
                            INSERT INTO user_groups (user_id, group_id, status)
                            VALUES (?, ?, 'AllRight')
                        )";
                        db.executePreparedQuery(insertUserGroupQuery, {userId, groupId});
                        result.insert(currentIdNumber, ""); // 已有用户，密码留空
                    } else {
                        // 用户已存在且已在目标组中，无需操作
                        result.insert(currentIdNumber, ""); // 已有用户，密码留空
                    }
                }
            }

            return result;
        }


        QMap<QString, QString> batchGetUserCanCreate(const QList<QString> &idNumber, const int groupId) {
            QMap<QString, QString> result;
            service::DatabaseManager db(service::Path::user());

            // 1. 查询 users 表，找出哪些 idNumber（学工号）已存在及其对应的 id
            QString idNumberListPlaceholder;
            QList<QVariant> idNumberQueryParams;
            for (int i = 0; i < idNumber.size(); ++i) {
                idNumberListPlaceholder += "?";
                idNumberQueryParams.append(idNumber.at(i));
                if (i < idNumber.size() - 1) {
                    idNumberListPlaceholder += ",";
                }
            }

            QString userQuery = QString(R"(
                SELECT id_number, id FROM users WHERE id_number IN (%1)
            )").arg(idNumberListPlaceholder);

            auto userResults = db.executePreparedQueryAndFetchAll(userQuery, idNumberQueryParams);

            QSet<QString> existingIdNumbers; // 用于快速查找已存在的学工号
            QMap<QString, int> existingUserIds; // 存储已存在学工号和它们的主键 id

            for (const auto &row: userResults) {
                QString currentIdNumber = row["id_number"].toString();
                int userId = row["id"].toInt();
                existingIdNumbers.insert(currentIdNumber);
                existingUserIds.insert(currentIdNumber, userId);
            }

            // 2. 遍历输入的 idNumber，判断用户状态并转换为字符串描述
            for (const QString &currentIdNumber: idNumber) {
                if (!existingIdNumbers.contains(currentIdNumber)) {
                    // 用户不存在，可以创建新用户
                    result.insert(currentIdNumber, "可以创建");
                } else {
                    // 用户已存在，进一步检查是否在指定组中
                    int userId = existingUserIds.value(currentIdNumber);

                    // 查询 user_groups 和 groups 表，检查用户是否在目标组中
                    QString checkGroupQuery = R"(
                        SELECT COUNT(*) FROM user_groups ug
                        WHERE ug.user_id = ? AND ug.group_id = ?
                    )";
                    QList<QVariant> checkGroupParams = {userId, groupId};
                    auto groupCheckResults = db.executePreparedQueryAndFetchAll(checkGroupQuery, checkGroupParams);

                    int count = 0;
                    if (!groupCheckResults.isEmpty()) {
                        count = groupCheckResults.first().value("COUNT(*)", 0).toInt();
                    }

                    if (count > 0) {
                        // 用户已存在且已在目标组中，无需操作
                        result.insert(currentIdNumber, "已存在且在组中");
                    } else {
                        // 用户已存在但不在目标组中，视为需要增加用户组关系
                        result.insert(currentIdNumber, "已存在但不在组中，需要增加组关系");
                    }
                }
            }
            return result;
        }

        QString generateRandomPassword(int length) {
            const QString possibleCharacters =
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
            QString randomString;
            for (int i = 0; i < length; ++i) {
                randomString.append(
                    possibleCharacters.at(QRandomGenerator::global()->bounded(possibleCharacters.length())));
            }
            return randomString;
        }

        QString hashPassword(const QString &password) {
            return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
        }
    }

    namespace permission {
        void createGroupTable() {
            service::DatabaseManager db(service::Path::user());
            if (!db.tableExists("groups")) {
                QString createTableQuery = R"(
            CREATE TABLE groups(
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT UNIQUE NOT NULL,
                description TEXT,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                status TEXT NOT NULL DEFAULT 'AllRight'
            )
        )";
                db.executeNonQuery(createTableQuery);
            }
        }

        void createUserGroupTable() {
            service::DatabaseManager db(service::Path::user());
            if (!db.tableExists("user_groups")) {
                QString createUserGroupTableQuery = R"(
            CREATE TABLE user_groups(
                user_id INTEGER NOT NULL,
                group_id INTEGER NOT NULL,
                status TEXT NOT NULL DEFAULT 'AllRight',
                FOREIGN KEY(user_id) REFERENCES users(id),
                FOREIGN KEY(group_id) REFERENCES groups(id),
                PRIMARY KEY(user_id, group_id)
            )
        )";
                db.executeNonQuery(createUserGroupTableQuery);
            }
        }


        std::expected<bool, UserControlError> createGroup(const QString &name, const QString &description) {
            service::DatabaseManager db(service::Path::user());
            if (!db.tableExists("groups")) {
                throw std::runtime_error("Group table does not exist.");
            }
            // 检查组是否已存在
            QString checkGroupQuery = R"(
                SELECT id FROM groups WHERE name = ?
            )";
            auto results = db.executePreparedQueryAndFetchAll(checkGroupQuery, {name});
            if (!results.isEmpty()) {
                log(service::LogLevel::INFO) << "组已存在: " << name;
                return std::unexpected(UserControlError::GroupAlreadyExists);
            }

            QString insertQuery = R"(
                INSERT INTO groups(name, description, status)
                VALUES(?, ?, 'AllRight')
            )";
            if (db.executePreparedNonQuery(insertQuery, {name, description})) {
                log(service::LogLevel::DATA) << "新组创建成功: " << name;
                return true;
            }

            log(service::LogLevel::ERR) << "新组创建失败: " << name;
            return std::unexpected(UserControlError::DatabaseError);
        }

        std::expected<bool, UserControlError> addUserToGroup(int userId, const QString &groupName) {
            auto groupIds = searchGroupIdByName(groupName);
            if (groupIds.isEmpty()) {
                return std::unexpected(UserControlError::GroupNotFound);
            }
            if (groupIds.size() > 1) {
                return std::unexpected(UserControlError::MutiResultFound);
            }
            return addUserToGroup(userId, groupIds.first());
        }

        std::expected<bool, UserControlError> addUserToGroup(int userId, int groupId) {
            service::DatabaseManager db(service::Path::user());
            // 检查用户是否已经在该组中
            QString checkQuery = R"(
                SELECT user_id FROM user_groups WHERE user_id = ? AND group_id = ?
            )";
            auto results = db.executePreparedQueryAndFetchAll(checkQuery, {userId, groupId});
            if (!results.isEmpty()) {
                log(service::LogLevel::INFO) << "用户 " << userId << " 已经在组 " << groupId << " 中";
                return std::unexpected(UserControlError::UserAlreadyInGroup);
            }
            // 将用户添加到组
            QString insertQuery = R"(
                INSERT INTO user_groups(user_id, group_id, status)
                VALUES(?, ?, 'AllRight')
            )";
            if (db.executePreparedNonQuery(insertQuery, {userId, groupId})) {
                log(service::LogLevel::DATA) << "用户 " << userId << " 添加到组 " << groupId << " 成功";
                return true;
            }
            log(service::LogLevel::ERR) << "用户 " << userId << " 添加到组 " << groupId << " 失败";
            return std::unexpected(UserControlError::DatabaseError);
        }

        QString getUserInWhichGroup(int userId) {
            auto group = getUserInWhichGroupList(userId);
            if (group.isEmpty()) {
                return QString();
            } else {
                return group.join(", ");
            }
        }

        QStringList getUserInWhichGroupList(int userId) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                SELECT g.name
                FROM groups g
                JOIN user_groups ug ON g.id = ug.group_id
                WHERE ug.user_id = ? AND g.status != 'Deleted' AND ug.status != 'Deleted'
            )";
            auto results = db.executePreparedQueryAndFetchAll(query, {userId});
            QStringList groupNames;
            for (const auto &row: results) {
                groupNames.append(row["name"].toString());
            }
            return groupNames;
        }

        QList<int> getUserInWhichGroupIdList(int userId) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                SELECT g.id
                FROM groups g
                JOIN user_groups ug ON g.id = ug.group_id
                WHERE ug.user_id = ? AND g.status != 'Deleted' AND ug.status != 'Deleted'
            )";
            auto results = db.executePreparedQueryAndFetchAll(query, {userId});
            QList<int> groupIds;
            for (const auto &row: results) {
                groupIds.append(row["id"].toInt());
            }
            return groupIds;
        }

        QList<int> searchGroupIdByName(const QString &groupName) {
            QList<int> groupIds;
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                SELECT id FROM groups WHERE name LIKE ? AND status != 'Deleted'
            )";
            auto results = db.executePreparedQueryAndFetchAll(query, {"%" + groupName + "%"});
            for (const auto &row: results) {
                groupIds.append(row["id"].toInt());
            }
            return groupIds;
        }

        void deleteUserFromGroup(int userId, const QString &groupName) {
            auto groupIds = searchGroupIdByName(groupName);
            if (groupIds.isEmpty()) {
                log(service::LogLevel::ERR) << "组不存在: " << groupName;
                return;
            }
            if (groupIds.size() > 1) {
                log(service::LogLevel::ERR) << "找到多个匹配组: " << groupName;
                return;
            }
            int groupId = groupIds.first();
            service::DatabaseManager db(service::Path::user());
            QString deleteQuery = R"(
                        DELETE FROM user_groups WHERE user_id = ? AND group_id = ?
                    )";
            if (db.executePreparedNonQuery(deleteQuery, {userId, groupId})) {
                log(service::LogLevel::DATA) << "用户" << userId << "已从组" << groupName << "中删除";
            } else {
                log(service::LogLevel::ERR) << "删除用户" << userId << "从组" << groupName << "失败";
            }
        }


        bool isUserInGroup(int userId, const QString &groupName) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                SELECT COUNT(*)
                FROM user_groups ug
                JOIN groups g ON ug.group_id = g.id
                WHERE ug.user_id = ? AND g.name = ?
            )";
            auto results = db.executePreparedQueryAndFetchAll(query, {userId, groupName});
            if (results.isEmpty()) {
                return false;
            }
            return results.first()["COUNT(*)"].toInt() > 0;
        }

        QList<int> getAllGroupId() {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                  SELECT id FROM groups WHERE status != 'Deleted'
              )";
            auto results = db.executePreparedQueryAndFetchAll(query, {});
            QList<int> groupIds;
            for (const auto &row: results) {
                groupIds.append(row["id"].toInt());
            }
            return groupIds;
        }

        QMap<QString, int> getAllGroup() {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                SELECT id, name FROM groups WHERE status != 'Deleted'
            )";
            auto results = db.executePreparedQueryAndFetchAll(query, {});

            QMap<QString, int> groupMap;
            for (const auto &row: results) {
                int id = row["id"].toInt();
                QString name = row["name"].toString();
                groupMap.insert(name, id); // 组名作为 key，id 作为 value
            }

            return groupMap;
        }
    }

    namespace UserInfo {
        std::expected<QString, UserInfoError> getUserNameById(int userId) {
            service::DatabaseManager db(service::Path::user());
            QString query = "SELECT username FROM users WHERE id = ? AND status != 'Deleted'";
            auto results = db.executePreparedQueryAndFetchAll(query, {userId});

            if (results.isEmpty()) {
                return std::unexpected(UserInfoError::UserNotFound);
            }

            return results.first()["username"].toString();
        }

        std::expected<QString, UserInfoError> getUserNameByIdNumber(QString IdNumber) {
            service::DatabaseManager db(service::Path::user());
            QString query = "SELECT username FROM users WHERE id_number = ? AND status != 'Deleted'";
            auto results = db.executePreparedQueryAndFetchAll(query, {IdNumber});

            if (results.isEmpty()) {
                return std::unexpected(UserInfoError::UserNotFound);
            }

            return results.first()["username"].toString();
        }

        std::expected<int, UserInfoError> getIdByIdNumber(QString IdNumber) {
            service::DatabaseManager db(service::Path::user());
            QString query = "SELECT id FROM users WHERE id_number = ? AND status != 'Deleted'";
            auto results = db.executePreparedQueryAndFetchAll(query, {IdNumber});

            if (results.isEmpty()) {
                return std::unexpected(UserInfoError::UserNotFound);
            }

            return results.first()["id"].toInt();
        }

        std::expected<int, UserInfoError> getIdNumberById(int Id) {
            service::DatabaseManager db(service::Path::user());
            QString query = "SELECT id_number FROM users WHERE id = ? AND status != 'Deleted'";
            auto results = db.executePreparedQueryAndFetchAll(query, {Id});

            if (results.isEmpty()) {
                return std::unexpected(UserInfoError::UserNotFound);
            }

            return results.first()["id_number"].toInt();
        }

        void changeUserName(int userId, const QString &newName) {
            service::DatabaseManager db(service::Path::user());
            QString updateQuery = R"(
                UPDATE users
                SET username = ?
                WHERE id = ?
            )";
            if (!db.executePreparedNonQuery(updateQuery, {newName, userId})) {
                log(service::LogLevel::ERR) << "更新用户名失败: " << userId;
                throw std::runtime_error("Failed to update username.");
            }
        }

        QMap<int, QString> loadUsersMap() {
            QMap<int, QString> usersMap;
            service::DatabaseManager db(service::Path::user());
            QString query = R"(SELECT id_number,username FROM users WHERE status != 'Deleted')";
            auto results = db.executeQueryAndFetchAll(query);
            for (const auto &row: results) {
                usersMap[row["id_number"].toInt()] = row["username"].toString();
            }
            return usersMap;
        }

        QMap<int, QString> loadGroupsMap() {
            QMap<int, QString> groupsMap;
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                SELECT g.name , u.id_number
                FROM groups g
                JOIN user_groups ug ON g.id = ug.group_id
                JOIN users u ON ug.user_id = u.id
                WHERE g.status != 'Deleted' AND ug.status != 'Deleted' AND u.status != 'Deleted'
            )";
            auto results = db.executeQueryAndFetchAll(query);
            for (const auto &row: results) {
                groupsMap[row["id_number"].toInt()] = row["name"].toString();
            }
            return groupsMap;
        }
    }

    namespace check {
        QList<int> getAllUserId(int page, int pageSize) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                  SELECT id FROM users WHERE status != 'Deleted' AND id_number NOT LIKE 'System%'
                    ORDER BY created_at DESC
                    LIMIT ? OFFSET ?
              )";
            QVariantList params;
            params << pageSize << (page - 1) * pageSize;

            auto results = db.executePreparedQueryAndFetchAll(query, params);
            QList<int> userIds;
            for (const auto &row: results) {
                userIds.append(row["id"].toInt());
            }
            return userIds;
        }

        QList<int> getUncheckedUserId(int page, int pageSize) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                  SELECT id FROM users WHERE status == 'Unchecked' AND id_number NOT LIKE 'System%'
                    ORDER BY created_at DESC
                    LIMIT ? OFFSET ?
              )";
            int offset = (page - 1) * pageSize;
            auto results = db.executePreparedQueryAndFetchAll(query, {pageSize, offset});
            QList<int> userIds;
            for (const auto &row: results) {
                userIds.append(row["id"].toInt());
            }
            return userIds;
        }

        QList<int> getAllUserId() {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                  SELECT id FROM users WHERE status != 'Deleted' AND id_number NOT LIKE 'System%'
              )";
            auto results = db.executePreparedQueryAndFetchAll(query, {});
            QList<int> userIds;
            for (const auto &row: results) {
                userIds.append(row["id"].toInt());
            }
            return userIds;
        }

        QList<int> getUncheckedUserId() {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                   SELECT id FROM users WHERE status == 'Unchecked' AND id_number NOT LIKE 'System%'
              )";
            auto results = db.executePreparedQueryAndFetchAll(query, {});
            QList<int> userIds;
            for (const auto &row: results) {
                userIds.append(row["id"].toInt());
            }
            return userIds;
        }

        int getUncheckedUserCount() {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                   SELECT COUNT(*) as count FROM users WHERE status == 'Unchecked' AND id_number NOT LIKE '-%'
              )";
            auto results = db.executePreparedQueryAndFetchAll(query, {});

            if (!results.isEmpty()) {
                return results.first()["count"].toInt();
            }

            return 0;
        }

        int getAllUserCount() {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                   SELECT COUNT(*) as count FROM users WHERE status != 'Deleted' AND id_number NOT LIKE '-%'
              )";
            auto results = db.executePreparedQueryAndFetchAll(query, {});

            if (!results.isEmpty()) {
                return results.first()["count"].toInt();
            }

            return 0;
        }

        bool allowUserRegister(int userId) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                UPDATE users SET status = 'AllRight' WHERE id = ? AND status = 'Unchecked'
            )";
            if (!db.executePreparedNonQuery(query, {userId})) {
                return false;
            }
            return true;
        }

        bool banUser(int userId) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                UPDATE users SET status = 'Banned' WHERE id = ?
            )";
            if (!db.executePreparedNonQuery(query, {userId})) {
                return false;
            }
            return true;
        }

        bool unbanUser(int userId) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                UPDATE users SET status = 'AllRight' WHERE id = ? AND status = 'Banned'
            )";
            if (!db.executePreparedNonQuery(query, {userId})) {
                return false;
            }
            return true;
        }

        bool rejectUserRegister(int userId) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                UPDATE users SET status = 'Rejected' WHERE id = ? AND status = 'Unchecked'
            )";
            if (!db.executePreparedNonQuery(query, {userId})) {
                return false;
            }
            return true;
        }

        QString getUserStatus(int userId) {
            service::DatabaseManager db(service::Path::user());
            QString query = R"(
                   SELECT status FROM users WHERE id = ?
              )";
            auto results = db.executePreparedQueryAndFetchAll(query, {userId});
            if (results.isEmpty()) {
                return QString();
            }
            return results.first()["status"].toString();
        }
    }
}

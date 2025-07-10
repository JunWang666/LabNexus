//
// Created by gouzuang on 25-7-6.
//
#include<pch.h>
#include "module/data/data_UserControl.h"

namespace data::UserControl {
void dropDB() {
    QFile dbFile(path);
    if (dbFile.exists()) {
        if (dbFile.remove()) {
            log(LogLevel::INFO) << "数据库文件删除成功";
        } else {
            log(LogLevel::ERR) << "数据库文件删除失败";
        }
    } else {
        log(LogLevel::INFO) << "数据库文件不存在";
    }
}

void buildDB() {
    QFile dbFile(path);
    if (!dbFile.exists()) {
        if (dbFile.open(QIODevice::WriteOnly)) {
            dbFile.close();
            log(service::LogLevel::INFO) << "数据库文件创建成功";
        } else {
            log(service::LogLevel::ERR) << "数据库文件创建失败";
        }

        Login::createUserTable();
        permission::createGroupTable();
        permission::createUserGroupTable();
        // 创建用户组
        if (auto r = permission::createGroup("Student", ""); !r) {
            log(LogLevel::ERR) << "创建组 Student 失败, 错误码:" << static_cast<int>(r.error());
        }
        if (auto r = permission::createGroup("Teacher", ""); !r) {
            log(LogLevel::ERR) << "创建组 Teacher 失败, 错误码:" << static_cast<int>(r.error());
        }
    } else {
        log(service::LogLevel::INFO) << "数据库文件已存在";
    }
}

namespace Login {
void createUserTable() {
    service::DatabaseManager db(path);
    if (!db.tableExists("users")) {
        QString createTableQuery = R"(
        CREATE TABLE users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_number TEXT UNIQUE NOT NULL,
            username TEXT NOT NULL,
            password TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            status Text NOT NULL DEFAULT 'AllRight'
        )
    )";
        db.executeNonQuery(createTableQuery);
    }
}

std::expected<int, UserControlError> isUserPasswordValid(const QString &idNumber, const QString &password) {
    service::DatabaseManager db(path);
    log(service::LogLevel::INFO) << "开始验证用户密码: " << idNumber;
    QString query = R"(
                    SELECT id, password, status FROM users WHERE id_number = ?
                )";
    auto results = db.executePreparedQueryAndFetchAll(query, {idNumber});

    if (results.isEmpty()) {
        log(service::LogLevel::ERR) << "登录失败。用户不存在: " << idNumber;
        return std::unexpected(UserControlError::UserNotFound);
    }

    const auto& row = results.first();
    int userId = row["id"].toInt();
    QString storedPassword = row["password"].toString();
    QString status = row["status"].toString();

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
                                                   const QString &password) {
    service::DatabaseManager db(path);
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
                VALUES(?, ?, ?, 'AllRight')
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
    return newUserId.value();
}

std::expected<int, UserControlError> createNewUser(const QString &idNumber, const QString &username,
                                                   const QString &password,
                                                   const QString group) {
    auto newUserResult = createNewUser(idNumber, username, password);
    if (!newUserResult) {
        return std::unexpected(newUserResult.error());
    }

    if (group.isEmpty()) {
        log(service::LogLevel::DATA) << "未指定组，用户创建成功但未添加到任何组";
        return newUserResult.value();
    }

    log(service::LogLevel::DATA) << "尝试将用户添加到组: " << group;
    auto addToGroupResult = permission::addUserToGroup(newUserResult.value(), group);
    if (!addToGroupResult) {
        log(service::LogLevel::ERR) << "用户 " << newUserResult.value() << " 添加到组 " << group << " 失败";
        return std::unexpected(addToGroupResult.error());
    }

    return newUserResult.value();
}

std::expected<int, UserControlError> foundUserIdByIdNumber(const QString &idNumber) {
    service::DatabaseManager db(path);
    QString query = "SELECT id FROM users WHERE id_number = ?";
    auto results = db.executePreparedQueryAndFetchAll(query, {idNumber});
    if (results.isEmpty()) {
        return std::unexpected(UserControlError::UserNotFound);
    }
    return results.first()["id"].toInt();
}


std::expected<bool, UserControlError> deleteUserById(int userId) {
    service::DatabaseManager db(path);
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

std::expected<bool, UserControlError> updateUserPassword(int userId, const QString &newPassword) {
    service::DatabaseManager db(path);
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
}

namespace permission {
void createGroupTable() {
    service::DatabaseManager db(path);
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
    service::DatabaseManager db(path);
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
    service::DatabaseManager db(path);
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
    service::DatabaseManager db(path);

    // 检查组是否存在
    QString checkGroupQuery = R"(
                SELECT id FROM groups WHERE name = ?
            )";
    auto groupResults = db.executePreparedQueryAndFetchAll(checkGroupQuery, {groupName});
    if (groupResults.isEmpty()) {
        log(service::LogLevel::INFO) << "组不存在: " << groupName;
        return std::unexpected(UserControlError::GroupNotFound);
    }
    int groupId = groupResults.first()["id"].toInt();

    // 检查用户是否已经在该组中
    QString checkUserInGroupQuery = R"(
                SELECT user_id FROM user_groups WHERE user_id = ? AND group_id = ?
            )";
    auto userInGroupResults = db.executePreparedQueryAndFetchAll(checkUserInGroupQuery, {userId, groupId});
    if (!userInGroupResults.isEmpty()) {
        log(service::LogLevel::INFO) << "用户 " << userId << " 已经在组 " << groupName << " 中";
        return std::unexpected(UserControlError::UserAlreadyInGroup);
    }

    // 将用户添加到组
    QString insertUserGroupQuery = R"(
                INSERT INTO user_groups(user_id, group_id, status)
                VALUES(?, ?, 'AllRight')
            )";
    if (db.executePreparedNonQuery(insertUserGroupQuery, {userId, groupId})) {
        log(service::LogLevel::DATA) << "用户 " << userId << " 添加到组 " << groupName << " 成功";
        return true;
    }

    log(service::LogLevel::ERR) << "用户 " << userId << " 添加到组 " << groupName << " 失败";
    return std::unexpected(UserControlError::DatabaseError);
}

QString getUserInWhichGroup(int userId) {
    service::DatabaseManager db(path);
    QString query = R"(
        SELECT g.name
        FROM groups g
        JOIN user_groups ug ON g.id = ug.group_id
        WHERE ug.user_id = ?
    )";
    auto results = db.executePreparedQueryAndFetchAll(query, {userId});
    QStringList groupNames;
    for (const auto &row : results) {
        groupNames.append(row["name"].toString());
    }
    return groupNames.join(", ");
}


bool isUserInGroup(int userId, const QString &groupName) {
    service::DatabaseManager db(path);
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
}
namespace UserInfo {
std::expected<QString,UserInfoError> getUserNameById(int userId) {
    service::DatabaseManager db(path);
    QString query = "SELECT username FROM users WHERE id = ?";
    auto results = db.executePreparedQueryAndFetchAll(query, {userId});

    if (results.isEmpty()) {
        return std::unexpected(UserInfoError::UserNotFound);
    }

    return results.first()["username"].toString();
}

}
}



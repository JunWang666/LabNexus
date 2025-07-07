//
// Created by gouzuang on 25-7-6.
//
#include<pch.h>
#include "data_UserControl.h"

namespace data::UserControl {
    void dropDB() {
        QFile dbFile("./user.db");
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
        Login::createUserTable();
        permission::createGroupTable();
        permission::createUserGroupTable();
    }

    namespace Login {
        void createUserTable() {
            service::DatabaseManager db("./user.db");
            if (!db.tableExists("users")) {
                QString createTableQuery = R"(
        CREATE TABLE users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            email TEXT UNIQUE NOT NULL,
            username TEXT NOT NULL,
            password TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            statue Text NOT NULL DEFAULT 'AllRight'
        )
    )";
                db.executeNonQuery(createTableQuery);
            }
        }

        int isUserPasswordValid(const QString &email, const QString &password) {
            service::DatabaseManager db("./user.db");
            log(service::LogLevel::INFO) << "开始验证用户密码: " << email;
            QString query = R"(
                    SELECT id, password, statue FROM users WHERE email = ?
                )";
            QSqlQuery q = db.executePreparedQuery(query, {email});

            if (!q.next()) {
                log(service::LogLevel::INFO) << "登录失败。用户不存在: " << email;
                // 确保查询被清理
                q.finish();
                q.clear();
                return -2; // 用户不存在
            }

            // 提取所需数据
            int userId = q.value(0).toInt();
            QString storedPassword = q.value(1).toString();
            QString status = q.value(2).toString();

            // 立即清理查询对象
            q.finish();
            q.clear();

            if (status == "AllRight") {
                if (storedPassword != password) {
                    log(service::LogLevel::INFO) << "登录失败。密码错误: " << email;
                    return -3; // 密码错误
                }
                log(service::LogLevel::INFO) << "登录成功。密码验证成功: " << email;
                return userId;
            } else {
                if (status == "Locked") {
                    log(service::LogLevel::INFO) << "登录失败。用户被锁定: " << email;
                    return -4; // 用户被锁定
                }
                if (status == "Deleted") {
                    log(service::LogLevel::INFO) << "登录失败。用户已删除: " << email;
                    return -5; // 用户已删除
                }
            }
            log(service::LogLevel::ERR) << "登录失败。未知错误: " << email;
            return -1; // 未知错误
        }

        bool createNewUser(const QString &email, const QString &username, const QString &password) {
            service::DatabaseManager db("./user.db");
            if (db.tableExists("users")) {
                // 检查用户是否已存在
                QString checkUserQuery = R"(
                    SELECT id FROM users WHERE email = ?
                )";
                QSqlQuery q = db.executePreparedQuery(checkUserQuery, {email});
                bool userExists = q.next();
                q.clear(); // 立即清理查询对象

                if (userExists) {
                    log(service::LogLevel::INFO) << "用户已存在: " << email;
                    return false; // 用户已存在
                }

                QString insertQuery = R"(
                    INSERT INTO users(email, username, password, statue)
                    VALUES(?, ?, ?, 'AllRight')
                )";
                bool isOk = db.executePreparedNonQuery(insertQuery, {email, username, password});
                if (isOk) {
                    log(service::LogLevel::DATA) << "新用户创建成功: " << email;
                } else {
                    log(service::LogLevel::ERR) << "新用户创建失败: " << email;
                }
                return isOk;
            }
            log(service::LogLevel::ERR) << "用户表不存在";
            return false;
        }

        bool createNewUser(const QString &email, const QString &username, const QString &password,
                           const QString group) {
            bool userCreated = createNewUser(email, username, password);
            if (!userCreated) {
                log(service::LogLevel::ERR) << "用户创建失败: " << email;
                return false; // 用户创建失败，提前返回
            }
            if (!group.isEmpty()) {
                log(service::LogLevel::DATA) << "尝试将用户添加到组: " << group;
                return permission::addUserToGroup(foundUserIdByEmail(email), group);
            } else {
                log(service::LogLevel::DATA) << "未指定组，用户创建成功但未添加到任何组";
                return true;
            }
        }

        int foundUserIdByEmail(const QString &email) {
            service::DatabaseManager db("./user.db");
            QString query = R"(
                SELECT id FROM users WHERE email = ?
            )";
            QSqlQuery q = db.executePreparedQuery(query, {email});

            if (q.next()) {
                int userId = q.value(0).toInt();
                log(service::LogLevel::DATA) << "找到用户ID: " << userId << " 对应邮箱: " << email;
                return userId;
            } else {
                log(service::LogLevel::INFO) << "未找到对应邮箱的用户: " << email;
                return -1; // 未找到用户
            }
        }
    }

    namespace permission {
        void createGroupTable() {
            service::DatabaseManager db("./user.db");
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
            service::DatabaseManager db("./user.db");
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


        bool createGroup(const QString &name, const QString &description) {
            service::DatabaseManager db("./user.db");
            if (db.tableExists("groups")) {
                // 检查组是否已存在
                QString checkGroupQuery = R"(
                    SELECT id FROM groups WHERE name = ?
                )";
                QSqlQuery q = db.executePreparedQuery(checkGroupQuery, {name});
                bool groupExists = q.next();
                q.clear(); // 立即清理查询对象

                if (groupExists) {
                    log(service::LogLevel::INFO) << "组已存在: " << name;
                    return false; // 组已存在
                }

                QString insertQuery = R"(
                    INSERT INTO groups(name, description, status)
                    VALUES(?, ?, 'AllRight')
                )";
                bool isOk = db.executePreparedNonQuery(insertQuery, {name, description});
                if (isOk) {
                    log(service::LogLevel::DATA) << "新组创建成功: " << name;
                } else {
                    log(service::LogLevel::ERR) << "新组创建失败: " << name;
                }
                return isOk;
            }
            log(service::LogLevel::ERR) << "组表不存在";
            return false;
        }

        bool addUserToGroup(int userId, const QString &groupName) {
            service::DatabaseManager db("./user.db");

            // 检查组是否存在
            QString checkGroupQuery = R"(
                SELECT id FROM groups WHERE name = ?
            )";
            QSqlQuery q = db.executePreparedQuery(checkGroupQuery, {groupName});
            if (!q.next()) {
                log(service::LogLevel::INFO) << "组不存在: " << groupName;
                return false; // 组不存在
            }
            int groupId = q.value(0).toInt();
            q.clear(); // 立即清理查询对象

            // 检查用户是否已经在该组中
            QString checkUserInGroupQuery = R"(
                SELECT user_id FROM user_groups WHERE user_id = ? AND group_id = ?
            )";
            q = db.executePreparedQuery(checkUserInGroupQuery, {userId, groupId});
            if (q.next()) {
                log(service::LogLevel::INFO) << "用户 " << userId << " 已经在组 " << groupName << " 中";
                return true; // 用户已经在组中
            }
            q.clear(); // 立即清理查询对象

            // 将用户添加到组
            QString insertUserGroupQuery = R"(
                INSERT INTO user_groups(user_id, group_id, status)
                VALUES(?, ?, 'AllRight')
            )";
            bool isOk = db.executePreparedNonQuery(insertUserGroupQuery, {userId, groupId});
            if (isOk) {
                log(service::LogLevel::DATA) << "用户 " << userId << " 添加到组 " << groupName << " 成功";
            } else {
                log(service::LogLevel::ERR) << "用户 " << userId << " 添加到组 " << groupName << " 失败";
            }
            return isOk;
        }

        QString getUserInWhichGroup(int userId) {
            service::DatabaseManager db("./user.db");
            QString query = R"(
                SELECT g.name FROM groups g
                JOIN user_groups ug ON g.id = ug.group_id
                WHERE ug.user_id = ?
            )";
            QSqlQuery q = db.executePreparedQuery(query, {userId});

            if (q.next()) {
                QStringList groupNames;
                do {
                    groupNames.append(q.value(0).toString());
                } while (q.next());

                QString groupList = groupNames.join(", ");
                log(service::LogLevel::DATA) << "用户 " << userId << " 在组: " << groupList;
                return groupList;
            } else {
                log(service::LogLevel::INFO) << "用户 " << userId << " 不在任何组中";
                return QString();
            }
        }

        bool isUserInGroup(int userId, const QString &groupName) {
            // 获取该用户所在的所有组，再检查是否包含目标组
            QString groupList = getUserInWhichGroup(userId);
            QStringList groups = groupList.split(", ", Qt::SkipEmptyParts);
            bool inGroup = groups.contains(groupName);
            if (inGroup) {
                log(service::LogLevel::DATA) << "用户 " << userId << " 在组: " << groupName;
            } else {
                log(service::LogLevel::INFO) << "用户 " << userId << " 不在组: " << groupName;
            }
            return inGroup;
        }
    }
}

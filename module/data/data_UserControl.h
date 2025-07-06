//
// Created by gouzuang on 25-7-6.
//

#ifndef USERCONTROL_H
#define USERCONTROL_H
#include "service/database/databaseManager.h"

namespace data::UserControl {
    namespace Login {
        /**
         * @brief 创建用户表。
         *
         * 该函数检查名为"users"的表是否存在。如果不存在，则创建一个新表.
         */
        void createUserTable();

        /**
         * @brief 检查给定的用户邮箱和密码是否有效。
         *
         * @param email 用户的电子邮件地址
         * @param password 用户的密码
         * @return int 返回值为：大于0的整数表示用户ID，负数表示错误代码。
         */
        int isUserPasswordValid(const QString &email, const QString &password);

        /**
         * @brief 创建新用户。
         *
         * @param email 用户的电子邮件地址
         * @param username 用户名
         * @param password 用户密码
         * @return bool 返回true表示创建成功，false表示失败或用户已存在。
         */
        bool createNewUser(const QString &email, const QString &username, const QString &password);
    }

    namespace permission {
        /**
         * @brief 创建用户组表。
         *
         * 该函数检查名为"groups"的表是否存在。如果不存在，则创建一个新表，用于存储用户组信息。
         */
        void createGroupTable();

        /**
         * @brief 创建用户-组关联表。
         *
         * 该函数检查名为"user_groups"的表是否存在。如果不存在，则创建一个新表，用于存储用户和组之间的关联信息。
         */
        void createUserGroupTable();

        /**
         * @brief 获取用户所在的组名。
         *
         * @param userId 用户ID
         * @return QString 返回组名，用逗号分割，如果用户不在任何组中则返回空字符串。
         */
        QString getUserInWhichGroup(int userId);

        /**
         * @brief 检查用户是否在指定的组中。
         *
         * @param userId 用户ID
         * @param groupName 组名
         * @return bool 返回true表示用户在组中，false表示不在组中。
         */
        bool isUserInGroup(int userId, const QString &groupName);
    }
} // namespace data::UserControl

#endif //USERCONTROL_H

//
// Created by gouzuang on 25-7-6.
//

#ifndef USERCONTROL_H
#define USERCONTROL_H
#include "service/database/databaseManager.h"

namespace data::UserControl {
    void dropDB();
    void buildDB();
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

        /**
         * @brief 创建新用户
         *
         * 该函数尝试在数据库中创建一个新用户。如果提供的电子邮件已经存在于数据库中，则不会创建新用户。
         * 成功创建用户后，该函数还将尝试将用户添加到指定的组中（如果提供了组名）。
         *
         * @param email 用户的电子邮件地址，用于唯一标识用户。
         * @param username 用户名，用户的显示名称。
         * @param password 用户密码。
         * @param group 用户所属的组名，默认为空字符串表示不添加到任何组。
         * @return 如果用户成功创建返回true；如果用户已存在或创建过程中发生错误则返回false。
         */
        bool createNewUser(const QString &email, const QString &username, const QString &password,
                           const QString group);

        /**
         * @brief 通过电子邮件查找用户ID。
         *
         * 此函数尝试根据给定的电子邮件地址在数据库中查找对应的用户ID。如果找到匹配项，则返回用户的ID；否则，返回-1表示未找到用户。
         *
         * @param email 用户的电子邮件地址
         * @return 返回找到的用户ID，如果未找到则返回-1
         */
        int foundUserIdByEmail(const QString &email);
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

        /**
        * 创建一个新的用户组。
        *
        * @param name 组的名称，必须是唯一的。
        * @param description 组的描述信息。
        * @return 如果成功创建了新组，则返回true；如果组已存在或创建过程中出现错误，则返回false。
        */
        bool createGroup(const QString &name, const QString &description);

        /**
        * @brief 将指定用户添加到指定组中
        *
        * 该函数首先检查目标组是否存在，如果不存在则返回false。
        * 如存在，则进一步检查用户是否已经存在于该组中，若已存在则直接返回true。
        * 若用户不在该组中，则尝试将用户添加
        组，并根据操作结果返回相应的布尔值。
        *
        * @param userId 要添加到组中的用户的ID
        * @param groupName 目标组的名称
        * @return 操作成功时返回true，失败时返回false。当组不存在或用户已存在于组中时也视为特定条件下的“成功”并返回相应布尔值。
        */
        bool addUserToGroup(int userId, const QString &groupName);
    }
} // namespace data::UserControl

#endif //USERCONTROL_H

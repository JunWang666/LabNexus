//
// Created by gouzuang on 25-7-6.
//

#ifndef USERCONTROL_H
#define USERCONTROL_H
#include <expected>
#include<pch.h>
#include "service/database/databaseManager.h"

namespace data::UserControl {
    inline QString path = "./user.db";

    enum class UserControlError {
        UserNotFound,
        IncorrectPassword,
        UserAlreadyExists,
        GroupNotFound,
        UserAlreadyInGroup,
        GroupAlreadyExists,
        DatabaseError,
        MutiResultFound
    };

    inline int currentUserId = -1;

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
         * @brief 检查给定的用户学工号和密码是否有效。
         *
         * @param idNumber 用户的学工号
         * @param password 用户的密码
         * @return std::expected<int, UserControlError> 成功时包含用户ID，失败时包含错误类型。
         */
        std::expected<int, UserControlError> isUserPasswordValid(const QString &idNumber, const QString &password);

        /**
         * @brief 创建新用户。
         *
         * @param idNumber 用户的学工号
         * @param username 用户名
         * @param password 用户密码
         * @return std::expected<int, UserControlError> 成功时包含新创建用户的ID，失败时包含错误类型。
         */
        std::expected<int, UserControlError> createNewUser(const QString &idNumber, const QString &username,
                                                           const QString &password);

        /**
         * @brief 创建新用户并可选添加到指定组。
         *
         * @param idNumber 用户的学工号
         * @param username 用户名
         * @param password 用户密码
         * @param group 用户所属的组名，默认空表示不添加到组。
         * @return std::expected<int, UserControlError> 成功时包含创建用户的ID，失败时包含错误类型。
         */
        std::expected<int, UserControlError> createNewUser(const QString &idNumber, const QString &username,
                                                           const QString &password,
                                                           const QString &group);

        /**
         * @brief 通过学工号查找用户ID。
         *
         * 此函数尝试根据给定的学工号在数据库中查找对应的用户ID。
         * @return std::expected<int, UserControlError> 成功时包含用户ID，若用户不存在则返回UserNotFound错误。
         */
        std::expected<int, UserControlError> foundUserIdByIdNumber(const QString &idNumber);

        /**
         * @brief 删除指定ID的用户
         *
         * 该函数尝试根据提供的用户ID删除一个用户。如果用户存在，则将用户及其在user_groups表中的关联标记为已删除。
         * 如果用户不存在或数据库操作失败，将返回相应的错误。
         *
         * @param userId 要删除用户的ID
         * @return 成功时返回true；如果用户不存在或发生数据库错误，则返回UserControlError枚举值
         */
        std::expected<bool, UserControlError> deleteUserById(int userId);

        /**
         * 更新指定用户的密码。
         *
         * @param userId 用户的唯一标识符。
         * @param newPassword 新密码，将被设置为该用户的密码。
         * @return 如果密码更新成功，则返回true；如果发生数据库错误，则返回一个包含UserControlError::DatabaseError的std::unexpected对象。
         */
        std::expected<bool, UserControlError> updateUserPassword(int userId, const QString &newPassword);
    }

    namespace permission {
        /**
         * @brief 创建用户组表。
         *
         * 该函数检查数据库中是否已存在名为"groups"的表。
         *
         * @note 该函数使用了`service::DatabaseManager`来管理与数据库的交互，并通过执行SQL语句来创建表。
         */
        void createGroupTable();

        /**
         * @brief 创建用户组关联表。
         *
         * 该函数检查数据库中是否存在名为 "user_groups" 的表，如果不存在，则创建一个新的表。
         */
        void createUserGroupTable();

        /**
         * @brief 创建新用户组。
         *
         * @param name 组的名称，必须唯一。
         * @param description 组的描述信息。
         * @return std::expected<bool, UserControlError> 成功时为true，失败时包含错误类型。
         */
        std::expected<bool, UserControlError> createGroup(const QString &name, const QString &description);

        /**
         * @brief 将指定用户添加到组中。
         *
         * @param userId 要添加到组中的用户的ID
         * @param groupName 目标组的名称
         * @return std::expected<bool, UserControlError> 成功时为true，失败时包含错误类型。
         */
        std::expected<bool, UserControlError> addUserToGroup(int userId, const QString &groupName);

        /**
         * @brief 将指定用户添加到组中。
         *
         * @param userId 要添加到组中的用户的ID
         * @param groupId 目标组Id
         * @return std::expected<bool, UserControlError> 成功时为true，失败时包含错误类型。
         */
        std::expected<bool, UserControlError> addUserToGroup(int userId, int groupId);

        /**
         * @brief 获取指定用户所属的所有组。
         *
         * 该函数通过查询数据库，获取给定用户ID所属的所有组，并将这些组名以逗号分隔的字符串形式返回。如果用户不属于任何组，则返回空字符串。
         *
         * @param userId 用户ID
         * @return QString 包含用户所属组名的字符串，多个组名之间用逗号分隔；如果用户不属于任何组，则返回空字符串。
         */
        QString getUserInWhichGroup(int userId);

        /**
         * @brief 获取指定用户所属的所有组名列表。
         *
         * 该函数通过查询数据库，获取给定用户ID所属的所有组，并将这些组名以QVector<QString>的形式返回。如果用户不属于任何组，则返回一个空的QVector。
         *
         * @param userId 用户ID
         * @return QVector<QString> 包含用户所属组名的字符串向量；如果用户不属于任何组，则返回一个空的QVector。
         */
        QStringList getUserInWhichGroupList(int userId);

        /**
         * @brief 获取指定用户所属的所有组ID列表。
         *
         * @param userId 用户ID
         * @return QVector<int> 包含用户所属组ID的int向量；如果用户不属于任何组，则返回一个空的QVector。
         */
        QList<int> getUserInWhichGroupIdList(int userId);

        /**
         * @brief 根据组名搜索组ID。
         *
         * 该函数通过查询数据库，获取与给定组名匹配的所有组ID，并将这些ID以QVector<int>的形式返回。如果没有找到匹配的组，则返回一个空的QVector。
         *
         * @param groupName 组名
         * @return QList<int> 包含与组名匹配的组ID列表；如果没有找到匹配的组，则返回一个空的QList。
         */
        QList<int> searchGroupIdByName(const QString &groupName);

        /**
         * @brief 从指定组中删除用户。
         *
         * 此函数尝试将给定的用户ID从指定的组名中移除。如果操作成功，则用户不再属于该组。
         * 如果提供的用户ID或组名无效，或者数据库操作失败，将返回相应的错误。
         *
         * @param userId 要从组中删除的用户的ID
         * @param groupName 用户所属的组名
         * @return std::expected<bool, UserControlError> 成功时为true，失败时包含错误类型。
         */
        void deleteUserFromGroup(int userId, const QString &groupName);

        /**
         * @brief 检查用户是否属于指定的组。
         *
         * 该函数通过获取用户所在的所有组，然后检查这些组中是否包含指定的目标组名来判断用户是否属于该组。
         *
         * @param userId 用户ID
         * @param groupName 组名
         * @return bool 如果用户在指定的组中则返回true，否则返回false。
         */
        bool isUserInGroup(int userId, const QString &groupName);

        /**
         * @brief 检查用户是否属于指定的组。
         *
         * 该函数通过获取用户所在的所有组，然后检查这些组中是否包含指定的目标组名来判断用户是否属于该组。
         *
         * @param userId 用户ID
         * @param groupId 组ID
         * @return bool 如果用户在指定的组中则返回true，否则返回false。
         */
        bool isUserInGroup(int userId, int groupId);

        /**
         * @brief 获取所有组的ID列表。
         *
         * @return QList<int> 所有组ID列表。
         */
        QList<int> getAllGroupId();
    }

    namespace UserInfo {
        enum class UserInfoError {
            UserNotFound
        };

        /**
         * @brief 根据用户ID获取用户名。
         *
         * @param userId 用户ID
         * @return std::expected<QString, UserInfoError> 成功时包含用户名，失败时返回UserNotFound错误。
         */
        std::expected<QString, UserInfoError> getUserNameById(int userId);

        /**
         * @brief 更改指定用户的用户名。
         *
         * 该函数通过用户ID查找用户，并将其用户名更新为新的名称。如果更新失败，将抛出一个异常并记录错误。
         *
         * @param userId 用户的唯一标识符。
         * @param newName 新的用户名。
         * @throws std::runtime_error 如果更新用户名失败，则抛出此异常。
         */
        void changeUserName(int userId, const QString &newName);
    }
}


#endif //USERCONTROL_H

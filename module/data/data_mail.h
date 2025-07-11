//
// Created by gouzuang on 25-7-8.
//
#pragma once

#ifndef DATA_MAIL_H
#define DATA_MAIL_H
#include "pch.h"
#include <QList>
#include <QString>
#include <QDateTime>

namespace data::mail {
    inline QString path = service::Path::mail();

    struct Mail {
        int id;
        int sender_id;
        int receiver_id;
        QString subject;
        QString content;
        QDateTime send_date;
        int status;
        QString extra_data;
    };

    /**
     * @brief 创建邮件数据库文件和邮件表
     *
     * 该方法首先检查指定路径下的邮件数据库文件是否存在。
     * - 如果不存在，则尝试创建一个新的空数据库文件，并记录相关信息。
     *   - 成功创建后，调用`createMailTable()`来初始化邮件表结构。
     *   - 如果在创建过程中遇到错误（例如权限问题），则会记录错误信息。
     * - 如果已存在，仅记录一条信息说明数据库文件已经存在。
     *
     * @note 此方法依赖于`QFile`类来操作文件系统以及自定义的日志函数`log`来进行日志记录。
     * @warning 在执行此操作前，请确保应用程序有足够的权限去读写指定的路径。
     */
    void buildDB();

    /**
     * @brief 删除数据库文件。
     *
     * 此函数尝试删除指定路径的数据库文件。如果文件存在且成功删除，则记录一条信息日志；如果删除失败，则记录错误日志；
     * 如果文件不存在，则记录一条信息日志表示文件不存在。
     */
    void dropDB();

    void registerSystemUser();

    void findSystemUser();

    /**
     * @brief 创建邮件表
     *
     * 该方法用于在数据库中创建一个名为`mail`的表。如果该表已经存在，则不会执行任何操作。
     * 表结构包括：主键ID、发送者ID、接收者ID、主题、内容、发送日期、状态及额外数据字段。
     * 发送者ID和接收者ID作为外键引用`users`表中的ID。
     *
     * @note 此函数依赖于`service::DatabaseManager`类来检查表是否存在并执行SQL语句。
     *       如果成功创建了表或表已存在，*/
    void createMailTable();

    /**
     * 发送邮件到指定接收者。
     *
     * @param senderId 发送者的用户ID
     * @param receiverId 接收者的用户ID
     * @param subject 邮件主题
     * @param content 邮件内容
     * @param extra_data 附加数据，JSON格式字符串
     */
    void send_mail(int senderId, int receiverId, const QString &subject, const QString &content,
                   const QString &extra_data={});

    /**
     * @brief 获取特定接收者的邮件列表，支持分页。
     *
     * 该函数从数据库中获取给定接收者ID的邮件列表，并支持分页。结果按发送日期降序排列。
     *
     * @param receiverId 要获取邮件的接收者的ID。
     * @param page 要获取的结果页码。
     * @param pageSize 每页的结果数量。
     * @return QList<Mail> 表示邮件的Mail对象列表。
     */
    QList<Mail> getAllMails(int receiverId, int page, int pageSize = 5);

    /**
     * 获取指定接收者的未读邮件列表。
     *
     * @param receiverId 邮件接收者的ID。
     * @param page 从数据库中获取的页码。
     * @param pageSize 每页的项目数。
     * @return 包含Mail对象的QList，每个对象代表一封未读邮件。
     */
    QList<Mail> getUnreadMails(int receiverId, int page, int pageSize=5);

    Mail getMailById(int mailId);

    /**
         * @brief 计算特定接收者的邮件所需的总页数。
         *
         * 该函数查询数据库以统计给定接收者的全部邮件数量，然后根据提供的每页邮件数量计算需要多少页来显示这些邮件。
         * 计算是通过将邮件总数除以每页邮件数量并向上取整来完成的。
         *
         * @param receiverId 邮件接收者的ID。
         * @return 显示所有接收者邮件总数。
         */
    int getMailCount(int receiverId);

    /**
     * 计算给定接收者的未读邮件所需的页数。
     *
     * @param receiverId 邮件接收者的ID。
     * @return 显示所有未读邮件总数。如果没有未读邮件，则返回0。
     */
    int getUnreadMailCount(int receiverId);

    inline QMap<QString, int> systemReservedAccounts;

    bool setMailRead(int mailId);
}


#endif //DATA_MAIL_H

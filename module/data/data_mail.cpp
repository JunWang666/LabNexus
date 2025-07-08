//
// Created by gouzuang on 25-7-8.
//

#include "data_mail.h"
#include "pch.h"

#include <QFile>

namespace data::mail {
    /**
     * @brief 删除数据库文件。
     *
     * 此函数尝试删除指定路径的数据库文件。如果文件存在且成功删除，则记录一条信息日志；如果删除失败，则记录错误日志；
     * 如果文件不存在，则记录一条信息日志表示文件不存在。
     */
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
    void buildDB() {
        QFile dbFile(path);
        if (!dbFile.exists()) {
            if (dbFile.open(QIODevice::WriteOnly)) {
                dbFile.close();
                log(LogLevel::INFO) << "邮件数据库文件创建成功";
            } else {
                log(LogLevel::ERR) << "数据库文件创建失败";
            }
            createMailTable();
        } else {
            log(LogLevel::INFO) << "数据库文件已存在";
        }
    }

    /**
     * @brief 创建邮件表
     *
     * 该方法用于在数据库中创建一个名为`mail`的表。如果该表已经存在，则不会执行任何操作。
     * 表结构包括：主键ID、发送者ID、接收者ID、主题、内容、发送日期、状态及额外数据字段。
     * 发送者ID和接收者ID作为外键引用`users`表中的ID。
     *
     * @note 此函数依赖于`service::DatabaseManager`类来检查表是否存在并执行SQL语句。
     *       如果成功创建了表或表已存在，*/
    void createMailTable() {
        service::DatabaseManager db(path);
        if (!db.tableExists("mail")) {
            QString createTableQuery = R"(
                CREATE TABLE mail (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    sender_id INTEGER NOT NULL,
                    receiver_id INTEGER NOT NULL,
                    subject TEXT NOT NULL,
                    content TEXT NOT NULL,
                    send_date DATETIME NOT NULL,
                    status INTEGER NOT NULL DEFAULT 0,
                    extra_data TEXT NOT NULL DEFAULT '{}',
                    FOREIGN KEY(sender_id) REFERENCES users(id),
                    FOREIGN KEY(receiver_id) REFERENCES users(id)
                )
            )";
            db.executeNonQuery(createTableQuery);
            log(service::LogLevel::DATA) << "邮件表创建成功";
        } else {
            log(service::LogLevel::INFO) << "邮件表已存在";
        }
    }

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
                   const QString &extra_data) {
        service::DatabaseManager db(path);
        QString insertQuery = R"(
            INSERT INTO mail (sender_id, receiver_id, subject, content, send_date, status, extra_data)
            VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP, 0, ?)
        )";
        if (!db.executePreparedNonQuery(insertQuery, {senderId, receiverId, subject, content, extra_data})) {
            log(service::LogLevel::ERR) << "发送邮件失败";
        } else {
            log(service::LogLevel::DATA) << "邮件发送成功: " << subject;
        }
    }

    /**
     * @brief Retrieves a list of Mail objects for a specific receiver, paginated.
     *
     * This function fetches a list of emails from the database for a given receiver ID,
     * with pagination support. It orders the results by send date in descending order.
     *
     * @param receiverId The ID of the receiver whose mails are to be fetched.
     * @param page The page number of the results to fetch.
     * @param pageSize The number of results per page.
     * @return QList<Mail> A list of Mail objects representing the emails.
     */
    QList<Mail> getAllMails(int receiverId, int page, int pageSize) {
        service::DatabaseManager db(path);
        int offset = (page - 1) * pageSize;
        QString selectQuery =
                "SELECT id, sender_id, receiver_id, subject, content, send_date, status, extra_data FROM mail WHERE receiver_id = ? ORDER BY send_date DESC LIMIT ? OFFSET ?";
        QList<Mail> mails;
        auto query = db.executePreparedQuery(selectQuery, {receiverId, pageSize, offset});
        while (query.next()) {
            Mail mail;
            mail.id = query.value(0).toInt();
            mail.sender_id = query.value(1).toInt();
            mail.receiver_id = query.value(2).toInt();
            mail.subject = query.value(3).toString();
            mail.content = query.value(4).toString();
            mail.send_date = QDateTime::fromString(query.value(5).toString(), Qt::ISODate);
            mail.status = query.value(6).toInt();
            mail.extra_data = query.value(7).toString();
            mails.append(mail);
        }
        return mails;
    }

    /**
     * Retrieves a list of unread mails for a specified receiver.
     *
     * @param receiverId The ID of the mail receiver.
     * @param page The page number to fetch from the database.
     * @param pageSize The number of items per page.
     * @return A QList containing Mail objects, each representing an unread mail.
     */
    QList<Mail> getUnreadMails(int receiverId, int page, int pageSize) {
        service::DatabaseManager db(path);
        int offset = (page - 1) * pageSize;
        QString selectQuery =
                "SELECT id, sender_id, receiver_id, subject, content, send_date, status, extra_data FROM mail WHERE receiver_id = ? AND status = 0 ORDER BY send_date DESC LIMIT ? OFFSET ?";
        QList<Mail> mails;
        auto query = db.executePreparedQuery(selectQuery, {receiverId, pageSize, offset});
        while (query.next()) {
            Mail mail;
            mail.id = query.value(0).toInt();
            mail.sender_id = query.value(1).toInt();
            mail.receiver_id = query.value(2).toInt();
            mail.subject = query.value(3).toString();
            mail.content = query.value(4).toString();
            mail.send_date = QDateTime::fromString(query.value(5).toString(), Qt::ISODate);
            mail.status = query.value(6).toInt();
            mail.extra_data = query.value(7).toString();
            mails.append(mail);
        }
        return mails;
    }

    /**
     * @brief Calculates the total number of pages for a specific receiver's mail based on page size.
     *
     * This function queries the database to count all mails for a given receiver and then calculates
     * how many pages are needed to display these mails, considering the provided page size. The
     * calculation is done by dividing the total count of mails by the page size and rounding up.
     *
     * @param receiverId The ID of the mail receiver.
     * @param pageSize The number of mails per page.
     * @return The total number of pages required to display all of the receiver's mails with the given page size.
     */
    int getMailPageCount(int receiverId, int pageSize) {
        service::DatabaseManager db(path);
        QString countQuery = "SELECT COUNT(*) FROM mail WHERE receiver_id = ?";
        auto query = db.executePreparedQuery(countQuery, {receiverId});
        if (query.next()) {
            int totalCount = query.value(0).toInt();
            return (totalCount + pageSize - 1) / pageSize; // 向上取整
        }
        return 0;
    }

    /**
     * Calculates the number of pages needed to display all unread mails for a given receiver.
     *
     * @param receiverId The ID of the mail receiver.
     * @param pageSize The number of items per page.
     * @return The total number of pages required to display all unread mails. Returns 0 if there are no unread mails.
     */
    int getUnreadMailCount(int receiverId, int pageSize) {
        service::DatabaseManager db(path);
        QString countQuery = "SELECT COUNT(*) FROM mail WHERE receiver_id = ? AND status = 0";
        auto query = db.executePreparedQuery(countQuery, {receiverId});
        if (query.next()) {
            return (query.value(0).toInt() + pageSize - 1) / pageSize;
        }
        return 0;
    }
}

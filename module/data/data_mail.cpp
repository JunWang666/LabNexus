//
// Created by gouzuang on 25-7-8.
//

#include "data_mail.h"
#include "pch.h"

#include <QFile>

namespace data::mail {
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
                log(LogLevel::INFO) << "邮件数据库文件创建成功";
            } else {
                log(LogLevel::ERR) << "数据库文件创建失败";
            }
            createMailTable();
        } else {
            log(LogLevel::INFO) << "数据库文件已存在";
        }
    }

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

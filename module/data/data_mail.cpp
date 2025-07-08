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
                return;
            }
            createMailTable();
        } else {
            log(LogLevel::INFO) << "数据库文件已存在";
        }
    }

    void createMailTable() {
        service::DatabaseManager db(path);
        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return;
        }

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

            if (db.executeNonQuery(createTableQuery)) {
                log(service::LogLevel::DATA) << "邮件表创建成功";
            } else {
                log(service::LogLevel::ERR) << "邮件表创建失败: " << db.getLastError();
            }
        } else {
            log(service::LogLevel::INFO) << "邮件表已存在";
        }
    }

    void send_mail(int senderId, int receiverId, const QString &subject, const QString &content,
                   const QString &extra_data) {
        service::DatabaseManager db(path);
        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return;
        }

        QString insertQuery = R"(
            INSERT INTO mail (sender_id, receiver_id, subject, content, send_date, status, extra_data)
            VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP, 0, ?)
        )";

        if (db.executePreparedNonQuery(insertQuery, {senderId, receiverId, subject, content, extra_data})) {
            log(service::LogLevel::DATA) << "邮件发送成功: " << subject;
        } else {
            log(service::LogLevel::ERR) << "发送邮件失败: " << db.getLastError();
        }
    }

    // 私有辅助函数，用于将查询结果转换为Mail对象
    static Mail createMailFromRecord(const QVariantMap &record) {
        Mail mail;
        mail.id = record["id"].toInt();
        mail.sender_id = record["sender_id"].toInt();
        mail.receiver_id = record["receiver_id"].toInt();
        mail.subject = record["subject"].toString();
        mail.content = record["content"].toString();
        mail.send_date = QDateTime::fromString(record["send_date"].toString(), Qt::ISODate);
        mail.status = record["status"].toInt();
        mail.extra_data = record["extra_data"].toString();
        return mail;
    }

    QList<Mail> getAllMails(int receiverId, int page, int pageSize) {
        service::DatabaseManager db(path);
        QList<Mail> mails;

        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return mails;
        }

        int offset = (page - 1) * pageSize;
        QString selectQuery = R"(
            SELECT id, sender_id, receiver_id, subject, content, send_date, status, extra_data
            FROM mail
            WHERE receiver_id = ?
            ORDER BY send_date DESC
            LIMIT ? OFFSET ?
        )";

        auto results = db.executePreparedQueryAndFetchAll(selectQuery, {receiverId, pageSize, offset});

        for (const auto &record: results) {
            mails.append(createMailFromRecord(record));
        }

        return mails;
    }

    QList<Mail> getUnreadMails(int receiverId, int page, int pageSize) {
        service::DatabaseManager db(path);
        QList<Mail> mails;

        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return mails;
        }

        int offset = (page - 1) * pageSize;
        QString selectQuery = R"(
            SELECT id, sender_id, receiver_id, subject, content, send_date, status, extra_data
            FROM mail
            WHERE receiver_id = ? AND status = 0
            ORDER BY send_date DESC
            LIMIT ? OFFSET ?
        )";

        auto results = db.executePreparedQueryAndFetchAll(selectQuery, {receiverId, pageSize, offset});

        for (const auto &record: results) {
            mails.append(createMailFromRecord(record));
        }

        return mails;
    }

    int getMailCount(int receiverId) {
        service::DatabaseManager db(path);
        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return 0;
        }

        QString countQuery = "SELECT COUNT(*) as count FROM mail WHERE receiver_id = ?";
        auto results = db.executePreparedQueryAndFetchAll(countQuery, {receiverId});

        if (!results.isEmpty()) {
            return results.first()["count"].toInt();
        }

        return 0;
    }

    int getUnreadMailCount(int receiverId) {
        service::DatabaseManager db(path);
        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return 0;
        }

        QString countQuery = "SELECT COUNT(*) as count FROM mail WHERE receiver_id = ? AND status = 0";
        auto results = db.executePreparedQueryAndFetchAll(countQuery, {receiverId});

        if (!results.isEmpty()) {
            return results.first()["count"].toInt();
        }

        return 0;
    }
}

﻿//
// Created by gouzuang on 25-7-8.
//

#include "data_mail.h"

#include <dwmapi.h>

#include "pch.h"

#include <QFile>

#include "service/stastic/staticdata.h"

namespace data::mail {
    void dropDB() {
        service::DatabaseManager db(service::Path::mail());
        if (db.isConnected()) {
            db.executeNonQuery("DROP TABLE IF EXISTS mail");
            log(LogLevel::INFO) << "数据库表删除成功 (mail)";
        } else {
            log(LogLevel::ERR) << "数据库连接失败，无法删除表";
        }
    }

    void registerSystemUser() {
        data::UserControl::Login::createNewUser("System_3", "库存预警", "huidbauiuicbabiabduiab", "System");
        data::UserControl::Login::createNewUser("System_2", "LabNexus团队", "bcfuiasbiasuibcviuab", "System");
        data::UserControl::Login::createNewUser("System_1", "工作提醒", "bcfuiasbiasuibcviuab", "System");
    }

    void findSystemUser() {
        std::vector<QString> searchIds = {"System_1", "System_2", "System_3",};
        for (QString id : searchIds) {
            auto userNameResult = data::UserControl::UserInfo::getUserNameByIdNumber(id);
            auto id2 = data::UserControl::Login::foundUserIdByIdNumber(id);
            if (userNameResult and id2.has_value()) {
                data::mail::systemReservedAccounts.insert(userNameResult.value(),id2.value());
                log(LogLevel::INFO) << "学工号: " << id << "，用户名: " << userNameResult.value();
            } else {
                log(LogLevel::ERR) << "查找学工号为 " << id << " 的用户名失败";
            }
        }
        log(LogLevel::DEBUG)<<data::mail::systemReservedAccounts;
    }

    void buildDB() {
        createMailTable();
    }

    void createMailTable() {
        service::DatabaseManager db(service::Path::mail());
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
        service::DatabaseManager db(service::Path::mail());
        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return;
        }

        QString insertQuery = R"(
            INSERT INTO mail (sender_id, receiver_id, subject, content, send_date, status, extra_data)
            VALUES (?, ?, ?, ?, strftime('%Y-%m-%dT%H:%M:%fZ', 'now'), 0, ?)
        )";

        // 保证 extra_data 为空时传递空字符串，避免违反 NOT NULL 约束
        QString extra = extra_data.isEmpty() ? "" : extra_data;
        if (db.executePreparedNonQuery(insertQuery, {senderId, receiverId, subject, content, extra})) {
            log(service::LogLevel::DATA) << "邮件发送成功: " << subject;
        } else {
            log(service::LogLevel::ERR) << "发送邮件失败: " << db.getLastError();
        }
    }

    static Mail createMailFromRecord(const QVariantMap &record) {
        Mail mail;
        mail.id = record["id"].toInt();
        mail.sender_id = record["sender_id"].toInt();
        mail.receiver_id = record["receiver_id"].toInt();
        mail.subject = record["subject"].toString();
        mail.content = record["content"].toString();
        mail.send_date = QDateTime::fromString(record["send_date"].toString(), Qt::ISODate);
        mail.send_date = mail.send_date.toTimeZone(service::ApplicationTimeZone);
        mail.status = record["status"].toInt();
        mail.extra_data = record["extra_data"].toString();
        return mail;
    }

    QList<Mail> getAllMails(int receiverId, int page, int pageSize) {
        service::DatabaseManager db(service::Path::mail());
        QList<Mail> mails;
        log(LogLevel::INFO) << "获取邮件: 接收者ID: " << receiverId << ", 页码: " << page << ", 每页大小: " << pageSize;
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
        service::DatabaseManager db(service::Path::mail());
        QList<Mail> mails;

        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return mails;
        }
        log(LogLevel::INFO) << "获取未读邮件: 接收者ID: " << receiverId << ", 页码: " << page << ", 每页大小: " << pageSize;

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

    Mail getMailById(int mailId) {
        service::DatabaseManager db(service::Path::mail());
        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return Mail();
        }
        QString selectQuery = R"(
            SELECT id, sender_id, receiver_id, subject, content, send_date, status, extra_data
            FROM mail
            WHERE id = ?
        )";
        auto results = db.executePreparedQueryAndFetchAll(selectQuery, {mailId});
        if (!results.isEmpty()) {
            return createMailFromRecord(results.first());
        }
        log(service::LogLevel::ERR) << "未找到邮件: ID = " << mailId;
        return Mail();
    }

    int getMailCount(int receiverId) {
        service::DatabaseManager db(service::Path::mail());
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
        service::DatabaseManager db(service::Path::mail());
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

    bool setMailRead(int mailId) {
        service::DatabaseManager db(service::Path::mail());
        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "数据库连接失败: " << db.getLastError();
            return false;
        }
        QString updateQuery = "UPDATE mail SET status = 1 WHERE id = ?";
        if (db.executePreparedNonQuery(updateQuery, {mailId})) {
            log(service::LogLevel::DATA) << "邮件已标记为已读: ID = " << mailId;
            return true;
        } else {
            log(service::LogLevel::ERR) << "标记邮件为已读失败: ID = " << mailId << ", 错误: " << db.getLastError();
            return false;
        }
    }

}

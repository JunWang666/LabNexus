//
// Created by gouzuang on 25-7-7.
//

#include "data_Booking.h"

namespace data::Booking {
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

            createBookingApprovalTable();
            createBookingEquipmentTable();
            createBookingInfoTable();
            createBookingTimeTable();
        } else {
            log(service::LogLevel::INFO) << "数据库文件已存在";
        }
    }


    // 表1：申请基本信息表
    void createBookingInfoTable() {
        service::DatabaseManager db(path);
        if (!db.tableExists("booking_info")) {
            QString createTableQuery = R"(
                CREATE TABLE booking_info (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    user_id INTEGER NOT NULL,
                    create_date DATETIME NOT NULL,
                    FOREIGN KEY(user_id) REFERENCES users(id)
                )
            )";
            db.executeNonQuery(createTableQuery);
            log(service::LogLevel::DATA) << "申请基本信息表创建成功";
        } else {
            log(service::LogLevel::INFO) << "申请基本信息表已存在";
        }
    }

    // 表2：目标设备表
    void createBookingEquipmentTable() {
        service::DatabaseManager db(path);
        if (!db.tableExists("booking_equipment")) {
            QString createTableQuery = R"(
                CREATE TABLE booking_equipment (
                    booking_id INTEGER PRIMARY KEY,
                    equipment_class_id INTEGER NOT NULL,
                    equipment_id INTEGER NOT NULL,
                    FOREIGN KEY(booking_id) REFERENCES booking_info(id),
                    FOREIGN KEY(equipment_class_id) REFERENCES equipment_class(id),
                    FOREIGN KEY(equipment_id) REFERENCES equipment(id)
                )
            )";
            db.executeNonQuery(createTableQuery);
            log(service::LogLevel::DATA) << "目标设备表创建成功";
        } else {
            log(service::LogLevel::INFO) << "目标设备表已存在";
        }
    }

    // 表3：时间表
    void createBookingTimeTable() {
        service::DatabaseManager db(path);
        if (!db.tableExists("booking_time")) {
            QString createTableQuery = R"(
                CREATE TABLE booking_time (
                    booking_id INTEGER PRIMARY KEY,
                    request_start_time DATETIME NOT NULL,
                    request_end_time DATETIME NOT NULL,
                    actual_start_time DATETIME,
                    actual_end_time DATETIME,
                    FOREIGN KEY(booking_id) REFERENCES booking_info(id)
                )
            )";
            db.executeNonQuery(createTableQuery);
            log(service::LogLevel::DATA) << "时间表创建成功";
        } else {
            log(service::LogLevel::INFO) << "时间表已存在";
        }
    }

    // 表4：审批状态表
    void createBookingApprovalTable() {
        service::DatabaseManager db(path);
        if (!db.tableExists("booking_approval")) {
            QString createTableQuery = R"(
                CREATE TABLE booking_approval (
                    booking_id INTEGER PRIMARY KEY,
                    approval_status TEXT NOT NULL,
                    approval_time DATETIME,
                    approver_id INTEGER,
                    FOREIGN KEY(booking_id) REFERENCES booking_info(id),
                    FOREIGN KEY(approver_id) REFERENCES users(id)
                )
            )";
            db.executeNonQuery(createTableQuery);
            log(service::LogLevel::DATA) << "审批状态表创建成功";
        } else {
            log(service::LogLevel::INFO) << "审批状态表已存在";
        }
    }

    QList<fullBookingRecord> loadBookingFullRecords() {
        service::DatabaseManager db(path);
        QList<fullBookingRecord> records;
        QString queryStr = R"(
            SELECT bi.id,
                   bi.user_id,
                   bi.create_date,
                   be.equipment_class_id,
                   be.equipment_id,
                   bt.request_start_time,
                   bt.request_end_time,
                   bt.actual_start_time,
                   bt.actual_end_time,
                   ba.approval_status,
                   ba.approval_time,
                   ba.approver_id
              FROM booking_info bi
              LEFT JOIN booking_equipment be ON be.booking_id = bi.id
              LEFT JOIN booking_time bt ON bt.booking_id = bi.id
              LEFT JOIN booking_approval ba ON ba.booking_id = bi.id
        )";
        auto query = db.executeQuery(queryStr);
        while (query.next()) {
            fullBookingRecord rec;
            rec.id = query.value("id").toInt();
            rec.userId = query.value("user_id").toInt();
            rec.createDate = query.value("create_date").toDateTime();
            rec.requestStartDate = query.value("request_start_time").toDateTime();
            rec.requestEndDate = query.value("request_end_time").toDateTime();
            rec.actualStartDate = query.value("actual_start_time").toDateTime();
            rec.actualEndDate = query.value("actual_end_time").toDateTime();
            rec.approvalStatus = query.value("approval_status").toString();
            rec.approvalDate = query.value("approval_time").toDateTime();
            rec.approverID = query.value("approver_id").toInt();

            auto userNameResult = data::UserControl::UserInfo::getUserNameById(rec.userId);
            if (userNameResult) {
                rec.userName = *userNameResult;
            } else {
                rec.userName = "未知";
                log(LogLevel::ERR) << "无法获取申请人人名称: " << rec.userId;
            }

            auto approverNameResult = data::UserControl::UserInfo::getUserNameById(rec.approverID);
            if (approverNameResult) {
                rec.approverName = *approverNameResult;
            } else {
                rec.approverName = "未知";
                log(LogLevel::ERR) << "无法获取审批人名称: " << rec.approverID;
            }

            records.append(rec);
        }
        return records;
    }

    bool updateBookingInfoField(int bookingId, const QString &fieldName, const QVariant &value) {
        return true;
    }

    bool updateBookingTimeField(int bookingId, const QString &fieldName, const QVariant &value) {
        return false;
    }

    bool updateBookingApprovalField(int bookingId, const QString &fieldName, const QVariant &value) {
        return false;
    }
}

//
// Created by gouzuang on 25-7-7.
//

#include "data_Booking.h"

namespace data::Booking {
    void buildDB() {
    }


    // 表1：申请基本信息表
    void createBookingInfoTable() {
        service::DatabaseManager db("./booking.db");
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
        service::DatabaseManager db("./booking.db");
        if (!db.tableExists("booking_equipment")) {
            QString createTableQuery = R"(
                CREATE TABLE booking_equipment (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    booking_id INTEGER NOT NULL,
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
        service::DatabaseManager db("./booking.db");
        if (!db.tableExists("booking_time")) {
            QString createTableQuery = R"(
                CREATE TABLE booking_time (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    booking_id INTEGER NOT NULL,
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
        service::DatabaseManager db("./booking.db");
        if (!db.tableExists("booking_approval")) {
            QString createTableQuery = R"(
                CREATE TABLE booking_approval (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    booking_id INTEGER NOT NULL,
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
}

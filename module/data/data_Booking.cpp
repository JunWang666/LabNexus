//
// Created by gouzuang on 25-7-7.
//

#include "data_Booking.h"

namespace data::Booking {
    void dropDB() {
        service::DatabaseManager db(service::Path::booking());
        if (db.isConnected()) {
            db.executeNonQuery("DROP TABLE IF EXISTS booking_info");
            db.executeNonQuery("DROP TABLE IF EXISTS booking_equipment");
            db.executeNonQuery("DROP TABLE IF EXISTS booking_time");
            db.executeNonQuery("DROP TABLE IF EXISTS booking_approval");
            log(LogLevel::INFO) << "数据库表删除成功 (booking_info, booking_equipment, booking_time, booking_approval)";
        } else {
            log(LogLevel::ERR) << "数据库连接失败，无法删除表";
        }
    }

    void buildDB() {
        createBookingApprovalTable();
        createBookingEquipmentTable();
        createBookingInfoTable();
        createBookingTimeTable();
    }


    // 表1：申请基本信息表
    void createBookingInfoTable() {
        service::DatabaseManager db(service::Path::booking());
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
        service::DatabaseManager db(service::Path::booking());
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
        service::DatabaseManager db(service::Path::booking());
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
        service::DatabaseManager db(service::Path::booking());
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

    bool createBookingARecord(int userId, const QDateTime &createDate, int equipmentClassId, int equipmentId,
        const QDateTime &requestStartTime, const QDateTime &requestEndTime, const QString &approvalStatus,
        int approverId) {
        // 实例化你的数据库管理器
        service::DatabaseManager db(service::Path::booking());
        if (!db.isConnected()) {
            log(service::LogLevel::ERR) << "创建预订记录失败：无法连接到数据库。" << db.getLastError();
            return false;
        }

        // 1. 使用 DatabaseManager::beginTransaction() 开启事务
        if (!db.beginTransaction()) {
            log(service::LogLevel::ERR) << "开启事务失败: " << db.getLastError();
            return false;
        }

        // 2. 插入 booking_info 表并获取新ID
        //    使用你提供的 executePreparedInsertAndGetId 便捷函数
        QString infoQuery = "INSERT INTO booking_info (user_id, create_date) VALUES (?, ?)";
        QVariantList infoParams = {userId, createDate};
        int newBookingId = db.executePreparedInsertAndGetId(infoQuery, infoParams);

        // 3. 检查ID是否有效。根据你的实现，失败时返回-1
        if (newBookingId == -1) {
            log(service::LogLevel::ERR) << "插入 booking_info 并获取ID失败: " << db.getLastError();
            db.rollbackTransaction(); // 出错，回滚事务
            return false;
        }

        // 4. 使用新ID插入 booking_equipment 表
        //    使用 executePreparedNonQuery 函数
        QString equipmentQuery = "INSERT INTO booking_equipment (booking_id, equipment_class_id, equipment_id) VALUES (?, ?, ?)";
        QVariantList equipmentParams = {newBookingId, equipmentClassId, equipmentId};
        if (!db.executePreparedNonQuery(equipmentQuery, equipmentParams)) {
            log(service::LogLevel::ERR) << "插入 booking_equipment 失败: " << db.getLastError();
            db.rollbackTransaction(); // 出错，回滚事务
            return false;
        }

        // 5. 使用新ID插入 booking_time 表
        QString timeQuery = "INSERT INTO booking_time (booking_id, request_start_time, request_end_time) VALUES (?, ?, ?)";
        QVariantList timeParams = {newBookingId, requestStartTime, requestEndTime};
        if (!db.executePreparedNonQuery(timeQuery, timeParams)) {
            log(service::LogLevel::ERR) << "插入 booking_time 失败: " << db.getLastError();
            db.rollbackTransaction(); // 出错，回滚事务
            return false;
        }

        // 6. 使用新ID插入 booking_approval 表
        QString approvalQuery = "INSERT INTO booking_approval (booking_id, approval_status, approver_id) VALUES (?, ?, ?)";
        QVariantList approvalParams = {newBookingId, approvalStatus, approverId};
        if (!db.executePreparedNonQuery(approvalQuery, approvalParams)) {
            log(service::LogLevel::ERR) << "插入 booking_approval 失败: " << db.getLastError();
            db.rollbackTransaction(); // 出错，回滚事务
            return false;
        }

        // 7. 所有插入都成功，提交事务
        if (db.commitTransaction()) {
            log(service::LogLevel::INFO) << "创建完整预订记录成功, 新ID: " << newBookingId;
            return true;
        } else {
            log(service::LogLevel::ERR) << "提交事务失败: " << db.getLastError();
            db.rollbackTransaction(); // 即使提交失败，也尝试回滚
            return false;
        }
    }

    bool updateBookingOnstatus(service::DatabaseManager &db,int id, const QString &status,int approvalId) {
        QString queryString = R"(
        UPDATE booking_approval
        SET approval_status = ? , approval_time = ? , approver_id = ? WHERE booking_id = ?)";
        QVariantList parmas;
        parmas << status << QDateTime::currentDateTime() << approvalId << id;
        bool success = db.executePreparedNonQuery(queryString, parmas);
        if (!success) {
            log(LogLevel::ERR) << "修改失败" << db.getLastError();
        }
        return success;
    }

    bool processApprovalTransaction(int bookingId, int equipmentId, int borrowerId, int approverId) {
        service::DatabaseManager db(service::Path::booking());
        if (!db.beginTransaction()) {
            log(LogLevel::ERR) << "审批事务开启失败: " << db.getLastError();
            return false;
        }
        if (!data::Equipment::updateEquipmentOnLoan(equipmentId, borrowerId)) {
            log(LogLevel::ERR) << "更新设备借出人失败，回滚事务。";
            db.rollbackTransaction();
            return false;
        }
        if (!data::Equipment::updateEquipmentOnStatus(equipmentId, "借出")) {
            log(LogLevel::ERR) << "更新设备状态失败，回滚事务。";
            db.rollbackTransaction();
            return false;
        }
        if (!updateBookingOnstatus(db,bookingId,"同意",approverId)) {
            log(LogLevel::ERR) << "更新当前申请状态失败，回滚事务。";
            db.rollbackTransaction();
            return false;
        }
        QString findOthersQuery = R"(
            SELECT booking_id FROM booking_approval
            WHERE approval_status = '待审批' AND booking_id IN (
                SELECT booking_id FROM booking_equipment WHERE equipment_id = ?
            ) AND booking_id != ?
        )";
        QVariantList findParams = {equipmentId, bookingId};
        auto otherBookings = db.executePreparedQueryAndFetchAll(findOthersQuery, findParams);
        for (const auto& row : otherBookings) {
            int otherBookingId = row["booking_id"].toInt();
            // 拒绝其他申请，审批人ID可以设为0或当前审批人ID，表示系统自动拒绝
            if (!updateBookingOnstatus(db,otherBookingId, "拒绝", 0)) {
                log(LogLevel::ERR) << "自动拒绝其他申请失败 (ID: " << otherBookingId << ")，回滚事务。";
                db.rollbackTransaction();
                return false;
            }
        }
        return db.commitTransaction();

    }


    QList<fullBookingRecord> loadBookingFullRecords() {
        service::DatabaseManager db(service::Path::booking());
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
        auto results = db.executeQueryAndFetchAll(queryStr);
        for (const auto &row: results) {
            fullBookingRecord record;
            record.id = row["id"].toInt();
            record.equipmentId = row["equipment_id"].toInt();
            record.userId = row["user_id"].toInt();
            record.createDate = row["create_date"].toDateTime();
            record.requestStartDate = row["request_start_time"].toDateTime();
            record.requestEndDate = row["request_end_time"].toDateTime();
            record.actualStartDate = row["actual_start_time"].toDateTime();
            record.actualEndDate = row["actual_end_time"].toDateTime();
            record.approvalStatus = row["approval_status"].toString();
            record.approvalDate = row["approval_time"].toDateTime();
            record.approverID = row["approver_id"].toInt();
            records.append(record);
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

    bool createFullBookingRecord(int userId, const QDateTime &createDate,
                                 int equipmentClassId, int equipmentId,
                                 const QDateTime &requestStartTime, const QDateTime &requestEndTime,
                                 const QString &approvalStatus, int approverId) {
        service::DatabaseManager db(service::Path::booking());

        QString infoQuery = "INSERT INTO booking_info (user_id, create_date) VALUES (?, ?)";
        auto bookingId = db.executePreparedInsertAndGetId(infoQuery,
                                                          {
                                                              QVariant(userId),
                                                              QVariant(createDate.toString("yyyy-MM-dd hh:mm:ss"))
                                                          });

        QString equipmentQuery = QString(
                    "INSERT INTO booking_equipment (booking_id, equipment_class_id, equipment_id) VALUES (%1, %2, %3)")
                .arg(bookingId)
                .arg(equipmentClassId)
                .arg(equipmentId);
        if (!db.executeNonQuery(equipmentQuery)) return false;

        QString timeQuery = QString(
                    "INSERT INTO booking_time (booking_id, request_start_time, request_end_time) VALUES (%1, '%2', '%3')")
                .arg(bookingId)
                .arg(requestStartTime.toString("yyyy-MM-dd hh:mm:ss"))
                .arg(requestEndTime.toString("yyyy-MM-dd hh:mm:ss"));
        if (!db.executeNonQuery(timeQuery)) return false;

        QString approvalQuery = QString(
                    "INSERT INTO booking_approval (booking_id, approval_status, approver_id) VALUES (%1, '%2', %3)")
                .arg(bookingId)
                .arg(approvalStatus)
                .arg(approverId);
        if (!db.executeNonQuery(approvalQuery)) return false;

        return true;
    }
}

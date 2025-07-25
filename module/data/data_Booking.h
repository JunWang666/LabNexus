﻿//
// Created by gouzuang on 25-7-7.
//

#ifndef DATA_BOOKING_H
#define DATA_BOOKING_H
#include "pch.h"
#include <QList>
#include <QDateTime>
#include <QString>
#include <QVariant>
#include "module/data/data_EquipmentManage.h"


namespace data::Booking {
    inline QString path = service::Path::booking();

    /**
     * @brief 删除预订数据库。
     *
     * 该函数删除预订相关的数据库表和数据。
     */
    void dropDB();

    /**
     * @brief 构建预订数据库。
     *
     * 该函数创建设备管理相关的数据库表和数据。
     */
    void buildDB();

    void createBookingInfoTable();

    void createBookingEquipmentTable();

    void createBookingTimeTable();

    void createBookingApprovalTable();

    /********************/
    bool createBookingARecord(int userId, const QDateTime &createDate,
                             int equipmentClassId, int equipmentId,
                             const QDateTime &requestStartTime, const QDateTime &requestEndTime,
                             const QString &approvalStatus, int approverId);

    bool createBookingARecord(QString typeName);
    bool updateBookingOnstatus(service::DatabaseManager &db,int id,const QString& status, int approvalId);

    //决策函数
    bool processApprovalTransaction(int bookingId, int equipmentId, int borrowerId ,int approverId);
    /********************/

    /**
     * @brief 预订记录结构，用于模型展示
     */
    struct fullBookingRecord {
        int id;
        int equipmentId;
        int userId;
        QString userName;
        QString userGroup;
        QDateTime createDate;
        QDateTime requestStartDate;
        QDateTime requestEndDate;
        QDateTime actualStartDate;
        QDateTime actualEndDate;
        QString approvalStatus;
        QDateTime approvalDate;
        int approverID;
        QString approverName;
    };

    /**
     * @brief 加载所有预订基本信息记录
     */
    QList<fullBookingRecord> loadBookingFullRecords();

    /**
     * @brief 更新预订基本信息字段
     */
    bool updateBookingInfoField(int bookingId, const QString &fieldName, const QVariant &value);

    /**
     * @brief 更新预订时间表字段
     */
    bool updateBookingTimeField(int bookingId, const QString &fieldName, const QVariant &value);

    /**
     * @brief 更新审批状态表字段
     */
    bool updateBookingApprovalField(int bookingId, const QString &fieldName, const QVariant &value);

    /**
     * @brief 创建一个完整的预订记录
     */
    bool createFullBookingRecord(int userId, const QDateTime &createDate,
                                 int equipmentClassId, int equipmentId,
                                 const QDateTime &requestStartTime, const QDateTime &requestEndTime,
                                 const QString &approvalStatus, int approverId);
    }
#endif //DATA_BOOKING_H

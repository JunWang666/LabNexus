//
// Created by gouzuang on 25-7-7.
//

#include <QAbstractTableModel>
#include <QList>
#include <QDateTime>
#include <QString>
#include "../data/data_Booking.h"  // for BookingRecord and loadAllBookingRecords

#ifndef BOOKINGDATAMODEL_H
#define BOOKINGDATAMODEL_H

namespace dataModel {
    class BookingDataModel : public QAbstractTableModel {
        Q_OBJECT

    public:
        enum Column {
            Col_Id,
            Col_UserId,
            Col_UserName,
            Col_UserGroup,
            Col_CreateDate,
            Col_RequestStartDate,
            Col_RequestEndDate,
            Col_ActualStartDate,
            Col_ActualEndDate,
            Col_ApprovalStatus,
            Col_ApprovalDate,
            Col_ApproverID,
            Col_ApproverName,
            Col_Count
        };
    public:
        explicit BookingDataModel(QObject *parent = nullptr);

        // --- 必须重写的核心函数 (用于读取和展示) ---
        int rowCount(const QModelIndex &parent) const override;

        int columnCount(const QModelIndex &parent) const override;

        QVariant data(const QModelIndex &index, int role) const override;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        // --- 为了让表格可编辑，需要重写的函数 ---
        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

        Qt::ItemFlags flags(const QModelIndex &index) const override;

        //获取id
        void setCurrentUserId(int id,const QString& name);

    public slots:
        // --- 自定义公共槽函数，用于控制模型行为 ---
        void fetchData(); // 从DAL加载/刷新数据的命令
    signals:
        void approvalStatusChanged();
    private:
        // 存储从数据库加载的记录
        QList<data::Booking::fullBookingRecord> m_records;

        int approvalId = -1;//传入的审批人的id
        QString approverName = "";//传入的审批人的姓名
    };
}

#endif //BOOKINGDATAMODEL_H

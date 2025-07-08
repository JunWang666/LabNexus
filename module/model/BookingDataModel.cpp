//
// Created by gouzuang on 25-7-7.
//

#include <Qt>
#include "BookingDataModel.h"

namespace dataModel {
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

    BookingDataModel::BookingDataModel(QObject *parent)
        : QAbstractTableModel(parent) {
        fetchData();
    }

    int BookingDataModel::rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent)
        return static_cast<int>(m_records.count());
    }

    int BookingDataModel::columnCount(const QModelIndex &parent) const {
        Q_UNUSED(parent)
        return Col_Count;
    }

    QVariant BookingDataModel::data(const QModelIndex &index, int role) const {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_records.count())
            return {};
        const auto &rec = m_records.at(index.row());
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            switch (index.column()) {
                case Col_Id: return rec.id;
                case Col_UserId: return rec.userId;
                case Col_UserName: return rec.userName;
                case Col_UserGroup: return rec.userGroup;
                case Col_CreateDate: return rec.createDate;
                case Col_RequestStartDate: return rec.requestStartDate;
                case Col_RequestEndDate: return rec.requestEndDate;
                case Col_ActualStartDate: return rec.actualStartDate;
                case Col_ActualEndDate: return rec.actualEndDate;
                case Col_ApprovalStatus: return rec.approvalStatus;
                case Col_ApprovalDate: return rec.approvalDate;
                case Col_ApproverID: return rec.approverID;
                case Col_ApproverName: return rec.approverName;
                default: return {};
            }
        }
        return {};
    }

    QVariant BookingDataModel::headerData(int section, Qt::Orientation orientation, int role) const {
        if (role != Qt::DisplayRole)
            return {};
        if (orientation == Qt::Horizontal) {
            static const QStringList headers = {
                "ID", "用户ID", "用户名", "用户组", "创建时间",
                "申请开始", "申请结束", "实际开始", "实际结束",
                "审批状态", "审批时间", "审批人ID", "审批人名"
            };
            if (section >= 0 && section < headers.size())
                return headers.at(section);
        }
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    bool BookingDataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
        if (role != Qt::EditRole || !index.isValid())
            return false;
        auto &rec = m_records[index.row()];
        const int col = index.column();
        // determine which table to update
        static const QStringList allFields = {
            "id", "user_id", "user_name", "user_group", "create_date",
            "request_start_time", "request_end_time", "actual_start_time", "actual_end_time",
            "approval_status", "approval_time", "approver_id", "approver_name"
        };
        const QString &fieldName = allFields.at(col);
        bool ok = false;
        if (col > Col_Id && col <= Col_CreateDate) {
            ok = data::Booking::updateBookingInfoField(rec.id, fieldName, value);
        } else if (col >= Col_RequestStartDate && col <= Col_ActualEndDate) {
            ok = data::Booking::updateBookingTimeField(rec.id, fieldName, value);
        } else if (col >= Col_ApprovalStatus && col <= Col_ApproverName) {
            ok = data::Booking::updateBookingApprovalField(rec.id, fieldName, value);
        }
        if (ok) {
            // update local record
            switch (col) {
                case Col_UserId: rec.userId = value.toInt();
                    break;
                case Col_UserName: rec.userName = value.toString();
                    break;
                case Col_UserGroup: rec.userGroup = value.toString();
                    break;
                case Col_CreateDate: rec.createDate = value.toDateTime();
                    break;
                case Col_RequestStartDate: rec.requestStartDate = value.toDateTime();
                    break;
                case Col_RequestEndDate: rec.requestEndDate = value.toDateTime();
                    break;
                case Col_ActualStartDate: rec.actualStartDate = value.toDateTime();
                    break;
                case Col_ActualEndDate: rec.actualEndDate = value.toDateTime();
                    break;
                case Col_ApprovalStatus: rec.approvalStatus = value.toString();
                    break;
                case Col_ApprovalDate: rec.approvalDate = value.toDateTime();
                    break;
                case Col_ApproverID: rec.approverID = value.toInt();
                    break;
                case Col_ApproverName: rec.approverName = value.toString();
                    break;
                default: break;
            }
            emit dataChanged(index, index);
            return true;
        }
        return false;
    }

    Qt::ItemFlags BookingDataModel::flags(const QModelIndex &index) const {
        if (!index.isValid())
            return {};
        Qt::ItemFlags f = QAbstractTableModel::flags(index);
        if (index.column() != Col_Id)
            f |= Qt::ItemIsEditable;
        return f;
    }

    void BookingDataModel::fetchData() {
        beginResetModel();
        m_records = data::Booking::loadBookingFullRecords();
        endResetModel();
    }
    //增加返回记录函数
    QList<data::Booking::fullBookingRecord> BookingDataModel::getBookingData() const {
        return m_records;
    }
} // namespace dataModel

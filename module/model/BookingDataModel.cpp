//
// Created by gouzuang on 25-7-7.
//

#include <Qt>
#include "BookingDataModel.h"

#include "EquipmentDataModel.h"

namespace dataModel {


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
        if (role != Qt::EditRole || !index.isValid() || index.column() != Col_ApprovalStatus) {
            return false;
        }
        auto &rec = m_records[index.row()];
        QString newStatus = value.toString();
        if (rec.approvalStatus == newStatus) {
            return false;
        }
        bool success = false;
        if (newStatus == "同意") {
            success = data::Booking::processApprovalTransaction(rec.id,rec.equipmentId,rec.userId,approvalId);
            rec.approvalStatus = newStatus;
            emit approvalStatusChanged();
            emit dataChanged(index, index);
        }
        else {
            service::DatabaseManager db(data::Booking::path);
            success = data::Booking::updateBookingOnstatus(db,rec.id,rec.approvalStatus,approvalId);
            if (success) {
                rec.approvalStatus = newStatus;
                rec.approverID = approvalId;
                rec.approverName = approverName;
                rec.approvalDate = QDateTime::currentDateTime();
                emit approvalStatusChanged();
                emit dataChanged(index, index.siblingAtColumn(Col_ApproverName));
            }
        }
        return success;
    }

    Qt::ItemFlags BookingDataModel::flags(const QModelIndex &index) const {
        if (!index.isValid())
            return {};
        Qt::ItemFlags f = QAbstractTableModel::flags(index);
        if (index.column() != Col_Id)
            f |= Qt::ItemIsEditable;
        return f;
    }

    void BookingDataModel::setCurrentUserId(int id,const QString& name) {
        approvalId = id;
        approverName = name;
    }

    void BookingDataModel::fetchData() {
        beginResetModel();
        m_records = data::Booking::loadBookingFullRecords();
        QMap<int,QString> usersMap = data::UserControl::UserInfo::loadUsersMap();
        QMap<int,QString> groupsMap = data::UserControl::UserInfo::loadGroupsMap();
        for (auto & rec : m_records) {
            if (usersMap.contains(rec.userId)) {
                rec.userName = usersMap.value(rec.userId);
            }
            if (usersMap.contains(rec.approverID) && rec.approverID > 0) {
                rec.approverName = usersMap.value(rec.approverID);
            }
            if (groupsMap.contains(rec.userId)) {
                rec.userGroup = groupsMap.value(rec.userId);
            }
        }
        endResetModel();
    }
} // namespace dataModel

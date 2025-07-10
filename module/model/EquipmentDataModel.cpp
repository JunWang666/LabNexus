//
// Created by Nana7mi_ on 25-7-8.
//

#include "EquipmentDataModel.h"

namespace dataModel{


    EquipmentDataModel::EquipmentDataModel(QObject*parent) : QAbstractTableModel(parent) {
        fetchData();
    }

    int EquipmentDataModel::rowCount(const QModelIndex &index) const {
        Q_UNUSED(index);
        return myEquipmentRecord.count();
    }

    int EquipmentDataModel::columnCount(const QModelIndex &index) const {
        Q_UNUSED(index);
        return Col_Count;
    }

    QVariant EquipmentDataModel::data(const QModelIndex &index, int role) const {
        if (!index.isValid() || index.row() >= myEquipmentRecord.count() || index.row() < 0) {
            return QVariant();
        }
        const auto &record = myEquipmentRecord.at(index.row());
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            switch (index.column()) {
                case Col_ID:     return record.id;
                case Col_Name:   return record.name;
                case Col_Type:   return record.type;
                case Col_Status: return record.status;
                case Col_InDate: return record.inDate;
                case Col_Classroom: return record.class_id; // 新增：所在教室
                default:         return QVariant();
            }
        }
        return QVariant();
    }

    QVariant EquipmentDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
        if (role != Qt::DisplayRole)
            return QVariant();
        if (orientation == Qt::Horizontal) {
            static QStringList headers = {
                "设备ID","设备名称","设备类型","当前状态","入库时间","所在教室"
            };
            if (section >= 0 && section < headers.size())
                return headers.at(section);
        }
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    bool EquipmentDataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
        if (!index.isValid() || role != Qt::EditRole)
            return false;
        auto &record = myEquipmentRecord[index.row()];
        int col = index.column();
        switch (col) {//只有状态和名字可以更改
            case Col_Name: record.name = value.toString(); break;
            case Col_Status: record.status = value.toString(); break;
            default: return false;
        }
        emit dataChanged(index, index);
        return true;
    }

    Qt::ItemFlags EquipmentDataModel::flags(const QModelIndex &index) const {
        if (!index.isValid())
            return Qt::NoItemFlags;
        Qt::ItemFlags defaultItemFlags = QAbstractItemModel::flags(index);//获取默认标签
        if (index.column() == Col_Name || index.column() == Col_Status) {//只有状态和名字可以更改
            defaultItemFlags |= Qt::ItemIsEditable;
        }
        return defaultItemFlags;
    }

    void EquipmentDataModel::fetchData() {
        beginResetModel();
        myEquipmentRecord = data::Equipment::loadFullEquipmentRecords();
        endResetModel();
    }
} // dataModel

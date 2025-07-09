//
// Created by Nana7mi_ on 25-7-8.
//

#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include <QAbstractTableModel>
#include "module/data/data_EquipmentManage.h"
namespace dataModel {

class EquipmentDataModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        enum Column {
            Col_ID,
            Col_Name,
            Col_Type,
            Col_RentId,
            Col_Status,
            Col_InDate,
            Col_Count // 列的总数
        };
    public:
        explicit EquipmentDataModel(QObject *parent = nullptr);

        // --- 必须重写的核心函数 (用于读取和展示) ---
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &index = QModelIndex(), int role = Qt::DisplayRole) const override;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        // --- 为了让表格可编辑，需要重写的函数 ---
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;

        Qt::ItemFlags flags(const QModelIndex &index) const override;

    public slots:
        //槽函数，用于拉取数据
        void fetchData();
    private:
        //存储从数据库拉取的数据
        QList<data::Equipment::fullEquipmentRecord> myEquipmentRecord;
};

} // dataModel

#endif //EQUIPMENT_H

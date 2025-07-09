//
// Created by gouzuang on 25-7-7.
//

#ifndef DATA_EQUIPMENTMANAGE_H
#define DATA_EQUIPMENTMANAGE_H

#include "pch.h"
#include <QString>
#include <QDateTime>
#include <QList>
namespace data::Equipment {
    inline static QString path = "./equipment.db";

    /**
     * @brief 删除设备管理数据库。
     *
     * 该函数删除设备管理相关的数据库表和数据。
     */
    void dropDB();

    /**
     * @brief 构建设备管理数据库。
     *
     * 该函数创建设备管理相关的数据库表和数据。
     */
    void buildDB();

    /*************/
    struct fullEquipmentRecord {
        int id;
        QString type;
        QString name;
        int class_id;
        QDateTime inDate;
        int rentId;
        QString status;
    };

    QList<fullEquipmentRecord> loadFullEquipmentRecords();
    bool updateEquipmentOnReturn(int id);
    /*************/


    namespace EquipmentClass {
        /**
         * @brief 创建设备表。
         *
         * 该函数检查名为"equipment_class"的表是否存在。如果不存在，则创建一个新表。
         */
        void createEquipmentClassTable();

    }

    namespace EquipmentInstnace {
        /**
         * @brief 创建设备表。
         *
         * 该函数检查名为"equipment_instance"的表是否存在。如果不存在，则创建一个新表。
         */
        void createEquipmentInstanceTable();
    }

}
#endif //DATA_EQUIPMENTMANAGE_H

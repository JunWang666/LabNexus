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
    inline QString path = service::Path::equipment();

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

    struct fullEquipmentRecord {
        int id;
        QString type;
        QString name;
        int class_id;
        QDateTime inDate;
        int rentId;
        QString status;
    };

    struct EquipmentIds {
        int id;
        int class_id;
    };


    QList<fullEquipmentRecord> loadFullEquipmentRecords();

    bool updateEquipmentOnReturn(int id);
    bool updateEquipmentOnStatus(int id,const QString & status);
    bool updateEquipmentOnLoan(int equipmentId, int borrowerId);
    QStringList getEquipmentOnStatus(const QString & status);
    EquipmentIds getEquipmentIdsByName(const QString & devName);


    namespace EquipmentClass {
        struct EquipmentClassRecord {
            int id;
            QString name;
            QString description;
            QDateTime created_at;
            int total_amount;
            int usable_amount;
            int alarm_amount;
        };

        /**
         * @brief 创建设备表。
         *
         * 该函数检查名为"equipment_class"的表是否存在。如果不存在，则创建一个新表。
         */
        void createEquipmentClassTable();

        QString getEquNameFromEquClassId(int classId);

        int getEquCountFromEquClassId(int classId);

        int getEquClassPageCount(int pageSize = 10);
        int getEquClassCount();
        QList<EquipmentClassRecord> getEquClassList(int page = 1, int pageSize = 10);

        /**
         * @brief 通过ID获取单个设备类别的详细信息。
         * @param classId 设备类别的ID。
         * @return 返回包含该类别信息的 EquipmentClassRecord 结构体。如果未找到，返回的id为-1。
         */
        EquipmentClassRecord getEquipmentClassById(int classId);

        /**
         * @brief 更新指定的设备类别信息。
         * @param record 包含要更新的设备类别信息的结构体。函数会通过record.id来定位要更新的行。
         * @return 如果更新成功，返回 true；否则返回 false。
         */
        bool updateEquipmentClass(const EquipmentClassRecord &record);

        /**
         * @brief 根据实例表重新统计并更新指定设备类别的总数和可用数量。
         * @param classId 要重新统计的设备类别的ID。
         * @return 如果统计和更新都成功，返回 true；否则返回 false。
         */
        bool recalculateClassCounts(int classId);

        /**
         * @brief 向数据库中添加一个新的设备类别。
         * @param record 包含新类别信息的结构体 (id 会被忽略)。
         * @return 如果创建成功，返回新记录的ID；如果失败，返回 -1。
         */
        int addEquipmentClass(const EquipmentClassRecord &record);
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

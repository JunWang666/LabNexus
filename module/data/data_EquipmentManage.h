//
// Created by gouzuang on 25-7-7.
//

#ifndef DATA_EQUIPMENTMANAGE_H
#define DATA_EQUIPMENTMANAGE_H
namespace data::Equipment {
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

    namespace EquipmentClass {
        /**
         * @brief 创建设备表。
         *
         * 该函数检查名为"equipment_class"的表是否存在。如果不存在，则创建一个新表。
         */
        void createEquipmentClassTable();

    }

}
#endif //DATA_EQUIPMENTMANAGE_H

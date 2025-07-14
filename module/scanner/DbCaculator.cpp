//
// Created by gouzuang on 25-7-14.
//

#include "DbCaculator.h"

#include "module/data/data_EquipmentManage.h"

namespace bot::DbCaculator {
    void reCaculate() {
        auto equipmentClasses = data::Equipment::EquipmentClass::getEquClassList();
        for (const auto &classItem : equipmentClasses) {
            int classId = classItem.id;
            data::Equipment::EquipmentClass::recalculateClassCounts(classId);
        }}
}

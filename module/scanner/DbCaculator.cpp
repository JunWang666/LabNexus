//
// Created by gouzuang on 25-7-14.
//

#include "DbCaculator.h"

#include "module/data/data_EquipmentManage.h"

namespace bot::DbCaculator {
    void reCaculate() {
        data::Equipment::EquipmentClass::recalculateClassCounts();
    }
}

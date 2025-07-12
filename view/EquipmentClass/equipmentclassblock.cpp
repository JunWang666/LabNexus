//
// Created by gouzuang on 25-7-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EquipmentClassBlock.h" resolved

#include "equipmentclassblock.h"
#include "ui_EquipmentClassBlock.h"
#include "module/data/data_EquipmentManage.h"

namespace view::EquipmentClass {
    EquipmentClassBlock::EquipmentClassBlock(QWidget *parent) : QWidget(parent), ui(new Ui::EquipmentClassBlock) {
        ui->setupUi(this);
    }

    EquipmentClassBlock::EquipmentClassBlock(const data::Equipment::EquipmentClass::EquipmentClassRecord &record,
                                             QWidget *parent): ui(new Ui::EquipmentClassBlock) {
        ui->setupUi(this);
        // 1. 设置仪器名称
        ui->label_name->setText(record.name);

        // 2. 设置仪器说明
        ui->label_discription->setText(record.description);

        // 3. 拼接总数、可用数量和预警数量字符串
        QString dataText = QStringLiteral("总数: %1 | 可用: %2 | 预警阈值: %3")
                               .arg(record.total_amount)
                               .arg(record.usable_amount)
                               .arg(record.alarm_amount);

        // 4. 设置数据标签
        ui->label_data->setText(dataText);
    }

    EquipmentClassBlock::~EquipmentClassBlock() {
        delete ui;
    }
} // view::EquipmentClass

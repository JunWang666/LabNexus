//
// Created by gouzuang on 25-7-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EquipmentInstanceButton.h" resolved

#include "equipmentinstancebutton.h"
#include "ui_EquipmentInstanceButton.h"

namespace view::EquipmentClass {
EquipmentInstanceButton::EquipmentInstanceButton(QWidget *parent) :
    QWidget(parent), ui(new Ui::EquipmentInstanceButton) {
    ui->setupUi(this);
}

EquipmentInstanceButton::~EquipmentInstanceButton() {
    delete ui;
}
} // view::EquipmentClass

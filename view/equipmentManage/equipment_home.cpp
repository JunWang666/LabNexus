//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_equipment_home.h" resolved

#include "equipment_home.h"
#include "ui_equipment_home.h"

namespace view::equipment {
equipment_home::equipment_home(QWidget *parent) :
    QWidget(parent), ui(new Ui::equipment_home) {
    ui->setupUi(this);
}

equipment_home::~equipment_home() {
    delete ui;
}
} // view::equipment

//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_homepage.h" resolved

#include "equipment_homepage.h"
#include "ui_homepage.h"

namespace view::equipment {
    equipment_homepage::equipment_homepage(QWidget *parent) : QWidget(parent), ui(new Ui::equipment_homepage) {
        ui->setupUi(this);
    }

    equipment_homepage::~equipment_homepage() {
        delete ui;
    }
} // view::equipment

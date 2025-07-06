//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_homepage.h" resolved

#include "maintenance_homepage.h"
#include "ui_homepage.h"

namespace view::maintenance {
    maintenance_homepage::maintenance_homepage(QWidget *parent) : QWidget(parent), ui(new Ui::maintenance_homepage) {
        ui->setupUi(this);
    }

    maintenance_homepage::~maintenance_homepage() {
        delete ui;
    }
} // view::maintenance

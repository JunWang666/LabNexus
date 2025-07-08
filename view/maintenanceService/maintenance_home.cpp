//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_maintenance_home.h" resolved

#include "pch.h"
#include "maintenance_home.h"
#include "ui_maintenance_home.h"

namespace view::maintenance {
maintenance_home::maintenance_home(QWidget *parent) :
    QWidget(parent), ui(new Ui::maintenance_home) {
    ui->setupUi(this);
}

maintenance_home::~maintenance_home() {
    delete ui;
}
} // view::maintenance

//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_homepage.h" resolved

#include "inventory_homepage.h"
#include "ui_homepage.h"

namespace view::inventory {
    inventory_homepage::inventory_homepage(QWidget *parent) : QWidget(parent), ui(new Ui::inventory_homepage) {
        ui->setupUi(this);
    }

    inventory_homepage::~inventory_homepage() {
        delete ui;
    }
} // view::inventory

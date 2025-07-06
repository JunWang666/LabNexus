//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_inventory_home.h" resolved

#include "inventory_home.h"
#include "ui_inventory_home.h"

namespace view::inventory {
inventory_home::inventory_home(QWidget *parent) :
    QWidget(parent), ui(new Ui::inventory_home) {
    ui->setupUi(this);
}

inventory_home::~inventory_home() {
    delete ui;
}
} // view::inventory

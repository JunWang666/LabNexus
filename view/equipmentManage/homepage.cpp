//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_homepage.h" resolved

#include "homepage.h"
#include "ui_homepage.h"

namespace view::equipment {
    homepage::homepage(QWidget *parent) : QWidget(parent), ui(new Ui::homepage) {
        ui->setupUi(this);
    }

    homepage::~homepage() {
        delete ui;
    }
} // view::equipment

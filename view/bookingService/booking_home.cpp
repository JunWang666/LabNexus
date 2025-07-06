//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_homepage.h" resolved

#include "booking_home.h"
#include "ui_homepage.h"

namespace view::booking {
    booking_home::booking_home(QWidget *parent) : QWidget(parent), ui(new Ui::booking_home) {
        ui->setupUi(this);
    }

    booking_home::~booking_home() {
        delete ui;
    }
} // view::booking

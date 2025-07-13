//
// Created by gouzuang on 25-7-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RegisterNewUser.h" resolved

#include "registernewuser.h"
#include "ui_RegisterNewUser.h"

namespace view::RegisterCenter {
    RegisterNewUser::RegisterNewUser(QWidget *parent) : QWidget(parent), ui(new Ui::RegisterNewUser) {
        ui->setupUi(this);
        service::style::setMica(this);
    }

    RegisterNewUser::~RegisterNewUser() {
        delete ui;
    }
} // view::RegisterCenter

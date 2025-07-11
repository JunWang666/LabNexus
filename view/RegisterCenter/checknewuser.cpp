//
// Created by gouzuang on 25-7-11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CheckNewUser.h" resolved

#include "checknewuser.h"
#include "ui_CheckNewUser.h"

namespace view::RegisterCenter {
CheckNewUser::CheckNewUser(QWidget *parent) :
    QWidget(parent), ui(new Ui::CheckNewUser) {
    ui->setupUi(this);
}

CheckNewUser::~CheckNewUser() {
    delete ui;
}
} // view::RegisterCenter

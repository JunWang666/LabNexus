//
// Created by gouzuang on 25-7-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChangePasswordAdmin.h" resolved

#include "changepasswordadmin.h"
#include "ui_ChangePasswordAdmin.h"

namespace view::RegisterCenter {
ChangePasswordAdmin::ChangePasswordAdmin(QWidget *parent) :
    QWidget(parent), ui(new Ui::ChangePasswordAdmin) {
    ui->setupUi(this);
    ui->message->setVisible(false);
}

ChangePasswordAdmin::~ChangePasswordAdmin() {
    delete ui;
}

void ChangePasswordAdmin::on_UpdateButton_clicked() {
    auto id_number = ui->id_number->text();
    auto new_password = ui->new_password->text();
    if (id_number.isEmpty() || new_password.isEmpty()) {
        log(LogLevel::ERR) << "错误：身份证号或新密码不能为空！";
        show_message("错误：身份证号或新密码不能为空！");
        return;
    }
    auto userId = data::UserControl::UserInfo::getIdByIdNumber(id_number);
    if (userId.has_value()) {
        auto id = userId.value();
        data::UserControl::Login::updateUserPassword(id, new_password);
    }else {
        return;
    }
}

void ChangePasswordAdmin::show_message(QString message, int timeout) {
    ui->message->setText(message);
    ui->message->setVisible(true);
    QTimer::singleShot(timeout, this, [this]() {
        ui->message->setVisible(false);
    });
}
} // view::RegisterCenter

//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Rent.h" resolved

#include "rent.h"
#include "ui_Rent.h"

namespace view::Order {
Rent::Rent(QWidget *parent) :
    QWidget(parent), ui(new Ui::Rent) {
    ui->setupUi(this);
}

Rent::~Rent() {
    delete ui;
}
//点击发送申请按钮显示对话框
void Rent::on_btnsend_clicked()
{
    sendRent = new SendRent(this);
    sendRent->show();
}
//点击查看按钮查看申请
void Rent::on_btncheck_clicked()
{
    checkApply = new Apply(this);
    checkApply->show();
}

} // view::Order

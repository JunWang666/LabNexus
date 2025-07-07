//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RentTeacher.h" resolved

#include "rentteacher.h"
#include "ui_RentTeacher.h"

namespace view::Order {
RentTeacher::RentTeacher(QWidget *parent) :
    QWidget(parent), ui(new Ui::RentTeacher) {
    ui->setupUi(this);
}

RentTeacher::~RentTeacher() {
    delete ui;
}

void RentTeacher::on_btnsend_clicked()
{
    sendRent = new SendRent(this);
    sendRent->show();
}

void RentTeacher::on_btncheck_clicked()
{
    apply = new Apply(this);
    apply->show();
}


} // view::Order

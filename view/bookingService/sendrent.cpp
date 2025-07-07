//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SendRent.h" resolved

#include "sendrent.h"
#include "ui_SendRent.h"

namespace view::Order {
SendRent::SendRent(QWidget *parent) :
    QDialog(parent), ui(new Ui::SendRent) {
    ui->setupUi(this);
    /*
    * ui->nameLineEdit->setText("");//设置姓名
    * ui->LineEditNo->setText("");设置学号
    * ui->nameLineEdit->setReadOnly(true);//设置只读
    * ui->LineEditNo->setReadOnly(true);//设置呢只读
     */
    //均设置为当前时间
    ui->rentDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->returnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

SendRent::SendRent(const QString &name,QWidget *parent) :
    QDialog(parent)
    ,ui(new Ui::SendRent) {
    ui->setupUi(this);
    /*
   * ui->nameLineEdit->setText("");//设置姓名
   * ui->LineEditNo->setText("");设置学号
   * ui->nameLineEdit->setReadOnly(true);//设置只读
   * ui->LineEditNo->setReadOnly(true);//设置呢只读
    */
    //点击table后默认设置设备且不可更改
    ui->deviceComboBpx->setCurrentText(name);
    ui->deviceComboBpx->setEnabled(true);
    //均设置为当前时间
    ui->rentDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->returnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

SendRent::~SendRent() {
    delete ui;
}
} // view::Order

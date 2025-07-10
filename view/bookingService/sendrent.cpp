//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SendRent.h" resolved

#include "sendrent.h"
#include "ui_SendRent.h"
#include "module/data/data_Booking.h"
#include "module/data/data_EquipmentManage.h"

namespace view::Order {
    SendRent::SendRent(const QString &name, const QString &id,
                       QWidget *parent) : QDialog(parent), ui(new Ui::SendRent) {
        ui->setupUi(this);
        this->setAttribute(Qt::WA_DeleteOnClose, false);
        ui->nameLineEdit->setText(name); //设置姓名
        ui->LineEditNo->setText(id); //设置学号
        ui->nameLineEdit->setReadOnly(true); //设置只读
        ui->LineEditNo->setReadOnly(true); //设置呢只读
        //均设置为当前时间
        ui->rentDateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui->returnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
        //获取类型
        ui->deviceComboBpx->addItems(data::Equipment::getEquipmentOnStatus("可用"));
    }

    SendRent::SendRent(const QString &name, const QString &id, const QString &devName,
                       QWidget *parent) : QDialog(parent)
                                          , ui(new Ui::SendRent) {
        ui->setupUi(this);
        ui->nameLineEdit->setText(name); //设置姓名
        ui->LineEditNo->setText(id); //设置学号
        ui->nameLineEdit->setReadOnly(true); //设置只读
        ui->LineEditNo->setReadOnly(true); //设置呢只读
        ui->deviceComboBpx->addItems(data::Equipment::getEquipmentOnStatus("可用"));
        //点击table后默认设置设备且不可更改

        ui->deviceComboBpx->setCurrentText(devName);
        ui->deviceComboBpx->setEnabled(true);
        //均设置为当前时间
        ui->rentDateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui->returnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    }

    SendRent::~SendRent() {
        delete ui;
    }

    void SendRent::on_btnSend_clicked() {
        int id = ui->LineEditNo->text().toInt();
        QString name = ui->nameLineEdit->text();
        QString devName = ui->deviceComboBpx->currentText();
        int number = ui->numberSpinBox->text().toInt();
        QDateTime startTime = ui->rentDateTimeEdit->dateTime();
        QDateTime endTime = ui->returnDateTimeEdit->dateTime();
        QString subject = "申请";
        QString content = tr("%1申请%2%3").arg(name).arg(devName).arg(number);

        data::mail::send_mail(data::UserControl::currentUserId, 1, subject, content, {});
    }
} // view::Order

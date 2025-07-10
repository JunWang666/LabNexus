//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SendRent.h" resolved

#include "sendrent.h"
#include "ui_SendRent.h"
#include "module/data/data_Booking.h"
#include "module/data/data_EquipmentManage.h"

namespace view::Order {
    SendRent::SendRent(const QString &name, const QString &id, const QString& devName, int equipmentId, int equipmentClassId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendRent),
    m_equipmentId(equipmentId),         // 存储传入的ID
    m_equipmentClassId(equipmentClassId) // 存储传入的ClassID
    {
        ui->setupUi(this);
        this->setAttribute(Qt::WA_DeleteOnClose,false);
        ui->nameLineEdit->setText(name);//设置姓名
        ui->LineEditNo->setText(id);//设置学号
        ui->nameLineEdit->setReadOnly(true);//设置只读
        ui->LineEditNo->setReadOnly(true);//设置呢只读
        //均设置为当前时间
        ui->rentDateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui->returnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
        // 填充下拉框，并将预选的设备设为当前项，然后禁用下拉框
        ui->deviceComboBpx->addItems(data::Equipment::getEquipmentOnStatus("可用"));
        ui->deviceComboBpx->setCurrentText(devName);
        ui->deviceComboBpx->setEnabled(false); // 因为设备已指定，所以禁用
    }

SendRent::SendRent(const QString &name,const QString & id,QWidget *parent) :
    QDialog(parent), ui(new Ui::SendRent) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,false);
    ui->nameLineEdit->setText(name);//设置姓名
    ui->LineEditNo->setText(id);//设置学号
    ui->nameLineEdit->setReadOnly(true);//设置只读
    ui->LineEditNo->setReadOnly(true);//设置呢只读
    //均设置为当前时间
    ui->rentDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->returnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    //获取类型
    ui->deviceComboBpx->addItems(data::Equipment::getEquipmentOnStatus("可用"));
}


SendRent::~SendRent() {
    delete ui;
}

void SendRent::on_btnSend_clicked() {
        int userId = ui->LineEditNo->text().toInt();
        QDateTime requestStartTime = ui->rentDateTimeEdit->dateTime();
        QDateTime requestEndTime = ui->returnDateTimeEdit->dateTime();
        int finalEquipmentId = m_equipmentId;
        int finalEquipmentClassId = m_equipmentClassId;

        // 数据校验
        if (requestStartTime >= requestEndTime) {
            QMessageBox::warning(this, "提示", "归还时间必须晚于借出时间。");
            return;
        }

        // 如果 m_equipmentId 为 -1, 说明是通过“无预选”构造函数创建的
        // 我们需要从 ComboBox 中获取信息并查询ID
        if (finalEquipmentId == -1) {
            QString devName = ui->deviceComboBpx->currentText();
            if (devName.isEmpty()) {
                QMessageBox::warning(this, "提示", "请选择一个设备。");
                return;
            }
            // 调用我们之前设计的辅助函数
            data::Equipment::EquipmentIds ids = data::Equipment::getEquipmentIdsByName(devName);
            if (ids.id == -1) {
                QMessageBox::critical(this, "错误", "无法在数据库中找到所选设备的信息。");
                return;
            }
            finalEquipmentId = ids.id;
            finalEquipmentClassId = ids.class_id;
        }

        // 准备其他参数
        QDateTime createDate = QDateTime::currentDateTime();
        QString approvalStatus = "待审批";
        int approverId = 0; // 同样，审批人ID暂时设为默认值

        // 调用数据库插入函数
        bool success = data::Booking::createBookingARecord(
            userId, createDate, finalEquipmentClassId, finalEquipmentId,
            requestStartTime, requestEndTime, approvalStatus, approverId
        );

        // 用户反馈
        if (success) {
            QMessageBox::information(this, "成功", "您的申请已成功发送！");
            this->accept();
        } else {
            QMessageBox::critical(this, "失败", "发送申请时发生错误，请稍后重试。");
        }
}
} // view::Order

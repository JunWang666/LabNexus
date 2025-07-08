//
// Created by gouzuang on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_teacherHomepage.h" resolved

#include "teacherhomepage.h"
#include "ui_teacherHomepage.h"
#include "../loginPage/loginpage.h"
#include "service/logger/logger.h"
#include <QMessageBox>

namespace view::homepage {
    teacherHomepage::teacherHomepage(const QString &name, const QString &ID, QWidget *parent) : QWidget(parent),
        ui(new Ui::teacherHomepage), T_name(name), T_ID(ID) {
        ui->setupUi(this);
        setupUI();
        rentTeacher = new view::Order::RentTeacher(name,ID,this);
    }

    teacherHomepage::~teacherHomepage() {
        delete ui;
    }

    void teacherHomepage::setupUI() {
        // 设置欢迎信息
        ui->welcomeLabel->setText(
            QString(
                "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">你好！%1</span></p></body></html>")
            .arg(T_name));

        // 设置公告文本框为只读
        ui->announcementTextEdit->setReadOnly(true);
        ui->announcementTextEdit->setText(
            "欢迎使用实验室设备管理系统！\n作为教师用户，您可以：\n1. 借用和归还实验设备\n2. 查看借用历史记录\n3. 报修设备故障\n4. 审批学生的借用申请\n5. 修改个人信息");

        service::log() << "教师主页初始化完成 - 用户: " << T_name << " (ID: " << T_ID << ")";
    }

    void teacherHomepage::on_borrowEquipmentButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了器材借用按钮";

        // TODO: 打开设备借用页面
        // auto *borrowPage = new view::booking::booking_home();
        // borrowPage->show();
        rentTeacher->show();
        // QMessageBox::information(this, "器材借用",
        //                          QString("器材借用功能开发中...\n用户: %1\nID: %2").arg(T_name).arg(T_ID));
    }

    void teacherHomepage::on_returnEquipmentButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了器材归还按钮";

        // TODO: 打开设备归还页面
        QMessageBox::information(this, "器材归还",
                                 QString("器材归还功能开发中...\n用户: %1\nID: %2").arg(T_name).arg(T_ID));
    }

    void teacherHomepage::on_borrowHistoryButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了借用日志按钮";

        // TODO: 打开借用历史页面
        rentTeacher->on_btnCheck_clicked();
        QMessageBox::information(this, "我的借用日志",
                                 QString("借用日志功能开发中...\n用户: %1\nID: %2").arg(T_name).arg(T_ID));
    }

    void teacherHomepage::on_reportMaintenanceButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了器材报修按钮";

        // TODO: 打开设备报修页面
        // auto *maintenancePage = new view::maintenance::maintenance_home();
        // maintenancePage->show();
        rentTeacher->show();
        QMessageBox::information(this, "器材报修",
                                 QString("器材报修功能开发中...\n用户: %1\nID: %2").arg(T_name).arg(T_ID));
    }

    void teacherHomepage::on_editProfileButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了修改个人信息按钮";

        // TODO: 打开修改密码/个人信息页面
        QMessageBox::information(this, "修改个人信息",
                                 QString("个人信息修改功能开发中...\n用户: %1\nID: %2\n\n您可以修改密码和其他个人信息。").arg(T_name).arg(T_ID));
    }

    void teacherHomepage::on_approvalButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了审批按钮";

        // TODO: 打开审批页面，显示待审批的学生借用申请
        rentTeacher->show();
        QMessageBox::information(this, "审批",
                                 QString("审批功能开发中...\n用户: %1\nID: %2\n\n在这里您可以审批学生的设备借用申请。").arg(T_name).arg(T_ID));
    }

    void teacherHomepage::on_logoutButton_clicked() {
        service::log() << "教师 " << T_name << " 退出登录";

        int ret = QMessageBox::question(this, "退出登录",
                                        QString("确定要退出登录吗？\n用户: %1").arg(T_name),
                                        QMessageBox::Yes | QMessageBox::No);

        if (ret == QMessageBox::Yes) {
            // 返回登录页面
            auto *loginPage = new view::login::loginPage();
            loginPage->setAttribute(Qt::WA_DeleteOnClose);
            loginPage->show();

            this->close(); // 关闭当前教师主页
        }
    }

    void teacherHomepage::on_messageButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了消息按钮";

        // TODO: 打开消息中心页面
        QMessageBox::information(this, "消息中心",
                                 QString("消息中心功能开发中...\n用户: %1\nID: %2\n\n在这里您可以查看系统通知和消息。").arg(T_name).arg(T_ID));
    }
} // view::homepage

//
// Created by gouzuang on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_studentHomepage.h" resolved

#include "pch.h"
#include "studenthomepage.h"
#include "ui_studentHomepage.h"
#include "../loginPage/loginpage.h"
#include "service/logger/logger.h"
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include "view/loginPage/changepassword.h"
#include "view/messageCenter/messagewindow.h"

namespace view::homepage {
    studentHomepage::studentHomepage(const QString &name, const QString &ID, QWidget *parent) : QWidget(parent),
        ui(new Ui::studentHomepage), S_name(name), S_ID(ID) {
        ui->setupUi(this);
        ui->frame_3->hide();
        ui->frame_4->hide();
        setupUI();
        rent = nullptr;
        this->setWindowFlag(Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

    }

    studentHomepage::~studentHomepage() {
        delete ui;
        delete rent;
    }

    void studentHomepage::setupUI() {
        // 设置欢迎信息
        ui->welcomeLabel->setText(
            QString(
                "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">你好！%1</span></p></body></html>")
            .arg(S_name));

        // 设置公告文本框为只读


        service::log() << "学生主页初始化完成 - 用户: " << S_name << " (ID: " << S_ID << ")";
    }

    void studentHomepage::on_borrowEquipmentButton_clicked() {
        service::log() << "学生 " << S_name << " 点击了器材借用按钮";

        // TODO: 打开设备借用页面
        // auto *borrowPage = new view::booking::booking_home();
        // borrowPage->show();
        if (!rent) {
            rent = new view::Order::Rent(S_name,S_ID);
        }
        rent->setIndex(Order::Rent::Col_Rent);
        // QMessageBox::information(this, "器材借用",
        //                          QString("器材借用功能开发中...\n用户: %1\nID: %2\n\n在这里您可以申请借用实验设备，申请需要等待教师审批。").arg(S_name).arg(
        //                              S_ID));
    }

    void studentHomepage::on_returnEquipmentButton_clicked() {
        service::log() << "学生 " << S_name << " 点击了器材归还按钮";

        // TODO: 打开设备归还页面
        if (!rent) {
            rent = new view::Order::Rent(S_name,S_ID);
        }
        rent->setIndex(Order::Rent::Col_Return);
        // QMessageBox::information(this, "器材归还",
        //                          QString("器材归还功能开发中...\n用户: %1\nID: %2\n\n在这里您可以归还已借用的设备。").arg(S_name).arg(S_ID));
    }

    void studentHomepage::on_borrowHistoryButton_clicked() {
        service::log() << "学生 " << S_name << " 点击了借用日志按钮";

        // TODO: 打开借用历史页面
        if (!rent) {
            rent = new view::Order::Rent(S_name,S_ID);
        }
        rent->on_btnCheck_clicked();
        // QMessageBox::information(this, "我的借用日志",
        //                          QString("借用日志功能开发中...\n用户: %1\nID: %2\n\n在这里您可以查看自己的设备借用历史记录。").arg(S_name).arg(S_ID));
    }

    void studentHomepage::on_reportMaintenanceButton_clicked() {
        service::log() << "学生 " << S_name << " 点击了器材报修按钮";

        // TODO: 打开设备报修页面
        // auto *maintenancePage = new view::maintenance::maintenance_home();
        // maintenancePage->show();
        if (!rent) {
            rent = new view::Order::Rent(S_name,S_ID);
        }
        rent->setIndex(Order::Rent::Col_Repair);
        // QMessageBox::information(this, "器材报修",
        //                          QString("器材报修功能开发中...\n用户: %1\nID: %2\n\n在这里您可以报告设备故障或损坏。").arg(S_name).arg(S_ID));
    }

    void studentHomepage::on_editProfileButton_clicked() {
        service::log() << "学生 " << S_name << " 点击了修改个人信息按钮";
        loginPage::changePassword *change_pass_page=new loginPage::changePassword();
        change_pass_page->setData(S_ID);
        change_pass_page->show();
        // TODO: 打开修改密码/个人信息页面

    }

    void studentHomepage::on_logoutButton_clicked() {
        service::log() << "学生 " << S_name << " 退出登录";



            // 返回登录页面
            auto *loginPage = new view::login::loginPage();
            loginPage->setAttribute(Qt::WA_DeleteOnClose);
            loginPage->show();

            this->close(); // 关闭当前学生主页

    }

    void studentHomepage::on_messageButton_clicked() {
        service::log() << "学生 " << S_name << " 点击了消息按钮";

        view::messageCenter::MessageWindow *messageWindow = new view::messageCenter::MessageWindow();
        service::MutiWindow::manager().addWindow(messageWindow);
        messageWindow->show();

    }
    void studentHomepage::on_Button_clicked()
    {
        ui->frame_3->show();
        ui->frame_4->hide();
    }
    void studentHomepage::on_Button2_clicked()
    {
        ui->frame_3->hide();
        ui->frame_4->show();
    }
    void studentHomepage::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
            mouseOffset = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }

    void studentHomepage::mouseMoveEvent(QMouseEvent *event)
    {
        if (event->buttons() & Qt::LeftButton)
            move(event->globalPosition().toPoint() - mouseOffset);
    }

    void studentHomepage::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
    }



} // view::homepage

//
// Created by gouzuang on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_teacherHomepage.h" resolved

#include "pch.h"
#include "teacherhomepage.h"
#include "ui_teacherHomepage.h"
#include "../loginPage/loginpage.h"
#include "service/logger/logger.h"
#include <QMessageBox>
#include <QMouseEvent>
#include "view/loginPage/changepassword.h"
namespace view::homepage {
    teacherHomepage::teacherHomepage(const QString &name, const QString &ID, QWidget *parent) : QWidget(parent),
        ui(new Ui::teacherHomepage), T_name(name), T_ID(ID) {
        ui->setupUi(this);
        setupUI();
        ui->frame_3->hide();
        ui->frame_4->hide();
        this->setWindowFlag(Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

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


        service::log() << "教师主页初始化完成 - 用户: " << T_name << " (ID: " << T_ID << ")";
    }

    void teacherHomepage::on_borrowEquipmentButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了器材借用按钮";

        // TODO: 打开设备借用页面
        // auto *borrowPage = new view::booking::booking_home();
        // borrowPage->show();


    }

    void teacherHomepage::on_returnEquipmentButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了器材归还按钮";

        // TODO: 打开设备归还页面

    }

    void teacherHomepage::on_borrowHistoryButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了借用日志按钮";

        // TODO: 打开借用历史页面

    }

    void teacherHomepage::on_reportMaintenanceButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了器材报修按钮";

        // TODO: 打开设备报修页面
        // auto *maintenancePage = new view::maintenance::maintenance_home();
        // maintenancePage->show();

    }

    void teacherHomepage::on_editProfileButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了修改个人信息按钮";

        // TODO: 打开修改密码/个人信息页面
        loginPage::changePassword *change_pass_page=new loginPage::changePassword();
        change_pass_page->setData(T_ID);
        change_pass_page->show();
    }

    void teacherHomepage::on_approvalButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了审批按钮";

        // TODO: 打开审批页面，显示待审批的学生借用申请

    }

    void teacherHomepage::on_logoutButton_clicked() {
        service::log() << "教师 " << T_name << " 退出登录";



            // 返回登录页面
            auto *loginPage = new view::login::loginPage();
            loginPage->setAttribute(Qt::WA_DeleteOnClose);
            loginPage->show();

            this->close(); // 关闭当前教师主页

    }

    void teacherHomepage::on_messageButton_clicked() {
        service::log() << "教师 " << T_name << " 点击了消息按钮";

        // TODO: 打开消息中心页面

    }

    void teacherHomepage::on_Button_clicked()
    {
        ui->frame_3->show();
        ui->frame_4->hide();
    }
    void teacherHomepage::on_Button2_clicked()
    {
        ui->frame_3->hide();
        ui->frame_4->show();
    }
    void teacherHomepage::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
            mouseOffset = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }

    void teacherHomepage::mouseMoveEvent(QMouseEvent *event)
    {
        if (event->buttons() & Qt::LeftButton)
            move(event->globalPosition().toPoint() - mouseOffset);
    }

    void teacherHomepage::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
    }
} // view::homepage

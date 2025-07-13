//
// Created by gouzuang on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_administratorHomepage.h" resolved

#include "pch.h"
#include "administratorhomepage.h"
#include "ui_administratorHomepage.h"
#include "../loginPage/loginpage.h"
#include "service/logger/logger.h"
#include <QMessageBox>
#include <QMouseEvent>

#include "view/EquipmentClass/equipmentclassblock.h"
#include "view/EquipmentClass/equipmentclassmanagehomepage.h"
#include "view/equipmentManage/equipment_home.h"
#include "view/messageCenter/messagewindow.h"
#include "view/RegisterCenter/checknewuser.h"

namespace view::homepage {
    administratorHomepage::administratorHomepage(const QString &name, const QString &ID,
                                                 QWidget *parent) : QWidget(parent), ui(new Ui::administratorHomepage),
                                                                    A_name(name), A_ID(ID) {
        ui->setupUi(this);
        setupUI();
        ui->frame_3->hide();
        ui->frame_4->hide();
        orderCheck = nullptr;
        this->setWindowFlag(Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

    }

    administratorHomepage::~administratorHomepage() {
        delete ui;
        delete orderCheck;
    }

    void administratorHomepage::setupUI() {
        // 设置欢迎信息
        ui->welcomeLabel->setText(
            QString(
                "<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">你好！%1</span></p></body></html>")
            .arg(A_name));

        // 设置公告文本框为只读


        service::log() << "管理员主页初始化完成 - 用户: " << A_name << " (ID: " << A_ID << ")";
    }

    void administratorHomepage::on_equipmentManageButton_clicked() {
        service::log() << "管理员 " << A_name << " 点击了器材管理按钮";
        view::equipment::equipment_home *c=new view::equipment::equipment_home();
        service::MutiWindow::manager().addWindow(c);
        c->show();
        // TODO: 打开设备管理页面
        // auto *equipmentPage = new view::equipment::equipment_home();
        // equipmentPage->show();
    }

    void administratorHomepage::on_approvalButton_clicked() {
        service::log() << "管理员 " << A_name << " 点击了审批按钮";

        // TODO: 打开审批页面
        if (!orderCheck) {
            orderCheck = new Order::ManagerCheck();
        }
        orderCheck->show();
        // QMessageBox::information(this, "审批管理",
        //                          QString(
        //                              "审批管理功能开发中...\n用户: %1\nID: %2\n\n在这里您可以：\n• 审批所有用户的借用申请\n• 查看审批历史记录\n• 设置审批流程和规则\n• 批量处理审批事务")
        //                          .arg(A_name).arg(A_ID));
    }

    void administratorHomepage::on_userManageButton_clicked() {
        service::log() << "管理员 " << A_name << " 点击了用户管理按钮";

        // TODO: 打开用户管理页面
        QMessageBox::information(this, "用户管理",
                                 QString(
                                     "用户管理功能开发中...\n用户: %1\nID: %2\n\n在这里您可以：\n• 创建、编辑、删除用户账户\n• 管理用户权限和角色\n• 重置用户密码\n• 查看用户活动日志")
                                 .arg(A_name).arg(A_ID));
    }

    void administratorHomepage::on_systemManageButton_clicked() {
        service::log() << "管理员 " << A_name << " 点击了系统管理按钮";

        // TODO: 打开系统管理页面
        QMessageBox::information(this, "系统管理",
                                 QString(
                                     "系统管理功能开发中...\n用户: %1\nID: %2\n\n在这里您可以：\n• 系统配置和参数设置\n• 数据库备份和恢复\n• 查看系统日志和性能\n• 系统安全和维护")
                                 .arg(A_name).arg(A_ID));
    }

    void administratorHomepage::on_editProfileButton_2_clicked() {
        service::log() << "管理员 " << A_name << " 点击了修改个人信息按钮";

        // TODO: 打开修改密码/个人信息页面
        // auto *changePasswordPage = new view::loginPage::changePassword(A_name, A_ID);
        // changePasswordPage->show();

        QMessageBox::information(this, "修改个人信息",
                                 QString("个人信息修改功能开发中...\n用户: %1\nID: %2\n\n您可以修改密码和其他个人信息。").arg(A_name).arg(A_ID));
    }

    void administratorHomepage::on_logoutButton_clicked() {
        service::log() << "管理员 " << A_name << " 退出登录";



            // 返回登录页面
            auto *loginPage = new view::login::loginPage();
            loginPage->setAttribute(Qt::WA_DeleteOnClose);
            loginPage->show();

            this->close(); // 关闭当前管理员主页

    }

    void administratorHomepage::on_messageButton_clicked() {
        service::log() << "管理员 " << A_name << " 点击了消息按钮";

        view::messageCenter::MessageWindow *messageWindow = new view::messageCenter::MessageWindow();
        service::MutiWindow::manager().addWindow(messageWindow);
        messageWindow->show();
    }
    void administratorHomepage::on_Button_clicked()
    {
        ui->frame_3->show();
        ui->frame_4->hide();
    }
    void administratorHomepage::on_Button2_clicked()
    {
        ui->frame_3->hide();
        ui->frame_4->show();
    }

    void administratorHomepage::on_EquipmentClassButton_clicked() {
        view::EquipmentClass::EquipmentClassManageHomepage* equipmentClassPage = new view::EquipmentClass::EquipmentClassManageHomepage();
        service::MutiWindow::manager().addWindow(equipmentClassPage);
        equipmentClassPage->show();
    }

    void administratorHomepage::on_RegisterControlButton_clicked() {
        view::RegisterCenter::CheckNewUser* checkNewUserPage = new view::RegisterCenter::CheckNewUser();
        service::MutiWindow::manager().addWindow(checkNewUserPage);
        checkNewUserPage->show();
    }

    void administratorHomepage::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
            mouseOffset = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }

    void administratorHomepage::mouseMoveEvent(QMouseEvent *event)
    {
        if (event->buttons() & Qt::LeftButton)
            move(event->globalPosition().toPoint() - mouseOffset);
    }

    void administratorHomepage::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
    }


} // view::homepage

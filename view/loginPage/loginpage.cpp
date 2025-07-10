//
// Created by gouzuang on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_loginPage.h" resolved

#include "pch.h"
#include "loginpage.h"
#include "ui_loginPage.h"
#include "registerpage.h"
#include "changepassword.h"
#include "../homepage/teacherhomepage.h"
#include "../homepage/studenthomepage.h"
#include "../homepage/administratorhomepage.h"
#include "module/data/data_UserControl.h"
#include "service/logger/logger.h"
#include <QMessageBox>
#include <QString>
#include <QMouseEvent>


namespace view::login {
    loginPage::loginPage(QWidget *parent) : QWidget(parent), ui(new Ui::loginPage) {
        ui->setupUi(this);

        // 设置密码输入框为密码模式
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        ui->passwordLineEdit->setMaxLength(16);


        this->setWindowFlag(Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);
        ui->lineEdit->setText("无效的用户名");
        ui->lineEdit_2->setText("密码错误或用户不存在");
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
    }

    loginPage::~loginPage() {
        delete ui;
    }

    int loginPage::confirm_user(QString &ID_c, QString &password_c) {
        auto result = data::UserControl::Login::isUserPasswordValid(ID_c, password_c);
        if (result) {
            int userId = result.value();

            // 获取用户名
            auto nameResult = data::UserControl::UserInfo::getUserNameById(userId);
            if (nameResult) {
                name = nameResult.value();
            } else {
                name = "未知用户";
            }

            return userId;
        } else {
            return 0;
        }
    }

    void loginPage::on_loginButton_clicked() {
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ID = ui->userIdLineEdit->text();
        QString password = ui->passwordLineEdit->text();

        if (ID.isEmpty() || password.isEmpty()) {
            ui->lineEdit->show();
            return;
        }

        int userId = confirm_user(ID, password);
        data::UserControl::currentUserId = userId;
        auto userGroup = data::UserControl::permission::getUserInWhichGroupIdList(userId);
        if (userId > 0 && !userGroup.isEmpty()) {
            if (userGroup.contains(1)) {
                // 学生用户 - 打开学生主页
                auto *studentHome = new view::homepage::studentHomepage(name, ID);
                studentHome->show();
                this->close(); // 关闭登录页面
            } else if (userGroup.contains(2)) {
                // 老师用户 - 打开教师主页
                auto *teacherHome = new view::homepage::teacherHomepage(name, ID);
                teacherHome->show();
                this->close(); // 关闭登录页面
            } else if (userGroup.contains(3)) {
                // 管理员用户 - 打开管理员主页
                auto *adminHome = new view::homepage::administratorHomepage(name, ID);
                adminHome->show();
                this->close(); // 关闭登录页面
            }
        } else {
            service::log() << "用户 " << ID << " 登录失败";
            ui->lineEdit_2->show();
        }
    }

    void loginPage::on_registerButton_clicked() {
        // 打开注册页面
        registerPage *regPage = new registerPage();
        regPage->show();
    }

    void loginPage::on_changePasswordButton_clicked() {
        // 打开修改密码页面
        view::loginPage::changePassword *changePwdPage = new view::loginPage::changePassword();

        // 连接信号，修改密码成功后返回登录页面
        connect(changePwdPage, &view::loginPage::changePassword::backToLogin, this, &loginPage::showLoginPage);

        changePwdPage->show();
    }

    void loginPage::showLoginPage() {
        // 显示登录页面（在修改密码后调用）
        this->show();
        this->raise();
        this->activateWindow();
    }
    void loginPage::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
            mouseOffset = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }

    void loginPage::mouseMoveEvent(QMouseEvent *event)
    {
        if (event->buttons() & Qt::LeftButton)
            move(event->globalPosition().toPoint() - mouseOffset);
    }

    void loginPage::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
    }
} // view::login

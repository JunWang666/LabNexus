//
// Created by gouzuang on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_loginPage.h" resolved

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

namespace view::login {
    loginPage::loginPage(QWidget *parent) : QWidget(parent), ui(new Ui::loginPage) {
        ui->setupUi(this);

        // 设置密码输入框为密码模式
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        ui->passwordLineEdit->setMaxLength(16);

        // 设置公告文本框为只读
        ui->announcementTextEdit->setReadOnly(true);
        ui->announcementTextEdit->setText("欢迎使用实验室设备管理系统！\n请输入您的用户ID和密码进行登录。");

        // 初始化用户数据库
        data::UserControl::buildDB();

        // Qt的自动连接机制会自动连接on_*_clicked()槽函数，无需手动连接
    }

    loginPage::~loginPage() {
        delete ui;
    }

    int loginPage::confirm_user(QString &ID_c, QString &password_c) {
        // 使用LabNexus的用户验证系统
        auto result = data::UserControl::Login::isUserPasswordValid(ID_c, password_c);
        if (result) {
            int userId = result.value();

            // 获取用户所在的组来确定用户类型
            QString userGroup = data::UserControl::permission::getUserInWhichGroup(userId);

            if (userGroup == "Student") {
                user = 1; // 学生
            } else if (userGroup == "Teacher") {
                user = 2; // 老师
            } else if (userGroup == "Administrator") {
                user = 4; // 管理员
            } else {
                user = 0; // 无效用户
            }

            // 获取用户名
            auto nameResult = data::UserControl::UserInfo::getUserNameById(userId);
            if (nameResult) {
                name = nameResult.value();
            } else {
                name = "未知用户";
            }

            return userId;
        } else {
            user = 0;
            return 0;
        }
    }

    void loginPage::on_loginButton_clicked() {
        ID = ui->userIdLineEdit->text();
        QString password = ui->passwordLineEdit->text();

        if (ID.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "错误", "请输入用户ID和密码");
            return;
        }

        int userId = confirm_user(ID, password);
        if (userId > 0 && user > 0) {
            service::log() << "用户 " << ID << " 登录成功，类型: " << user;

            if (user == 1) {
                // 学生用户 - 打开学生主页
                QMessageBox::information(this, "登录成功", QString("欢迎，%1同学！").arg(name));
                auto *studentHome = new view::homepage::studentHomepage(name, ID);
                studentHome->show();
                this->close(); // 关闭登录页面
            } else if (user == 2) {
                // 老师用户 - 打开教师主页
                QMessageBox::information(this, "登录成功", QString("欢迎，%1老师！").arg(name));
                auto *teacherHome = new view::homepage::teacherHomepage(name, ID);
                teacherHome->show();
                this->close(); // 关闭登录页面
            } else if (user >= 4) {
                // 管理员用户 - 打开管理员主页
                QMessageBox::information(this, "登录成功", QString("欢迎，管理员%1！").arg(name));
                auto *adminHome = new view::homepage::administratorHomepage(name, ID);
                adminHome->show();
                this->close(); // 关闭登录页面
            }

            // 暂时不关闭登录窗口，等实现了其他页面后再添加
            // this->close();
        } else {
            service::log() << "用户 " << ID << " 登录失败";
            QMessageBox::warning(this, "错误", "无效的用户ID或密码");
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
} // view::login

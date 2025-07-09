#include "pch.h"
//
// Created by gouzuang on 25-7-7.
//

#include "changepassword.h"
#include "ui_changePassword.h"
#include "module/data/data_UserControl.h"
#include "service/logger/logger.h"
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>
namespace view::loginPage {
    changePassword::changePassword(QWidget *parent) : QWidget(parent), ui(new Ui::changePassword) {
        ui->setupUi(this);

        // 设置窗口属性
        setAttribute(Qt::WA_DeleteOnClose);
        setWindowTitle("修改密码 - LabNexus");

        // 设置密码输入框
        setupPasswordFields();

        // 连接信号槽
        setupConnections();

        ui->lineEdit->setText("用户ID不能为空");
        ui->lineEdit_2->setText("用户名不能为空");
        ui->lineEdit_3->setText("用户不存在或密码错误");
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->passwordHintLabel->setText("密码必须是8~16位且必须包含数字，大小写字母");

        this->setWindowFlag(Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

        // 记录日志
        service::log() << "修改密码页面已打开";
    }

    changePassword::~changePassword() {
        service::log() << "修改密码页面已关闭";
        delete ui;
    }


    void changePassword::setData(const QString &IDD)
    {
        ui->lineEdit_userId->setText(IDD);
        ui->lineEdit_userId->setReadOnly(true);
    }
    void changePassword::setupPasswordFields() {
        // 设置密码输入框为密码模式
        ui->lineEdit_oldPassword->setEchoMode(QLineEdit::Password);
        ui->lineEdit_newPassword->setEchoMode(QLineEdit::Password);
        ui->lineEdit_confirmPassword->setEchoMode(QLineEdit::Password);

        // 设置占位符文本

    }

    void changePassword::setupConnections() {
        // 连接确认按钮
        connect(ui->pushButton_confirm, &QPushButton::clicked, this, &changePassword::onConfirmChangePassword);

        // 连接取消按钮
        connect(ui->pushButton_cancel, &QPushButton::clicked, this, &changePassword::onCancel);
    }

    bool changePassword::isPasswordValid(const QString &password) {
        // 检查密码长度
        if (password.length() < 8 || password.length() > 16) {
            return false;
        }

        // 检查是否包含小写字母
        bool hasLower = false;
        // 检查是否包含大写字母
        bool hasUpper = false;
        // 检查是否包含数字
        bool hasDigit = false;

        for (const QChar &ch: password) {
            if (ch.isLower()) hasLower = true;
            if (ch.isUpper()) hasUpper = true;
            if (ch.isDigit()) hasDigit = true;
            if (hasLower && hasUpper && hasDigit) break;
        }

        return hasLower && hasUpper && hasDigit;
    }

    bool changePassword::validateUserCredentials(const QString &id, const QString &name, const QString &oldPassword) {
        // 使用LabNexus的用户验证系统
        change_name=0;
        auto result = data::UserControl::Login::isUserPasswordValid(id, oldPassword);
        if (!result.has_value()) {
            return false;
        }

        int userId = result.value();

        // 验证用户名是否匹配
        auto nameResult = data::UserControl::UserInfo::getUserNameById(userId);
        if (!nameResult.has_value() || nameResult.value() != name) {
            change_name=1;
        }

        return true;
    }

    bool changePassword::updateUserPassword(const QString &id, const QString &newPassword) {
        // 使用LabNexus的用户数据库更新密码
        auto userResult = data::UserControl::Login::isUserPasswordValid(id, prePassword); // 使用旧密码验证获取用户ID
        if (!userResult.has_value()) {
            return false;
        }

        int userId = userResult.value();
        auto updateResult = data::UserControl::Login::updateUserPassword(userResult.value(), newPassword);
        if(change_name) data::UserControl::UserInfo::changeUserName(userResult.value(),userName);
        return updateResult.has_value() && updateResult.value();
    }

    void changePassword::onConfirmChangePassword() {

        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->passwordHintLabel->setText("");
        userId = ui->lineEdit_userId->text().trimmed();
        userName = ui->lineEdit_userName->text().trimmed();
        prePassword = ui->lineEdit_oldPassword->text();
        newPassword = ui->lineEdit_newPassword->text();
        QString confirmPassword = ui->lineEdit_confirmPassword->text();

        // 检查是否填写完整
        if (userId.isEmpty() || userName.isEmpty() || prePassword.isEmpty() ||
            newPassword.isEmpty() || confirmPassword.isEmpty()) {
            ui->passwordHintLabel->setText("信息填写不完整");
            if(userId.isEmpty()) ui->lineEdit->show();
            if(userName.isEmpty()) ui->lineEdit_2->show();
            service::log() << "修改密码失败：信息不完整，用户ID: " << userId;
            return;
        }

        // 检查两次密码输入是否一致
        if (newPassword != confirmPassword) {
            ui->passwordHintLabel->setText("两次密码输入不一致");
            service::log() << "修改密码失败：两次密码输入不一致，用户ID: " << userId;
            return;
        }

        // 检查新密码格式
        if (!isPasswordValid(newPassword)) {
            ui->passwordHintLabel->setText("密码必须是8~16位且必须包含数字，大小写字母");
            service::log() << "修改密码失败：新密码格式不符合要求，用户ID: " << userId;
            return;
        }

        // 验证用户凭据
        if (!validateUserCredentials(userId, userName, prePassword)) {
            ui->lineEdit_3->show();
            service::log() << "修改密码失败：用户凭据验证失败，用户ID: " << userId << " 姓名: " << userName;
            return;
        }

        // 更新密码
        if (updateUserPassword(userId, newPassword)) {
            service::log() << "密码修改成功，用户ID: " << userId << " 姓名: " << userName;

            // 发送信号返回登录页面
            emit backToLogin();
            this->close();
        } else {
            ui->passwordHintLabel->setText("密码修改失败，请稍后重试");
            service::log() << "密码修改失败：数据库更新失败，用户ID: " << userId;
        }
    }

    void changePassword::onCancel() {
        service::log() << "用户取消修改密码操作";

        // 发送信号返回登录页面
        emit backToLogin();
        this->close();
    }

    void changePassword::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
            mouseOffset = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }

    void changePassword::mouseMoveEvent(QMouseEvent *event)
    {
        if (event->buttons() & Qt::LeftButton)
            move(event->globalPosition().toPoint() - mouseOffset);
    }

    void changePassword::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
    }



} // view::loginPage

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

        // 记录日志
        service::log() << "修改密码页面已打开";
    }

    changePassword::~changePassword() {
        service::log() << "修改密码页面已关闭";
        delete ui;
    }

    void changePassword::setupPasswordFields() {
        // 设置密码输入框为密码模式
        ui->lineEdit_oldPassword->setEchoMode(QLineEdit::Password);
        ui->lineEdit_newPassword->setEchoMode(QLineEdit::Password);
        ui->lineEdit_confirmPassword->setEchoMode(QLineEdit::Password);

        // 设置占位符文本
        ui->lineEdit_userId->setPlaceholderText("请输入用户ID");
        ui->lineEdit_userName->setPlaceholderText("请输入用户姓名");
        ui->lineEdit_oldPassword->setPlaceholderText("请输入旧密码");
        ui->lineEdit_newPassword->setPlaceholderText("请输入新密码(8-16位，包含大小写字母和数字)");
        ui->lineEdit_confirmPassword->setPlaceholderText("请再次输入新密码");
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
        auto result = data::UserControl::Login::isUserPasswordValid(id, oldPassword);
        if (!result.has_value()) {
            return false;
        }

        int userId = result.value();

        // 验证用户名是否匹配
        auto nameResult = data::UserControl::UserInfo::getUserNameById(userId);
        if (!nameResult.has_value() || nameResult.value() != name) {
            return false;
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
        auto updateResult = data::UserControl::Login::updateUserPassword(userId, newPassword);
        return updateResult.has_value() && updateResult.value();
    }

    void changePassword::onConfirmChangePassword() {
        // 获取输入内容
        userId = ui->lineEdit_userId->text().trimmed();
        userName = ui->lineEdit_userName->text().trimmed();
        prePassword = ui->lineEdit_oldPassword->text();
        newPassword = ui->lineEdit_newPassword->text();
        QString confirmPassword = ui->lineEdit_confirmPassword->text();

        // 检查是否填写完整
        if (userId.isEmpty() || userName.isEmpty() || prePassword.isEmpty() ||
            newPassword.isEmpty() || confirmPassword.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "请填写完整信息");
            service::log() << "修改密码失败：信息不完整，用户ID: " << userId;
            return;
        }

        // 检查两次密码输入是否一致
        if (newPassword != confirmPassword) {
            QMessageBox::warning(this, "输入错误", "两次密码输入不一致");
            service::log() << "修改密码失败：两次密码输入不一致，用户ID: " << userId;
            return;
        }

        // 检查新密码格式
        if (!isPasswordValid(newPassword)) {
            QMessageBox::warning(this, "密码格式错误", "新密码必须是8~16位且必须包含数字、大小写字母");
            service::log() << "修改密码失败：新密码格式不符合要求，用户ID: " << userId;
            return;
        }

        // 验证用户凭据
        if (!validateUserCredentials(userId, userName, prePassword)) {
            QMessageBox::warning(this, "验证失败", "用户ID、姓名或旧密码错误");
            service::log() << "修改密码失败：用户凭据验证失败，用户ID: " << userId << " 姓名: " << userName;
            return;
        }

        // 更新密码
        if (updateUserPassword(userId, newPassword)) {
            QMessageBox::information(this, "成功", "密码修改成功！");
            service::log() << "密码修改成功，用户ID: " << userId << " 姓名: " << userName;

            // 发送信号返回登录页面
            emit backToLogin();
            this->close();
        } else {
            QMessageBox::critical(this, "修改失败", "密码修改失败，请稍后重试");
            service::log() << "密码修改失败：数据库更新失败，用户ID: " << userId;
        }
    }

    void changePassword::onCancel() {
        service::log() << "用户取消修改密码操作";

        // 发送信号返回登录页面
        emit backToLogin();
        this->close();
    }
} // view::loginPage

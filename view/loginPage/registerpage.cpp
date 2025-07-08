//
// Created by gouzuang on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_registerPage.h" resolved

#include "pch.h"
#include "registerpage.h"
#include "ui_registerPage.h"

namespace view::login {
registerPage::registerPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::registerPage) {
    ui->setupUi(this);
    
    // 设置密码输入框为密码模式
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    
    // 初始化身份为0（未选择）
    identity = 0;
    
    // 连接信号槽
    connect(ui->submitButton, &QPushButton::clicked, this, &registerPage::on_submitButton_clicked);
    connect(ui->studentRadioButton, &QRadioButton::clicked, this, &registerPage::on_studentRadioButton_clicked);
    connect(ui->teacherRadioButton, &QRadioButton::clicked, this, &registerPage::on_teacherRadioButton_clicked);
}

registerPage::~registerPage() {
    delete ui;
}

bool registerPage::isPasswordValid(const QString &password) {
    // 检查密码长度
    if (password.length() < 8 || password.length() > 16) {
        return false;
    }
    
    // 检查是否包含小写字母、大写字母和数字
    bool hasLower = false;
    bool hasUpper = false;
    bool hasDigit = false;

    for (const QChar &ch : password) {
        if (ch.isLower()) hasLower = true;
        if (ch.isUpper()) hasUpper = true;
        if (ch.isDigit()) hasDigit = true;
        if (hasLower && hasUpper && hasDigit) break;
    }
    
    return hasLower && hasUpper && hasDigit;
}

void registerPage::on_submitButton_clicked() {
    // 检查是否填写完整信息
    if (ui->idLineEdit->text().isEmpty() || 
        ui->passwordLineEdit->text().isEmpty() || 
        ui->confirmPasswordLineEdit->text().isEmpty() || 
        ui->usernameLineEdit->text().isEmpty() || 
        identity == 0) {
        QMessageBox::warning(this, "警告", "请填写完整信息");
        return;
    }
    
    // 检查密码是否一致
    if (ui->passwordLineEdit->text() != ui->confirmPasswordLineEdit->text()) {
        QMessageBox::warning(this, "错误", "密码输入不一致");
        return;
    }
    
    password = ui->passwordLineEdit->text();
    
    // 验证密码强度
    if (!isPasswordValid(password)) {
        QMessageBox::warning(this, "密码强度不足", "密码必须是8~16位且必须包含数字，大小写字母");
        return;
    }
    
    // 获取输入信息
    name = ui->usernameLineEdit->text();
    ID = ui->idLineEdit->text();
    
    // 确定用户组
    QString userGroup;
    if (identity == 1) {
        userGroup = "Student";
    } else if (identity == 2) {
        userGroup = "Teacher";
    }
    
    // 使用LabNexus的数据库系统创建用户
    auto result = data::UserControl::Login::createNewUser(ID, name, password, userGroup);
    
    if (result) {
        service::log() << "新用户注册成功: " << ID << " (" << name << ") - " << userGroup;
        QMessageBox::information(this, "注册成功", 
            QString("申请成功！\n用户ID: %1\n用户名: %2\n身份: %3\n\n请等待管理员审核。")
            .arg(ID)
            .arg(name)
            .arg(identity == 1 ? "学生" : "老师"));
        this->close();
    } else {
        QString errorMsg;
        switch (result.error()) {
            case data::UserControl::UserControlError::UserAlreadyExists:
                errorMsg = "用户ID已存在，请使用其他ID";
                break;
            case data::UserControl::UserControlError::GroupNotFound:
                errorMsg = "用户组不存在";
                break;
            case data::UserControl::UserControlError::DatabaseError:
                errorMsg = "数据库错误";
                break;
            default:
                errorMsg = "注册失败，请稍后重试";
                break;
        }
        service::log() << "用户注册失败: " << ID << " - " << errorMsg;
        QMessageBox::warning(this, "注册失败", errorMsg);
    }
}

void registerPage::on_studentRadioButton_clicked() {
    identity = 1;
}

void registerPage::on_teacherRadioButton_clicked() {
    identity = 2;
}

} // view::login

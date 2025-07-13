//
// Created by gouzuang on 25-7-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RegisterNewUser.h" resolved

#include "registernewuser.h"
#include "ui_RegisterNewUser.h"
#include <QFileDialog>

namespace view::RegisterCenter {
    RegisterNewUser::RegisterNewUser(QWidget *parent) : QWidget(parent), ui(new Ui::RegisterNewUser) {
        ui->setupUi(this);
        service::style::setMica(this);
        auto groupMap = data::UserControl::permission::getAllGroup();
        ui->message->setVisible(false);
        ui->UserGroup->clear();

        for (auto it = groupMap.begin(); it != groupMap.end(); ++it) {
            QString groupName = it.key();
            int groupId = it.value();
            ui->UserGroup->addItem(groupName, groupId);  // 添加项并附带数据
        }
    }

    RegisterNewUser::~RegisterNewUser() {
        delete ui;
    }

    void RegisterNewUser::on_UpdateButton_clicked() {
        show_message("正在创建用户，请稍候...", 10000);
        QString text = ui->NewUser->toPlainText();
        QStringList idNumbers = text.split(QRegularExpression("[\\s，,\\n]+"), Qt::SplitBehaviorFlags::SkipEmptyParts);
        int selectedGroupId = ui->UserGroup->currentData().toInt();
        auto res = data::UserControl::Login::batchCreateNewUser(idNumbers, selectedGroupId);

        // 使用文件选择器选择保存目录
        QString filePath = QFileDialog::getSaveFileName(this, "选择保存文件", "", "CSV文件 (*.csv)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << "学工号,密码\n";
                for (auto it = res.begin(); it != res.end(); ++it) {
                    out << it.key() << "," << it.value() << "\n";
                }
                file.close();
            } else {
                show_message("无法打开文件保存结果", 5000);
            }
        }
    }

    void RegisterNewUser::checkVaild() {
        auto idNumbers = ui->NewUser->toPlainText().split(QRegularExpression("[\\s，,\\n]+"), Qt::SplitBehaviorFlags::SkipEmptyParts);
        bool allValid = true;
        int selectedGroupId = ui->UserGroup->currentData().toInt();
        auto canCreate = data::UserControl::Login::batchGetUserCanCreate(idNumbers,selectedGroupId);

        int failedCount = 0;
        QString Message;
        for (auto it = canCreate.begin(); it != canCreate.end(); ++it) {
            Message+= it.key() + it.value() +"\n";
            if (it.value() == "已存在且在组中") {
                failedCount++;
            }
        }
        int successCount = canCreate.size() - failedCount;
        ui->TotalNum->setNum(successCount);
        ui->TotalNum_2->setNum(static_cast<int>(idNumbers.count()));
        ui->textBrowser->setText(Message);
    }

    void RegisterNewUser::on_NewUser_textChanged() {
        checkVaild();
    }

    void RegisterNewUser::on_UserGroup_currentTextChanged(const QString &arg1) {
        checkVaild();
    }

    void RegisterNewUser::show_message(QString message, int timeout) {
        ui->message->setText(message);
        ui->message->setVisible(true);
        QTimer::singleShot(timeout, this, [this]() {
            ui->message->setVisible(false);
        });
    }
} // view::RegisterCenter

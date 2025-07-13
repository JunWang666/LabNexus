//
// Created by gouzuang on 25-7-13.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CheckUserBlock.h" resolved

#include "checkuserblock.h"
#include "ui_CheckUserBlock.h"

namespace view::RegisterCenter {
    CheckUserBlock::CheckUserBlock(int userId, QWidget *parent): QWidget(parent), ui(new Ui::CheckUserBlock),
                                                                 userId(userId), acceptButton("接受", this),
                                                                 rejectButton("拒绝", this),
                                                                 label_status("状态: ", this) {
        ui->setupUi(this);
        acceptButton.setVisible(false);
        rejectButton.setVisible(false);
        label_status.setVisible(false);
        setButtonStyle();

        auto name = data::UserControl::UserInfo::getUserNameById(userId);
        if (name.has_value()) {
            ui->label_name->setText(name.value());
        } else {
            ui->label_name->setText("未设置名称的用户");
        }
        ui->label_id->setText(QString::number(userId));
        ui->label_group->setText(data::UserControl::permission::getUserInWhichGroup(userId));

        if (data::UserControl::check::getUserStatus(userId) == "Unchecked") {
            addCheckButton();
        } else {
            addStatusString();
        }

        connect(&acceptButton, &QPushButton::clicked, this, &CheckUserBlock::AcceptCheck);
    }

    CheckUserBlock::~CheckUserBlock() {
        delete ui;
    }

    void CheckUserBlock::addCheckButton() {
        ui->StatusFrame->layout()->addWidget(&acceptButton);
        ui->StatusFrame->layout()->addWidget(&rejectButton);
        acceptButton.setVisible(true);
        rejectButton.setVisible(true);
    }

    void CheckUserBlock::addStatusString(QString status) {
        ui->StatusFrame->layout()->addWidget(&label_status);
        label_status.setText(status);
        label_status.setVisible(true);
    }

    void CheckUserBlock::addStatusString() {
        auto status = data::UserControl::check::getUserStatus(userId);
        ui->StatusFrame->layout()->addWidget(&label_status);
        if (status == "Unchecked") {
            label_status.setText("未审核");
        } else if (status == "Accepted") {
            label_status.setText("已通过");
        } else if (status == "AllRight") {
            label_status.setText("一切正常");
        } else {
            label_status.setText("未知状态");
        }
        label_status.setVisible(true);
    }

    void CheckUserBlock::setButtonStyle() {
        QString styleSheet1 = R"(
            QPushButton {
                background-color: #0078D4;
                color: white;
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-weight: 600;
                font-size: 10pt;
            }
            QPushButton:hover { /* 鼠标悬停时 */
                background-color: #005FA8; /* 比默认蓝色稍深一点 */
            }

            QPushButton:pressed { /* 按钮按下时 */
                background-color: #004C8C; /* 更深的蓝色 */
                padding-top: 9px; /* 模拟下沉效果 */
                padding-bottom: 7px;
            }

        )";
        acceptButton.setStyleSheet(styleSheet1);
        QString styleSheet2 = R"(
            QPushButton {
                background-color: white;
                color: black;
                border: none;
                border-radius: 4px;
                padding: 8px 16px;
                font-weight: 600;
                font-size: 10pt;
            }

            QPushButton:hover { /* 鼠标悬停时 */
                background-color: #f0f0f0; /* 可以是更浅的颜色 */
            }

            QPushButton:pressed { /* 按钮按下时 */
                background-color: #e0e0e0; /* 可以是更深的颜色 */
                padding-top: 9px; /* 让按钮看起来稍微下沉 */
                padding-bottom: 7px;
            }
        )";
        rejectButton.setStyleSheet(styleSheet2);
    }

    void CheckUserBlock::AcceptCheck() {
        if (data::UserControl::check::allowUserRegister(userId)) {
            acceptButton.setVisible(false);
            rejectButton.setVisible(false);
            addStatusString("审批成功");
        } else {
            acceptButton.setText("通过失败");
        }
    }
} // view::RegisterCenter

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
        connect(&rejectButton, &QPushButton::clicked, this, &CheckUserBlock::RejectCheck);
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
            label_status.setText(
                "<img src=\":/SegoeFluentIcons/242518a4-f3f3-49d5-acae-97b284b3d5ed.png\" "
                "width=\"32\" height=\"32\" style=\"vertical-align: middle; margin-right: 5px;\">"
                "<span style=\"color: #f7a800; font-weight: bold;\">未审核</span>" // 警告黄
            );
        } else if (status == "AllRight") {
            label_status.setText(
                "<img src=\":/SegoeFluentIcons/af5b765c-4d2f-4146-a198-654d41b87d8f.png\" "
                "width=\"32\" height=\"32\" style=\"vertical-align: middle; margin-right: 5px;\">"
                "<span style=\"color: #28a745; font-weight: bold;\">一切正常</span>" // 成功绿
            );
        } else if (status == "Banned") {
            label_status.setText(
                "<img src=\":/SegoeFluentIcons/5e89ce73-9ce9-4f59-b157-ec1399f3548f.png\" "
                "width=\"32\" height=\"32\" style=\"vertical-align: middle; margin-right: 5px;\">"
                "<span style=\"color: #6c757d; font-weight: bold;\">被拉黑</span>" // 中性灰
            );
        } else if (status == "Rejected") {
            label_status.setText(
                "<img src=\":/SegoeFluentIcons/71f93e9a-7bd5-409b-918e-51a58ac863f7.png\" "
                "width=\"32\" height=\"32\" style=\"vertical-align: middle; margin-right: 5px;\">"
                "<span style=\"color: #dc3545; font-weight: bold;\">已拒绝</span>" // 危险红
            );
        } else {
            label_status.setText(
                "<img src=\":/SegoeFluentIcons/7adc4afd-5150-418b-b6e1-2a02b0d1f129.png\" " // 使用你提供的未知图标
                "width=\"32\" height=\"32\" style=\"vertical-align: middle; margin-right: 5px;\">"
                "<span style=\"color: #6c757d; font-weight: bold;\">未知状态</span>" // 保持中性灰
            );
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

    void CheckUserBlock::RejectCheck() {
        data::UserControl::check::rejectUserRegister(userId);
    }

    void CheckUserBlock::AcceptCheck() {
        if (data::UserControl::check::allowUserRegister(userId)) {
            acceptButton.setVisible(false);
            rejectButton.setVisible(false);
            addStatusString(
                "<img src=\":/SegoeFluentIcons/af5b765c-4d2f-4146-a198-654d41b87d8f.png\" " // 图片路径
                "width=\"16\" height=\"16\" style=\"vertical-align: middle; margin-right: 5px;\">" // 图片大小和对齐
                "<span style=\"color: #28a745; font-weight: bold; font-size: 11pt;\">审批成功</span>"
            );
        } else {
            addStatusString(
                "<img src=\":/SegoeFluentIcons/154ec457-cfd6-453c-bfe9-bd505c0b177d.png\" " // 图片路径
                "width=\"16\" height=\"16\" style=\"vertical-align: middle; margin-right: 5px;\">" // 图片大小和对齐
                "<span style=\"color: #dc3545; font-weight: bold; font-size: 11pt;\">审批失败</span>"
            );
        }
    }
} // view::RegisterCenter

//
// Created by gouzuang on 25-7-9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MessageDetailPage.h" resolved

#include "messagedetailpage.h"
#include "ui_MessageDetailPage.h"
#include "module/data/data_mail.h"

namespace view::messageCenter {
    MessageDetailPage::MessageDetailPage(QWidget *parent) : QWidget(parent), ui(new Ui::MessageDetailPage) {
        ui->setupUi(this);
    }

    MessageDetailPage::MessageDetailPage(int messageId, QWidget *parent) : QWidget(parent),
                                                                           ui(new Ui::MessageDetailPage) {
        ui->setupUi(this);
        auto mailData = data::mail::getMailById(messageId);
        ui->Subject->setText(mailData.subject);
        auto sender = data::UserControl::UserInfo::getUserNameById(mailData.sender_id);
        if (sender.has_value()) {
            ui->SenderName->setText(sender.value());
        } else {
            ui->SenderName->setText("未知发件人：" + QString::number(mailData.sender_id));
        }
        auto senderGroup = data::UserControl::permission::getUserInWhichGroup(mailData.sender_id);
        ui->SenderGroup->setText(senderGroup);
        ui->SendDate->setText(mailData.send_date.toString("yyyy-MM-dd hh:mm:ss"));
        ui->textBrowser->setPlainText(mailData.content);
    }

    MessageDetailPage::~MessageDetailPage() {
        delete ui;
    }
} // view::messageCenter

//
// Created by gouzuang on 25-7-8.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MessageWindow.h" resolved

#include "messagewindow.h"
#include "ui_MessageWindow.h"
#include "module/data/data_mail.h"
#include "module/data/data_UserControl.h"
#include <QVBoxLayout>
#include <QScrollArea>

namespace view::messageCenter {
    MessageWindow::MessageWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MessageWindow) {
        ui->setupUi(this);
        initializeUI();
        loadMailsFromDatabase(1);
    }

    MessageWindow::MessageWindow(int userId, QWidget *parent) : QWidget(parent), ui(new Ui::MessageWindow) {
        ui->setupUi(this);
        initializeUI();
        loadMailsFromDatabase(userId);
    }

    MessageWindow::~MessageWindow() {
        delete ui;
    }

    void MessageWindow::initializeUI() {
        auto *layout = new QVBoxLayout(ui->messageBoxAreaItem);
        layout->setSpacing(5); // 设置消息块之间的间距
        layout->setAlignment(Qt::AlignTop); // 从顶部开始排列
    }


    void MessageWindow::loadMailsFromDatabase(int userId, int page) {
        // 从数据库获取邮件数据
        auto mails = data::mail::getAllMails(userId, page, 5); // 获取第1页，每页50条邮件

        auto *layout = qobject_cast<QVBoxLayout *>(ui->messageBoxAreaItem->layout());
        if (!layout) {
            layout = new QVBoxLayout(ui->messageBoxAreaItem);
            layout->setSpacing(5);
            layout->setAlignment(Qt::AlignTop);
        }

        // 清除现有的消息块
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        // 为每封邮件创建消息块
        for (const auto &mail: mails) {
            MessageData data;
            data.id = QString::number(mail.id);
            data.title = mail.subject;
            data.sender = QString::number(mail.sender_id);
            data.content = mail.content;
            data.timestamp = mail.send_date.toString("yyyy-MM-dd hh:mm");

            auto *messageBlock = new MessageBlock(data);
            layout->addWidget(messageBlock);
        }
    }
}

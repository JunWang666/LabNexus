//
// Created by gouzuang on 25-7-8.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MessageBlock.h" resolved

#include "pch.h"
#include "messageblock.h"
#include "ui_MessageBlock.h"

namespace view::messageCenter {
    MessageBlock::MessageBlock(QWidget *parent) : QWidget(parent), ui(new Ui::MessageBlock) {
        ui->setupUi(this);
    }

    MessageBlock::MessageBlock(const QString &messageId, QWidget *parent) : QWidget(parent), ui(new Ui::MessageBlock) {
        ui->setupUi(this);

        MessageData messageData;
        init(messageData);
    }


    MessageBlock::MessageBlock(const MessageData &messageData, QWidget *parent) : QWidget(parent),
        ui(new Ui::MessageBlock) {
        ui->setupUi(this);
        init(messageData);
    }

    MessageBlock::~MessageBlock() {
        delete ui;
    }

    void MessageBlock::init(const MessageData &messageData) {
        ui->titleLabel->setText(messageData.title);
        ui->senderLabel->setText(messageData.sender);
        ui->contentLabel->setText(messageData.content);
        ui->timeLabel->setText(messageData.timestamp);
    }
}

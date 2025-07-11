//
// Created by gouzuang on 25-7-8.
//

#include "pch.h"
#include "messagewindow.h"
#include "ui_MessageWindow.h"
#include "module/data/data_mail.h"
#include "module/data/data_UserControl.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include "service/styleHelper/MicaHelper.h"

namespace view::messageCenter {
    MessageWindow::MessageWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MessageWindow) {
        ui->setupUi(this);
        initializeUI();
        service::style::setMica(this);

        // --- 分页逻辑初始化 ---
        m_currentUserId = data::UserControl::currentUserId;
        m_currentPage = 1;

        // 假设存在一个函数 getMailCount 来获取总邮件数
        // 如果没有，你需要自己在 data::mail 中实现它
        m_totalMails = data::mail::getMailCount(m_currentUserId);

        loadMailsFromDatabase(m_currentUserId, m_currentPage);
        ui->label_num->setNum(data::mail::getUnreadMailCount(m_currentUserId));
    }

    MessageWindow::MessageWindow(int userId, QWidget *parent) : QWidget(parent), ui(new Ui::MessageWindow) {
        ui->setupUi(this);
        initializeUI();

        // --- 分页逻辑初始化 ---
        m_currentUserId = userId;
        m_currentPage = 1;
        m_totalMails = data::mail::getMailCount(m_currentUserId);

        loadMailsFromDatabase(m_currentUserId, m_currentPage);
    }

    MessageWindow::~MessageWindow() {
        delete ui;
    }

    void MessageWindow::initializeUI() {
        auto *layout = new QVBoxLayout(ui->messageBoxAreaItem);
        layout->setSpacing(5);
        layout->setAlignment(Qt::AlignTop);
        this->resize(400, 1000);
    }

    void MessageWindow::loadMailsFromDatabase(int userId, int page) {
        // 从数据库获取邮件数据
        auto mails = data::mail::getAllMails(userId, page, m_mailsPerPage);

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
            auto senderOpt = data::UserControl::UserInfo::getUserNameById(mail.sender_id);
            if (senderOpt.has_value()) {
                data.sender = senderOpt.value();
            } else {
                data.sender = QStringLiteral("未知发件人：") + QString::number(mail.sender_id);
            }
            data.content = mail.content;
            data.timestamp = mail.send_date.toString("yyyy-MM-dd hh:mm");

            auto *messageBlock = new MessageBlock(data);
            layout->addWidget(messageBlock);
        }

        // 加载完数据后更新分页控件
        updatePaginationControls();
    }


    void MessageWindow::on_prevButton_clicked() {
        if (m_currentPage > 1) {
            m_currentPage--;
            loadMailsFromDatabase(m_currentUserId, m_currentPage);
        }
    }

    void MessageWindow::on_nextButton_clicked() {
        int totalPages = (m_totalMails + m_mailsPerPage - 1) / m_mailsPerPage;
        if (m_currentPage < totalPages) {
            m_currentPage++;
            loadMailsFromDatabase(m_currentUserId, m_currentPage);
        }
    }

    void MessageWindow::updatePaginationControls() {
        if (m_totalMails == 0) {
            ui->pageLabel->setText("0 / 0");
            ui->prevButton->setEnabled(false);
            ui->nextButton->setEnabled(false);
            return;
        }

        // 计算总页数，(a + b - 1) / b 是向上取整的整数除法技巧
        int totalPages = (m_totalMails + m_mailsPerPage - 1) / m_mailsPerPage;

        // 更新页码标签
        ui->pageLabel->setText(QString("%1 / %2").arg(m_currentPage).arg(totalPages));

        // 根据当前页码更新按钮状态
        ui->prevButton->setEnabled(m_currentPage > 1);
        ui->nextButton->setEnabled(m_currentPage < totalPages);
    }
}

//
// Created by gouzuang on 25-7-11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CheckNewUser.h" resolved

#include "checknewuser.h"

#include "checkuserblock.h"
#include "ui_CheckNewUser.h"

namespace view::RegisterCenter {
CheckNewUser::CheckNewUser(QWidget *parent) :
    QWidget(parent), ui(new Ui::CheckNewUser),
    m_currentPage(1),m_itemsPerPage(10),m_totalPages(){
    ui->setupUi(this);

    // --- 分页逻辑初始化 ---
    m_currentPage = 1;
    // 从数据层获取总页数
    m_totalPages = data::UserControl::check::getAllUserCount() / m_itemsPerPage + 1;

    loadDataFromDatabase(m_currentPage);
}

CheckNewUser::~CheckNewUser() {
    delete ui;
}

void CheckNewUser::loadDataFromDatabase(int page) {
    // 从数据库获取设备分类数据
    auto records = data::UserControl::check::getAllUserId(page, m_itemsPerPage);

    ui->scrollAreaWidgetContents->setUpdatesEnabled(false);

    auto *layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    if (!layout) {
        layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        layout->setSpacing(5);
        layout->setAlignment(Qt::AlignTop);
        layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    }

    // 清除现有的消息块
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // 为每个设备分类记录创建信息块
    for (const auto &record : records) {
        auto *block = new CheckUserBlock(); // 创建信息块实例
        layout->addWidget(block);
    }

    // 加载完数据后更新分页控件
    ui->scrollAreaWidgetContents->setUpdatesEnabled(true);
    updatePaginationControls();
    ui->label_num->setNum(data::UserControl::check::getAllUserCount());
    ui->label_num_2->setNum(data::UserControl::check::getUncheckedUserCount());
}

void CheckNewUser::updatePaginationControls() {
    if (m_totalPages == 0) {
        ui->pageLabel->setText("0 / 0");
        ui->prevButton->setEnabled(false);
        ui->nextButton->setEnabled(false);
        return;
    }
}

    void CheckNewUser::on_prevButton_clicked() {
        if (m_currentPage > 1) {
            m_currentPage--;
            loadDataFromDatabase(m_currentPage);
        }
    }

    void CheckNewUser::on_nextButton_clicked() {
        if (m_currentPage < m_totalPages) {
            m_currentPage++;
            loadDataFromDatabase(m_currentPage);
        }
    }

    void CheckNewUser::on_addButton_clicked() {
    }

    void CheckNewUser::on_refreshButton_clicked() {
        m_totalPages = data::UserControl::check::getAllUserCount() / m_itemsPerPage + 1;
        loadDataFromDatabase();
    }
}

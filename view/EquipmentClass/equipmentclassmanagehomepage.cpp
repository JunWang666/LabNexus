//
// Created by gouzuang on 25-7-12.
//

#include "equipmentclassmanagehomepage.h"
#include "ui_EquipmentClassManageHomepage.h"
#include "module/data/data_EquipmentManage.h"
#include <QVBoxLayout>

#include "equipmentclassblock.h"

namespace view::EquipmentClass {

EquipmentClassManageHomepage::EquipmentClassManageHomepage(QWidget *parent) :
    QWidget(parent), ui(new Ui::EquipmentClassManageHomepage) {
    ui->setupUi(this);
    initializeUI();
    service::style::setMica(this);

    // --- 分页逻辑初始化 ---
    m_currentPage = 1;
    // 从数据层获取总页数
    m_totalPages = data::Equipment::EquipmentClass::getEquClassPageCount(m_itemsPerPage);

    // 加载第一页数据
    loadEquipmentClasses(m_currentPage);
}

EquipmentClassManageHomepage::~EquipmentClassManageHomepage() {
    delete ui;
}

void EquipmentClassManageHomepage::initializeUI() {
    // 假设你的.ui文件中QScrollArea内的QWidget的objectName是scrollAreaWidgetContents
    // 这与MessageWindow中的messageBoxAreaItem是类似作用
    auto *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignTop);
}

void EquipmentClassManageHomepage::loadEquipmentClasses(int page) {
    // 从数据库获取设备分类数据
    auto records = data::Equipment::EquipmentClass::getEquClassList(page, m_itemsPerPage);

    ui->scrollAreaWidgetContents->setUpdatesEnabled(false);

    auto *layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    if (!layout) {
        // 如果布局不存在，则创建一个新的
        initializeUI();
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
        auto *block = new EquipmentClassBlock(record, this); // 创建信息块实例
        layout->addWidget(block);                            // 添加到布局
    }

    // 加载完数据后更新分页控件
    ui->scrollAreaWidgetContents->setUpdatesEnabled(true);
    updatePaginationControls();
    ui->label_num->setNum(data::Equipment::EquipmentClass::getEquClassCount());
}


void EquipmentClassManageHomepage::on_prevButton_clicked() {
    if (m_currentPage > 1) {
        m_currentPage--;
        loadEquipmentClasses(m_currentPage);
    }
}

void EquipmentClassManageHomepage::on_nextButton_clicked() {
    if (m_currentPage < m_totalPages) {
        m_currentPage++;
        loadEquipmentClasses(m_currentPage);
    }
}

void EquipmentClassManageHomepage::on_addButton_clicked() {
    view::EquipmentClass::EquipmentClassDetail *detail = new view::EquipmentClass::EquipmentClassDetail();
    service::MutiWindow::manager().addWindow(detail);
    detail->show();
}

void EquipmentClassManageHomepage::on_refreshButton_clicked() {
    loadEquipmentClasses();
}

void EquipmentClassManageHomepage::updatePaginationControls() {
    if (m_totalPages == 0) {
        ui->pageLabel->setText("0 / 0");
        ui->prevButton->setEnabled(false);
        ui->nextButton->setEnabled(false);
        return;
    }

    // 更新页码标签
    ui->pageLabel->setText(QString("%1 / %2").arg(m_currentPage).arg(m_totalPages));

    // 根据当前页码更新按钮状态
    ui->prevButton->setEnabled(m_currentPage > 1);
    ui->nextButton->setEnabled(m_currentPage < m_totalPages);
}

} // namespace view::EquipmentClass
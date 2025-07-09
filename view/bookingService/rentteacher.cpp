//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RentTeacher.h" resolved

#include "rentteacher.h"

#include <qscreen_platform.h>

#include "ui_RentTeacher.h"

namespace view::Order {
RentTeacher::RentTeacher(QWidget *parent) :
    QWidget(parent), ui(new Ui::RentTeacher) {
    ui->setupUi(this);
    setUpModel_device();
    setUpModel_request();
    loadData();
    ui->pageListWidget->addItem("申请");
    ui->pageListWidget->addItem("审批");
    connect(ui->pageListWidget,&QListWidget::itemDoubleClicked,[this](QListWidgetItem* item) {
        const int row = ui->pageListWidget->row(item);
        if (row > 0 && row < ui->pageListWidget->count()) {
            ui->stackedWidget->setCurrentIndex(row);
        }
    });
    ui->pageListWidget->setCurrentRow(0);
    ui->stackedWidget->setCurrentIndex(0);
}

RentTeacher::~RentTeacher() {
    delete ui;
}

void RentTeacher::loadData() {
    //调用模型函数，抓取数据
    modelDevice->fetchData();
    modelRequest->fetchData();
}

void RentTeacher::setUpModel_device() {
    modelDevice = new dataModel::EquipmentDataModel(this);
    //设置模型
    ui->sendTableView->setModel(modelDevice);
    //隐藏ID和列数
    ui->sendTableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->sendTableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
}

void RentTeacher::setUpModel_request() {
    modelRequest = new dataModel::BookingDataModel(this);
    //设置模型
    ui->examTableView->setModel(modelRequest);
    //设置隐藏列
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_Id);
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_Count);
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_ActualStartDate);
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_ActualEndDate);
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_UserGroup);
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_CreateDate);
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_ApproverName);
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_ApproverID);
    ui->examTableView->hideColumn(dataModel::BookingDataModel::Col_ApprovalDate);
}

// void RentTeacher::setColEditable(QStandardItemModel *model, int col, bool editable) {
//     if (!model) {
//         return;
//     }
//     //按行遍历，将每行的特定列设置状态
//     for (int row = 0; row < model->rowCount(); row++) {
//         QStandardItem *item = model->item(row, col);
//         if (item) {
//             item->setEditable(editable);
//         }
//     }
// }

void RentTeacher::on_btnSend_clicked()
{
    //设置选择模型
    QItemSelectionModel *selectionModel = ui->sendTableView->selectionModel();
    if (selectionModel->hasSelection()) {//如果有单元格被选择
        QModelIndexList  indexes = selectionModel->selectedRows();//获取index
        int index = indexes.at(0).row();//转为行数
        QModelIndex statusIndex = modelDevice->index(index, dataModel::EquipmentDataModel::Col_Status);
        QModelIndex nameIndex = modelDevice->index(index, dataModel::EquipmentDataModel::Col_Name);
        QString status = modelDevice->data(statusIndex).toString();
        if (status == "可用"){
            QString name = modelDevice->data(nameIndex).toString();
            sendRent = new SendRent(name, this);
            sendRent->show();
        }
    }
    else {
        sendRent = new SendRent(this);
        sendRent->show();
    }
}

void RentTeacher::on_btnCheck_clicked()
{
    apply = new Apply(this);
    apply->show();
}


} // view::Order

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
    this->setWindowTitle("我的事务");
    this->setAttribute(Qt::WA_DeleteOnClose,false);
    ui->pageListWidget->addItem("申请");
    ui->pageListWidget->addItem("审批");
    ui->pageListWidget->addItem("报修");
    ui->pageListWidget->addItem("归还");
    setUpModel_device();
    setUpModel_request();
    setUpModel_repair();
    setUpModel_return();
    loadData();
    connect(ui->pageListWidget,&QListWidget::itemDoubleClicked,[this](QListWidgetItem* item) {
        const int row = ui->pageListWidget->row(item);
        if (row >= 0 && row < ui->pageListWidget->count()) {
            ui->stackedWidget->setCurrentIndex(row);
        }
    });
    // ui->pageListWidget->setCurrentRow(0);
    // ui->stackedWidget->setCurrentIndex(0);
}

RentTeacher::RentTeacher(const QString &name, const QString &id, QWidget *parent) :
    QWidget(parent),ui(new Ui::RentTeacher),name(name),id(id){
    ui->setupUi(this);
    this->setWindowTitle("我的事务");
    ui->pageListWidget->addItem("申请");
    ui->pageListWidget->addItem("审批");
    ui->pageListWidget->addItem("报修");
    ui->pageListWidget->addItem("归还");
    setUpModel_device();
    setUpModel_request();
    setUpModel_repair();
    setUpModel_return();
    loadData();
    connect(ui->pageListWidget,&QListWidget::itemDoubleClicked,[this](QListWidgetItem* item) {
        const int row = ui->pageListWidget->row(item);
        if (row >= 0 && row < ui->pageListWidget->count()) {
            ui->stackedWidget->setCurrentIndex(row);
        }
    });
    // ui->pageListWidget->setCurrentRow(0);
    // ui->stackedWidget->setCurrentIndex(0);
}

RentTeacher::~RentTeacher() {
    delete ui;
}

void RentTeacher::loadData() {
    //调用模型函数，抓取数据
    modelDevice->fetchData();
    //只显示可用
    deviceFilterProxyMdel->setStatusColumn(dataModel::EquipmentDataModel::Col_Status);
    deviceFilterProxyMdel->setStatusFilter("可用");

    modelRequest->fetchData();
    //只显示学生
    requestFilterProxyMdel->setGroupColunm(dataModel::BookingDataModel::Col_UserGroup);
    requestFilterProxyMdel->setGroupFilter("Student");

    modelRepair->fetchData();
    //只显示可用
    repairFilterProxyMdel->setStatusColumn(dataModel::EquipmentDataModel::Col_Status);
    repairFilterProxyMdel->setStatusFilter("可用");

    modelReturn->fetchData();
    //显示自己和借出
    returnFilterProxyMdel->setStatusColumn(dataModel::EquipmentDataModel::Col_Status);
    returnFilterProxyMdel->setStatusFilter("借出");
    returnFilterProxyMdel->setUserIdColumn(dataModel::EquipmentDataModel::Col_RentId);
    returnFilterProxyMdel->setUserIdFilter(id.toInt());
}

void RentTeacher::setUpModel_device() {
    modelDevice = new dataModel::EquipmentDataModel(this);
    deviceFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
    deviceFilterProxyMdel->setSourceModel(modelDevice);
    //设置模型
    ui->sendTableView->setModel(deviceFilterProxyMdel);
    //隐藏ID和列数和借用人ID
    ui->sendTableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->sendTableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
    ui->sendTableView->hideColumn(dataModel::EquipmentDataModel::Col_RentId);
    ui->sendTableView->hideColumn(dataModel::EquipmentDataModel::Col_ClassId);
}

void RentTeacher::setUpModel_request() {
    modelRequest = new dataModel::BookingDataModel(this);
    modelRequest->setCurrentUserId(id.toInt(),name);
    connect(modelRequest,&dataModel::BookingDataModel::approvalStatusChanged,this,&view::Order::RentTeacher::loadData);
    requestFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
    requestFilterProxyMdel->setSourceModel(modelRequest);
    //设置模型
    ui->examTableView->setModel(requestFilterProxyMdel);
    //使用代理类来代理状态栏
    auto * approvalDelegate = new delegateModel::ApprovalStatusDelegate(this);
    ui->examTableView->setItemDelegateForColumn(dataModel::BookingDataModel::Col_ApprovalStatus, approvalDelegate);

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

void RentTeacher::setUpModel_repair() {
    modelRepair = new dataModel:: EquipmentDataModel(this);
    repairFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
    repairFilterProxyMdel->setSourceModel(modelRepair);
    //设置模型
    ui->repairTableView->setModel(repairFilterProxyMdel);
    //使用代理类来代理状态栏
    auto * repairDelegate = new delegateModel::RepairStatusDelegate(this);
    ui->repairTableView->setItemDelegateForColumn(dataModel::EquipmentDataModel::Col_Status, repairDelegate);
    //设置隐藏列
    ui->repairTableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->repairTableView->hideColumn(dataModel::EquipmentDataModel::Col_RentId);
    ui->repairTableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
    ui->repairTableView->hideColumn(dataModel::EquipmentDataModel::Col_ClassId);
}

void RentTeacher::setUpModel_return() {
    modelReturn = new dataModel::EquipmentDataModel(this);
    returnFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
    returnFilterProxyMdel->setSourceModel(modelReturn);
    //设置模型
    ui->returnTableView->setModel(returnFilterProxyMdel);
    //设置隐藏列
    ui->returnTableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->returnTableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
    ui->returnTableView->hideColumn(dataModel::EquipmentDataModel::Col_ClassId);
}

void RentTeacher::setIndex(int row) {
    ui->pageListWidget->setCurrentRow(row);
    ui->stackedWidget->setCurrentIndex(row);
    this->show();
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
        QModelIndexList  indexes = selectionModel->selectedIndexes();//获取index
        QModelIndex proxyIndex = indexes.first();
        QModelIndex index = deviceFilterProxyMdel->mapToSource(proxyIndex);
        QModelIndex statusIndex = modelDevice->index(index.row(), dataModel::EquipmentDataModel::Col_Status);
        QModelIndex devIndex = modelDevice->index(index.row(), dataModel::EquipmentDataModel::Col_Name);
        QModelIndex equipmentIdIndex = modelDevice->index(index.row(), dataModel::EquipmentDataModel::Col_ID);
        QModelIndex equipmentClassIDIndex = modelDevice->index(index.row(), dataModel::EquipmentDataModel::Col_ClassId);
        QString status = modelDevice->data(statusIndex).toString();
        if (status == "可用"){
            QString devName = modelDevice->data(devIndex).toString();
            int equipmentId = modelDevice->data(equipmentIdIndex).toInt();
            int equipmentClassId = modelDevice->data(equipmentClassIDIndex).toInt();
            sendRent = new SendRent(name, id,devName,equipmentId,equipmentClassId,this);
            sendRent->show();
        }
    }
    else {
        sendRent = new SendRent(name,id,this);
        sendRent->show();
    }
}

void RentTeacher::on_btnCheck_clicked()
{
    apply = new Apply(name,id);
    apply->show();
}

void RentTeacher::on_btnReturn_clicked() {
    QItemSelectionModel *selectionModel = ui->returnTableView->selectionModel();
    if (selectionModel->hasSelection()) {
        QModelIndexList  indexes = selectionModel->selectedIndexes();
        QModelIndex proxyIndex = indexes.first();
        QModelIndex index = returnFilterProxyMdel->mapToSource(proxyIndex);
        QModelIndex idIndex = modelReturn->index(index.row(), dataModel::EquipmentDataModel::Col_ID);
        int id = modelReturn->data(idIndex).toInt();
        if (data::Equipment::updateEquipmentOnReturn(id)) {
            loadData();
        }
        else {
            QMessageBox::warning(this,"警告","归还失败",QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::warning(this,"警告","请选择要归还的设备",QMessageBox::Ok);
    }
}
} // view::Order

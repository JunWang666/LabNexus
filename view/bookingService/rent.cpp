//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Rent.h" resolved

#include "rent.h"

#include <QMessageBox>

#include "ui_Rent.h"

namespace view::Order {
Rent::Rent(QWidget *parent) :
    QWidget(parent), ui(new Ui::Rent) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,false);
    ui->pageListWidget->addItem("申请");
    ui->pageListWidget->addItem("报修");
    ui->pageListWidget->addItem("归还");
    this->setWindowTitle("我的事务");
    setUpModel();
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

Rent::Rent(const QString &name,const QString &id, QWidget *parent):
    QWidget(parent),ui(new Ui::Rent), name(name),id(id){
    ui->setupUi(this);
    ui->pageListWidget->addItem("申请");
    ui->pageListWidget->addItem("报修");
    ui->pageListWidget->addItem("归还");
    this->setWindowTitle("我的事务");
    setUpModel();
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


Rent::~Rent() {
    delete ui;
}

void Rent::loadData()
{
    //抓取数据
    modelRent->fetchData();
    //过滤状态
    rentFilterProxyMdel->setStatusColumn(dataModel::EquipmentDataModel::Col_Status);
    rentFilterProxyMdel->setStatusFilter("可用");

    //抓取数据
    modelRepair->fetchData();
    //过滤状态
    repairFilterProxyMdel->setStatusColumn(dataModel::EquipmentDataModel::Col_Status);
    repairFilterProxyMdel->setStatusFilter("可用");

    //抓取数据
    modelReturn->fetchData();
    //过滤id和状态
    returnFilterProxyMdel->setStatusColumn(dataModel::EquipmentDataModel::Col_Status);
    returnFilterProxyMdel->setStatusFilter("借出");
    returnFilterProxyMdel->setUserIdColumn(dataModel::EquipmentDataModel::Col_RentId);
    returnFilterProxyMdel->setUserIdFilter(id.toInt());
}

void Rent::setUpModel()
{
    //初始化模型
    modelRent = new dataModel::EquipmentDataModel(this);
    rentFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
    rentFilterProxyMdel->setSourceModel(modelRent);
    //给视图指定模型
    ui->stuRentTableView->setModel(rentFilterProxyMdel);
    ui->stuRentTableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->stuRentTableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
    ui->stuRentTableView->hideColumn(dataModel::EquipmentDataModel::Col_RentId);
    ui->stuRentTableView->hideColumn(dataModel::EquipmentDataModel::Col_ClassId);
    ui->stuRentTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //初始化模型
    modelRepair = new dataModel::EquipmentDataModel(this);
    repairFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
    repairFilterProxyMdel->setSourceModel(modelRepair);
    //给视图指定模型
    ui->repairTableView->setModel(repairFilterProxyMdel);

    //使用代理类来代理状态栏
    auto * repairDelegate = new delegateModel::RepairStatusDelegate(this);
    ui->repairTableView->setItemDelegateForColumn(dataModel::EquipmentDataModel::Col_Status, repairDelegate);

    ui->repairTableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->repairTableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
    ui->repairTableView->hideColumn(dataModel::EquipmentDataModel::Col_RentId);
    ui->repairTableView->hideColumn(dataModel::EquipmentDataModel::Col_ClassId);

    //初始化模型
    modelReturn = new dataModel::EquipmentDataModel(this);
    returnFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
    returnFilterProxyMdel->setSourceModel(modelReturn);
    //给视图指定模型
    ui->returnTableView->setModel(returnFilterProxyMdel);
    ui->returnTableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->returnTableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
    ui->returnTableView->hideColumn(dataModel::EquipmentDataModel::Col_RentId);

}

void Rent::setIndex(int row) {
    ui->pageListWidget->setCurrentRow(row);
    ui->stackedWidget->setCurrentIndex(row);
    this->show();
}

// void Rent::setColEditable(QStandardItemModel *model, int col, bool editable) {
//     if (!model) {
//         return;
//     }
//     //按行遍历，每行的特定列设置状态
//     for (int row = 0; row < model->rowCount(); row++) {
//         QStandardItem *item = model->item(row, col);
//         if (item) {
//             item->setEditable(editable);
//         }
//     }
// }

//点击发送申请按钮显示对话框
void Rent::on_btnSend_clicked()
{
    //初始化选择模型
    QItemSelectionModel *selectionModel = ui->stuRentTableView->selectionModel();
    if (selectionModel->hasSelection()) {//如果有被选择的单元格则获取它的index
        QModelIndexList  indexes = selectionModel->selectedIndexes();
        QModelIndex proxyIndex = indexes.first();
        QModelIndex index = rentFilterProxyMdel->mapToSource(proxyIndex);
        QModelIndex statusIndex = modelRent->index(index.row(), dataModel::EquipmentDataModel::Col_Status);//获取状态索引
        QModelIndex devIndex = modelRent->index(index.row(), dataModel::EquipmentDataModel::Col_Name);//获取名称索引
        QModelIndex idIndex = modelRent->index(index.row(),dataModel::EquipmentDataModel::Col_ID);//获取id
        QModelIndex classIdIndex = modelRent->index(index.row(),dataModel::EquipmentDataModel::Col_ClassId);//获取classId
        QString status = modelRent->data(statusIndex).toString();
        if (status == "可用") {
            QString devType = modelRent->data(devIndex).toString();
            int equipmentId = modelRent->data(idIndex).toInt();
            int classId = modelRent->data(classIdIndex).toInt();
            sendRent = new SendRent(name,id,devType,equipmentId,classId,this);
            sendRent->show();
        }
    }
    else {
        sendRent = new SendRent(name,id,this);
        sendRent->show();
    }


}
//点击查看按钮查看申请
void Rent::on_btnCheck_clicked()
{
    checkApply = new Apply(name,id);
    checkApply->show();
}

void Rent::on_btnReturn_clicked() {
    QItemSelectionModel *selectionModel = ui->returnTableView->selectionModel();
    if (selectionModel->hasSelection()) {
        QModelIndexList  indexes = selectionModel->selectedIndexes();
        QModelIndex proxyIndex = indexes.first();
        QModelIndex index  = returnFilterProxyMdel->mapToSource(proxyIndex);
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

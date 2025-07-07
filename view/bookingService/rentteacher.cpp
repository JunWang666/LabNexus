//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RentTeacher.h" resolved

#include "rentteacher.h"
#include "ui_RentTeacher.h"

namespace view::Order {
RentTeacher::RentTeacher(QWidget *parent) :
    QWidget(parent), ui(new Ui::RentTeacher) {
    ui->setupUi(this);
    setUpModel_device();
    setUpModel_request();
    loadData();
    setColEditable(modelDevice,0,false);
    setColEditable(modelDevice,1,false);
    setColEditable(modelDevice,2,false);
    setColEditable(modelRequest,0,false);
    setColEditable(modelRequest,1,false);
    setColEditable(modelRequest,2,false);
}

RentTeacher::~RentTeacher() {
    delete ui;
}

void RentTeacher::loadData() {
    //清除缓存，清空模型
    modelDevice->removeRows(0, modelDevice->rowCount());
    modelRequest->removeRows(0, modelRequest->rowCount());
    /*占位符，获取数据
     * QList<Equipment> equp = dataBaseManager.getdata();
     */
    // for (const auto& equi : /*equp*/)
    // {
    //     QList<QStandardItem*> items;
    //     /*
    //      *添加数据 items.append();
    //      */
    //     model->appendRow(items);
    // }
    /*占位符，获取数据
     * QList<Request> req = dataBaseManager.getData;
     */
    // for (const auto& reqs : /*req*/)
    // {
    //     QList<QStandardItem*> items;
    //     /*
    //      *添加数据 items.append();
    //      */
    //     model->appendRow(items);
    // }

}

void RentTeacher::setUpModel_device() {
    modelDevice = new QStandardItemModel(this);
    modelDevice->setColumnCount(4);
    modelDevice->setHorizontalHeaderLabels({
        QString("类别"),
        QString("教室"),
        QString("入库时间"),
        QString("状态")
    });
    ui->sendTableView->setModel(modelDevice);
}

void RentTeacher::setUpModel_request() {
    modelRequest = new QStandardItemModel(this);
    modelRequest->setColumnCount(4);
    modelRequest->setHorizontalHeaderLabels({
        QString("借用人"),
        QString("借用时间"),
        QString("归还时间"),
        QString("批准")
    });
    ui->sendTableView->setModel(modelRequest);
}

void RentTeacher::setColEditable(QStandardItemModel *model, int col, bool editable) {
    if (!model) {
        return;
    }

    for (int row = 0; row < model->rowCount(); row++) {
        QStandardItem *item = model->item(row, col);
        if (item) {
            item->setEditable(editable);
        }
    }
}

void RentTeacher::on_btnSend_clicked()
{
    QItemSelectionModel *selectionModel = ui->sendTableView->selectionModel();
    if (selectionModel->hasSelection()) {
        QModelIndexList  indexes = selectionModel->selectedRows();
        int index = indexes.at(0).row();
        if (modelDevice->item(index,3)->text() == "/*可用*/" ) {
            QString name = modelDevice->item(index,1)->text();
            sendRent = new SendRent(name,this);
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

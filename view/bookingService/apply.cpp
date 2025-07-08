//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Apply.h" resolved

#include "apply.h"
#include "ui_Apply.h"
#include "module/model/EquipmentDataModel.h"

namespace view::Order {
Apply::Apply(QWidget *parent) :
    QWidget(parent), ui(new Ui::Apply) {
    ui->setupUi(this);
    setUpModel();
    loadData();
}

Apply::~Apply() {
    delete ui;
}

void Apply::loadData() {
    //抓取数据
    model->fetchData();
}

void Apply::setUpModel() {
    //初始化模型
    model = new dataModel::BookingDataModel(this);
    //设置模型
    ui->applyTableView->setModel(model);
    //隐藏某些列
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_Id);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_UserGroup);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_CreateDate);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_ActualStartDate);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_ActualEndDate);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_ApproverID);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_ApproverName);
    //设置不可更改
    ui->applyTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Apply::on_btnClose_clicked()
{
    this->close();
}

} // view::Order

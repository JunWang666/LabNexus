//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ManagerCheck.h" resolved

#include "managercheck.h"
#include "ui_ManagerCheck.h"

namespace view::Order {
ManagerCheck::ManagerCheck(QWidget *parent) :
    QWidget(parent), ui(new Ui::ManagerCheck) {
    ui->setupUi(this);
}

ManagerCheck::~ManagerCheck() {
    delete ui;
}

void ManagerCheck::loadData() {
    model->fetchData();
}

void ManagerCheck::setUpModel() {
    //初始化模型
    model = new dataModel::BookingDataModel(this);
    //设置模型
    ui->CheckTableView->setModel(model);
    //隐藏某些列
    ui->CheckTableView->hideColumn(dataModel::BookingDataModel::Col_Id);
    ui->CheckTableView->hideColumn(dataModel::BookingDataModel::Col_UserGroup);
    ui->CheckTableView->hideColumn(dataModel::BookingDataModel::Col_CreateDate);
    ui->CheckTableView->hideColumn(dataModel::BookingDataModel::Col_Count);
}

// void ManagerCheck::setColEditable(QStandardItemModel *model, int col, bool editable) {
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

void ManagerCheck::on_btnClose_clicked()
{
    this->close();
}

} // view::Order

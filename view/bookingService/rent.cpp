//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Rent.h" resolved

#include "rent.h"
#include "ui_Rent.h"

namespace view::Order {
Rent::Rent(QWidget *parent) :
    QWidget(parent), ui(new Ui::Rent) {
    ui->setupUi(this);
    this->setWindowTitle("temp_rent");
    setUpModel();
    loadData();
}

Rent::Rent(const QString &name,const QString &id, QWidget *parent):
    QWidget(parent),ui(new Ui::Rent), name(name),id(id){
    ui->setupUi(this);
    this->setWindowTitle("temp_rent");
    setUpModel();
    loadData();
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
        QModelIndexList  indexes = selectionModel->selectedRows();
        int index = indexes.at(0).row();//转换为行号
        QModelIndex statusIndex = model->index(index, dataModel::EquipmentDataModel::Col_Status);//获取状态索引
        QModelIndex nameIndex = model->index(index, dataModel::EquipmentDataModel::Col_Name);//获取名称索引
        QString status = model->data(statusIndex).toString();
        if (status == "可用") {
            QString name = model->data(nameIndex).toString();
            sendRent = new SendRent(name,id,this);
            sendRent->show();
        }
    }
    else {
        sendRent = new SendRent(this);
        sendRent->show();
    }


}
//点击查看按钮查看申请
void Rent::on_btnCheck_clicked()
{
    checkApply = new Apply(this);
    checkApply->show();
}

} // view::Order

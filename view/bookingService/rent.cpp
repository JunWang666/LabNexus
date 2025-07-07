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
    setColEditable(model,0,false);
    setColEditable(model,1,false);
    setColEditable(model,2,false);
}

Rent::~Rent() {
    delete ui;
}

void Rent::loadData()
{
    //清除缓存，清空模型
    model->removeRows(0, model->rowCount());
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

}

void Rent::setUpModel()
{
    model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHorizontalHeaderLabels({
        QString("类别"),
        QString("教室"),
        QString("入库时间"),
        QString("状态")
    });
    ui->stuRentTableView->setModel(model);
}

void Rent::setColEditable(QStandardItemModel *model, int col, bool editable) {
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

//点击发送申请按钮显示对话框
void Rent::on_btnSend_clicked()
{
    QItemSelectionModel *selectionModel = ui->stuRentTableView->selectionModel();
    if (selectionModel->hasSelection()) {
        QModelIndexList  indexes = selectionModel->selectedRows();
        int index = indexes.at(0).row();
        if (model->item(index,3)->text() == "/*可用*/" ) {
            QString name = model->item(index,1)->text();
            sendRent = new SendRent(name,this);
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

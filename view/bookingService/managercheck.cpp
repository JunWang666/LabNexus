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
    model->removeRows(0, model->rowCount());
    /*占位符，获取数据
     * QList<Request> requ = dataBaseManager.getdata();
     */
    // for (const auto& req : /*requ*/)
    // {
    //     QList<QStandardItem*> items;
    //     /*
    //      *添加数据 items.append();
    //      */
    //     model->appendRow(items);
    // }

}

void ManagerCheck::setUpModel() {
    model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHorizontalHeaderLabels({
        QString("借用人"),
        QString("借用时间"),
        QString("归还时间"),
        QString("批准")
    });
    ui->CheckTableView->setModel(model);
}

void ManagerCheck::setColEditable(QStandardItemModel *model, int col, bool editable) {
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

void ManagerCheck::on_btnClose_clicked()
{
    this->close();
}

} // view::Order

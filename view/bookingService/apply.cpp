//
// Created by Nana7mi_ on 25-7-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Apply.h" resolved

#include "apply.h"
#include "ui_Apply.h"
#include "module/model/EquipmentDataModel.h"
#include "module/data/data_mail.h"
namespace view::Order {
Apply::Apply(QWidget *parent) :
    QWidget(parent), ui(new Ui::Apply) {
    ui->setupUi(this);
    this->setWindowTitle("我的申请");
    this->setAttribute(Qt::WA_DeleteOnClose,false);
    setUpModel();
    loadData();
}

Apply::Apply(const QString &name, const QString &id, QWidget *parent):
    QWidget(parent), ui(new Ui::Apply),name(name),id(id){
    ui->setupUi(this);
    this->setWindowTitle("我的申请");
    this->setAttribute(Qt::WA_DeleteOnClose,false);
    setUpModel();
    loadData();

}

Apply::~Apply() {
    delete ui;
}

void Apply::loadData() {
    //抓取数据
    model->fetchData();
    //仅显示自己
    fliterModel->setUserIdColumn(dataModel::BookingDataModel::Col_UserId);
    fliterModel->setUserIdFilter(id.toInt());
}

void Apply::setUpModel() {
    //初始化模型
    model = new dataModel::BookingDataModel(this);
    fliterModel = new fliterModel::FilterProxyMdel(this);
    fliterModel->setSourceModel(model);
    //设置模型
    ui->applyTableView->setModel(fliterModel);
    //隐藏某些列
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_Id);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_UserGroup);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_CreateDate);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_ActualStartDate);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_ActualEndDate);
    ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_ApproverID);
    // ui->applyTableView->hideColumn(dataModel::BookingDataModel::Col_ApproverName);
    //设置不可更改
    ui->applyTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Apply::on_btnClose_clicked()
{
    this->close();
}

void Apply::on_btnRush_clicked() {
    if (teacherId =="") {
        setApprover = new SetApprover(teacherId,this);
        setApprover->show();
    }
    else {
        QItemSelectionModel *selectionModel = ui->applyTableView->selectionModel();
        if (selectionModel->hasSelection()) {//如果有单元格被选择
            QModelIndexList  indexes = selectionModel->selectedIndexes();//获取index
            QModelIndex proxyIndex = indexes.first();
            QModelIndex index = fliterModel->mapToSource(proxyIndex);
            // QString senderId = fliterModel->data(fliterModel->index(index.row(),dataModel::BookingDataModel::Col_Id)).toString();
            auto result = data::UserControl::Login::foundUserIdByIdNumber(teacherId);
            QString receiverId = "";
            if (result.has_value()) {
                receiverId = QString::number(result.value());
            }
            QDateTime startTime = fliterModel->data(fliterModel->index(index.row(),dataModel::BookingDataModel::Col_RequestStartDate)).toDateTime();
            QDateTime endTime = fliterModel->data(fliterModel->index(index.row(),dataModel::BookingDataModel::Col_RequestEndDate)).toDateTime();
            // QString type  =fliterModel->data(fliterModel->index(index.row(),dataModel::BookingDataModel::Col_));
            QString subject = "申请";
            QString senderName;
            auto resultSenderName = data::UserControl::UserInfo::getUserNameById(data::UserControl::currentUserId);
            if (resultSenderName.has_value()) {
                    senderName = resultSenderName.value();
            }
            QString receiverName;
            auto resultReceiverName1 = data::UserControl::UserInfo::getUserNameById(receiverId.toInt());
            if (resultReceiverName1.has_value()) {
                receiverName = resultReceiverName1.value();
            }
            QString content = tr("%1向%2申请%3设备").arg(senderName).arg(receiverName).arg("");
            data::mail::send_mail(data::UserControl::currentUserId,receiverId.toInt(),subject,content,"");
            QMessageBox::information(this,"提醒","发送成功",QMessageBox::Ok);
        }
        else {
            QMessageBox::warning(this,"警告","请选择一个申请",QMessageBox::Ok);
        }
    }


}
} // view::Order

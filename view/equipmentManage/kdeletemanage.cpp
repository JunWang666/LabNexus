#include "kdeletemanage.h"
#include "ui_kdeletemanage.h"
#include <QMessageBox>

namespace view::equipment {

kdeletemanage::kdeletemanage(QWidget *parent)
    : QDialog(parent)
    , ui(new view::equipment::Ui::kdeletemanage) {
    ui->setupUi(this);
    setWindowTitle("删除器材");
}

kdeletemanage::~kdeletemanage() {
    delete ui;
}

void kdeletemanage::on_addrecord_clicked()
{
    // int ret = QMessageBox::question(this, "确认", "确定要删除选中的器材吗？",
    //                                 QMessageBox::Yes | QMessageBox::No);
    // if (ret == QMessageBox::Yes) {
    //     // TODO: 实现删除选中器材的逻辑
    //     QMessageBox::information(this, "成功", "器材删除成功！");
    // }
}

void kdeletemanage::on_changerecord_clicked()
{
    // int ret = QMessageBox::question(this, "确认", "确定要批量删除器材吗？",
    //                                 QMessageBox::Yes | QMessageBox::No);
    // if (ret == QMessageBox::Yes) {
    //     // TODO: 实现批量删除器材的逻辑
    //     QMessageBox::information(this, "成功", "批量删除成功！");
    // }
}

void kdeletemanage::on_deleterecord_clicked()
{
    // int ret = QMessageBox::question(this, "确认", "确定要删除记录吗？",
    //                                 QMessageBox::Yes | QMessageBox::No);
    // if (ret == QMessageBox::Yes) {
    //     // TODO: 实现删除记录的逻辑
    //     QMessageBox::information(this, "成功", "记录删除成功！");
    // }
}

void kdeletemanage::on_close_clicked()
{
    this->close();
}





} // view::equipment

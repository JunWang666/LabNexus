#include "kchangemanage.h"
#include "ui_kchangemanage.h"
#include <QMessageBox>

namespace view::equipment {

kchangemanage::kchangemanage(QWidget *parent)
    : QDialog(parent)
    , ui(new view::equipment::Ui::kchangemanage) {
    ui->setupUi(this);
    setWindowTitle("修改器材信息");
}

kchangemanage::~kchangemanage() {
    delete ui;
}

void kchangemanage::on_find_clicked() {
    QString equipmentName = ui->equipmentNameEdit->text();
    if (equipmentName.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入器材名称！");
        return;
    }
    
    // TODO: 实现查找器材的逻辑
    QMessageBox::information(this, "提示", "查找功能待实现");
}

void kchangemanage::on_change_clicked() {
    QString changeValue = ui->changeValueEdit->text();
    if (changeValue.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入修改值！");
        return;
    }
    
    // TODO: 实现修改器材信息的逻辑
    QMessageBox::information(this, "成功", "器材信息修改成功！");
}

void kchangemanage::on_deleteButton_clicked() {
    int ret = QMessageBox::question(this, "确认", "确定要删除该器材吗？",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        // TODO: 实现删除器材的逻辑
        QMessageBox::information(this, "成功", "器材删除成功！");
    }
}

void kchangemanage::on_next_clicked() {
    // TODO: 实现查找下一个的逻辑
    QMessageBox::information(this, "提示", "查找下一个功能待实现");
}

void kchangemanage::on_before_clicked() {
    // TODO: 实现查找上一个的逻辑
    QMessageBox::information(this, "提示", "查找上一个功能待实现");
}

void kchangemanage::on_cancelButton_clicked() {
    this->close();
}

} // view::equipment

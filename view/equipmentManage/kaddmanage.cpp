#include "kaddmanage.h"
#include "ui_kaddmanage.h"
#include <QMessageBox>

namespace view::equipment {

kaddmanage::kaddmanage(QWidget *parent)
    : QDialog(parent)
    , ui(new view::equipment::Ui::kaddmanage) {
    ui->setupUi(this);
    setWindowTitle("添加器材");
}

kaddmanage::~kaddmanage() {
    delete ui;
}

void kaddmanage::on_addButton_clicked() {
    // 获取表单数据
    QString equipmentName = ui->equipmentNameEdit->text();
    QString equipmentId = ui->equipmentIdEdit->text();
    QString quantity = ui->quantityEdit->text();
    QString batchNumber = ui->batchNumberEdit->text();
    
    // 验证数据
    if (equipmentName.isEmpty() || equipmentId.isEmpty() || quantity.isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写必填项！");
        return;
    }
    
    // TODO: 实现添加器材到数据库的逻辑
    QMessageBox::information(this, "成功", "器材添加成功！");
    
    // 清空表单
    ui->equipmentNameEdit->clear();
    ui->equipmentIdEdit->clear();
    ui->quantityEdit->clear();
    ui->batchNumberEdit->clear();
}

} // view::equipment

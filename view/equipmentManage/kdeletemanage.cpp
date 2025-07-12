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
    int ret = QMessageBox::question(this, "确认", "仅看查记录新增记录",
                                     QMessageBox::Yes | QMessageBox::No);
     if (ret == QMessageBox::Yes) {
    QFile file("kadd_log.txt");
         if (file.exists()) {
             if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                 QByteArray content = file.readAll();
                 file.close();
                 QMessageBox::information(this, "记录查看", content);
             } else {
                 QMessageBox::information(this, "记录查看", "读取失败");
             }
         } else {
             QMessageBox::information(this, "记录查看", "暂无记录");
         }
}
}
void kdeletemanage::on_changerecord_clicked()
{
     int ret = QMessageBox::question(this, "确认", "仅看查记录修改记录",
                                     QMessageBox::Yes | QMessageBox::No);
     if (ret == QMessageBox::Yes) {
         QFile file("kchange_log.txt");
         if (file.exists()) {
             if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                 QByteArray content = file.readAll();
                 file.close();
                 QMessageBox::information(this, "记录查看", content);
             } else {
                 QMessageBox::information(this, "记录查看", "读取失败");
             }
         } else {
             QMessageBox::information(this, "记录查看", "暂无记录");
         }
   }
}

void kdeletemanage::on_deleterecord_clicked()
{
    int ret = QMessageBox::question(this, "确认", "确定要删除记录吗？",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        QFile file("kdelete_log.txt");
        if (file.exists()) {
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QByteArray content = file.readAll();
                file.close();
                QMessageBox::information(this, "记录查看", content);
            } else {
                QMessageBox::information(this, "记录查看", "读取失败");
            }
        } else {
            QMessageBox::information(this, "记录查看", "暂无记录");
        }
    }
}

void kdeletemanage::on_close_clicked()
{
    this->close();
}





} // view::equipment

#include "kchangemanage.h"
#include "ui_kchangemanage.h"
#include <QSqlTableModel>  // 包含QSqlTableModel头文件
#include <QMessageBox>
#include <QItemSelectionModel>
#include "module/model/EquipmentDataModel.h"
#include "module/model/filterproxymdel.h"
#include <QInputDialog>
#include "service/database/databaseManager.h" // 数据库操作类
#include "module/data/data_EquipmentManage.h"

using namespace view::equipment;

kchangemanage::kchangemanage(QWidget *parent)
    : QDialog(parent), ui(new Ui::kchangemanage)
   {
    ui->setupUi(this);
    //初始化模型
    modelRent = new dataModel::EquipmentDataModel(this);
    rentFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
    rentFilterProxyMdel->setSourceModel(modelRent);


    //给视图指定模型

    ui->tableView_change->setModel(rentFilterProxyMdel);
    ui->tableView_change->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->tableView_change->hideColumn(dataModel::EquipmentDataModel::Col_Count);
    ui->tableView_change->hideColumn(dataModel::EquipmentDataModel::Col_RentId);
    ui->tableView_change->hideColumn(dataModel::EquipmentDataModel::Col_ClassId);

    // 调整列宽以显示数据
    ui->tableView_change->resizeColumnsToContents();
    ui->tableView_change->horizontalHeader()->setStretchLastSection(true);

    // 关键设置：选择整行模式（解决卡死的关键）
    ui->tableView_change->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_change->setSelectionMode(QAbstractItemView::SingleSelection);

    // 加载数据
    modelRent->fetchData();

    // 调试信息
    qDebug() << "Model row count:" << modelRent->rowCount();
    qDebug() << "Model column count:" << modelRent->columnCount();
}

kchangemanage::~kchangemanage() {
    delete ui;
}

// 删除按钮功能 - 将选中行状态改为'deleted'
void kchangemanage::on_deleteButton_clicked() {
    // 获取当前选中行
    QModelIndex proxyIndex = ui->tableView_change->currentIndex();
    if (!proxyIndex.isValid()) {
        QMessageBox::warning(this, "操作失败", "请先选择要删除的设备");
        return;
    }

    // 映射到源模型索引
    QModelIndex sourceIndex = rentFilterProxyMdel->mapToSource(proxyIndex);
    int row = sourceIndex.row();

    // 获取设备ID（从源模型获取）
    int equipmentId = modelRent->index(row, dataModel::EquipmentDataModel::Col_ID).data().toInt();

    // 确认对话框
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "确认删除",
        "确定要将此设备标记为删除吗？",
        QMessageBox::Yes|QMessageBox::No
        );
    if (reply != QMessageBox::Yes) return;

    // 更新数据库
    service::DatabaseManager db(data::Equipment::path);
    QString query = QString("UPDATE equipment_instance SET status = 'deleted' WHERE id = %1")
                        .arg(equipmentId);

    if (db.executeNonQuery(query)) {
        QMessageBox::information(this, "成功", "设备状态已更新为删除");
        modelRent->fetchData(); // 刷新模型数据

        QFile file("kdelete_log.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            file.write(QString(query)
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                           .toUtf8());
            file.close();
        }

        emit dataChanged(); // 通知其他界面
    } else {
        QMessageBox::critical(this, "失败", "更新失败: " + db.getLastError());
    }
}

// 修改按钮功能 - 修改设备名称
void kchangemanage::on_changeButton_clicked() {
    // 获取当前选中行
    QModelIndex proxyIndex = ui->tableView_change->currentIndex();
    if (!proxyIndex.isValid()) {
        QMessageBox::warning(this, "操作失败", "请先选择要修改的设备");
        return;
    }

    // 映射到源模型索引
    QModelIndex sourceIndex = rentFilterProxyMdel->mapToSource(proxyIndex);
    int row = sourceIndex.row();

    // 获取当前设备名称
    QString currentName = modelRent->index(row, dataModel::EquipmentDataModel::Col_Name).data().toString();

    // 弹出输入对话框获取新名称
    bool ok;
    QString newName = QInputDialog::getText(
        this,
        "修改设备名称",
        "请输入新名称:",
        QLineEdit::Normal,
        currentName,
        &ok
        );

    if (!ok || newName.isEmpty()) return; // 用户取消或输入为空

    // 获取设备ID
    int equipmentId = modelRent->index(row, dataModel::EquipmentDataModel::Col_ID).data().toInt();

    // 更新数据库
    service::DatabaseManager db(data::Equipment::path);
    QString escapedName = newName.replace("'", "''");
    QString query = QString("UPDATE equipment_instance SET name = '%1' WHERE id = %2")
                        .arg(escapedName).arg(equipmentId);

    if (db.executeNonQuery(query)) {
        QMessageBox::information(this, "成功", "设备名称已更新");
        modelRent->fetchData(); // 刷新模型数据

        QFile file("kchange_log.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            file.write(QString(query)
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                           .toUtf8());
            file.close();
        }

         emit dataChanged(); // 通知其他界面
    } else {
        QMessageBox::critical(this, "失败", "更新失败: " + db.getLastError());
    }
}


void kchangemanage::on_statusButton_clicked()
{
    // 获取当前选中行
    QModelIndex proxyIndex = ui->tableView_change->currentIndex();
    if (!proxyIndex.isValid()) {
        QMessageBox::warning(this, "操作失败", "请先选择要修改的设备");
        return;
    }

    // 映射到源模型索引
    QModelIndex sourceIndex = rentFilterProxyMdel->mapToSource(proxyIndex);
    int row = sourceIndex.row();

    // 获取当前设备名称
    QString currentName = modelRent->index(row, dataModel::EquipmentDataModel::Col_Status).data().toString();

    // 弹出输入对话框获取新名称
    bool ok;
    QString newName = QInputDialog::getText(
        this,
        "修改状态",
        "请输入新状态:",
        QLineEdit::Normal,
        currentName,
        &ok
        );

    if (!ok || newName.isEmpty()) return; // 用户取消或输入为空

    // 获取设备ID
    int equipmentId = modelRent->index(row, dataModel::EquipmentDataModel::Col_ID).data().toInt();

    // 更新数据库
    service::DatabaseManager db(data::Equipment::path);
    QString escapedName = newName.replace("'", "''");
    QString query = QString("UPDATE equipment_instance SET name = '%1' WHERE id = %2")
                        .arg(escapedName).arg(equipmentId);

    if (db.executeNonQuery(query)) {
        QMessageBox::information(this, "成功", "设备状态已更新");
        modelRent->fetchData(); // 刷新模型数据

        QFile file("kchange_log.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            file.write(QString(query)
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                           .toUtf8());
            file.close();
        }

        emit dataChanged(); // 通知其他界面
    } else {
        QMessageBox::critical(this, "失败", "更新失败: " + db.getLastError());
    }
}


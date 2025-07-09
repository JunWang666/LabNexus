//
// Created by gouzuang on 25-7-6.
//

#include "equipment_home.h"
#include "ui_equipment_home.h"
#include "kaddmanage.h"
#include "kchangemanage.h"
#include "kdeletemanage.h"
#include "applysee.h"
#include "exam.h"
#include <QStandardItemModel>

namespace view::equipment {
    equipment_home::equipment_home(QWidget *parent) : QWidget(parent), ui(new view::equipment::Ui::equipment_home) {
        ui->setupUi(this);
    }

    equipment_home::~equipment_home() {
        delete ui;
    }

    void equipment_home::on_zclose_clicked() //关闭应用
    {
        this->close();
    }

    void equipment_home::on_kadd_clicked() //添加器材
    {
        kaddmanage *dialog = new kaddmanage(this);
        dialog->show();
    }

    void equipment_home::on_kreall_clicked() //整理库数据
    {
        // TODO: 实现整理库数据功能
    }

    void equipment_home::on_kchange_clicked() //修改库数据
    {
        kchangemanage *dialog = new kchangemanage(this);
        dialog->show();
    }

    void equipment_home::on_kdel_clicked() //删除库数据
    {
        kdeletemanage *dialog = new kdeletemanage(this);
        dialog->show();
    }

    void equipment_home::on_kexam_clicked() //检查和审批审查
    {
        // TODO: 实现检查和审批审查功能
    }

    void equipment_home::on_kseeborrow_clicked()
    {
        // 创建一个对话框
        QDialog dialog(this);
        dialog.setWindowTitle("借用日志");

        // 创建一个 QTableView
        QTableView tableView;

        // 创建一个模型（示例中使用 QStandardItemModel）
        QStandardItemModel model(0, 3); // 3 列：ID、设备名称、借用日期
        model.setHorizontalHeaderLabels(QStringList() << "ID" << "设备名称" << "借用日期");

        // 添加示例数据
        QList<QStandardItem *> items;
        for (int i = 0; i < 5; ++i) { // 添加5行示例数据
            items.append(new QStandardItem(QString::number(i + 1)));
            items.append(new QStandardItem("设备 " + QString::number(i + 1)));
            items.append(new QStandardItem("2024-11-01"));
            model.appendRow(items);
            items.clear(); // 清空 items 以准备下一行
        }

        // 设置模型到 QTableView
        tableView.setModel(&model);

        // 创建布局并添加 QTableView
        QVBoxLayout layout(&dialog);
        layout.addWidget(&tableView);

        // 显示对话框
        dialog.setLayout(&layout);
        dialog.exec();
    }

    void equipment_home::on_seefix_clicked()
    {
        QDialog dialog(this);
        dialog.setWindowTitle("维修日志");

        QTableView tableView;
        QStandardItemModel model(0, 3); // 3 列：ID、设备名称、维修日期
        model.setHorizontalHeaderLabels(QStringList() << "ID" << "设备名称" << "维修日期");

        // 添加示例数据
        QList<QStandardItem *> items;
        for (int i = 0; i < 3; ++i) {
            items.append(new QStandardItem(QString::number(i + 1)));
            items.append(new QStandardItem("设备 " + QString::number(i + 1)));
            items.append(new QStandardItem("2024-10-15"));
            model.appendRow(items);
            items.clear();
        }

        tableView.setModel(&model);

        QVBoxLayout layout(&dialog);
        layout.addWidget(&tableView);
        dialog.setLayout(&layout);
        dialog.exec();
    }

    void equipment_home::on_outdata_clicked()
    {
        QDialog dialog(this);
        dialog.setWindowTitle("超期预警");

        QTableView tableView;
        QStandardItemModel model(0, 3); // 3 列：ID、设备名称、超期天数
        model.setHorizontalHeaderLabels(QStringList() << "ID" << "设备名称" << "超期天数");

        QList<QStandardItem *> items;
        for (int i = 0; i < 2; ++i) {
            items.append(new QStandardItem(QString::number(i + 1)));
            items.append(new QStandardItem("设备 " + QString::number(i + 1)));
            items.append(new QStandardItem(QString::number(i + 5))); // 超期天数
            model.appendRow(items);
            items.clear();
        }

        tableView.setModel(&model);

        QVBoxLayout layout(&dialog);
        layout.addWidget(&tableView);
        dialog.setLayout(&layout);
        dialog.exec();
    }


    void equipment_home::on_blacklist_clicked()
    {
        QDialog dialog(this);
        dialog.setWindowTitle("黑名单");

        QTableView tableView;
        QStandardItemModel model(0, 2); // 2 列：用户ID、用户名
        model.setHorizontalHeaderLabels(QStringList() << "用户ID" << "用户名");

        QList<QStandardItem *> items;
        for (int i = 0; i < 4; ++i) {
            items.append(new QStandardItem(QString::number(i + 1001)));
            items.append(new QStandardItem("用户 " + QString::number(i + 1)));
            model.appendRow(items);
            items.clear();
        }

        tableView.setModel(&model);

        QVBoxLayout layout(&dialog);
        layout.addWidget(&tableView);
        dialog.setLayout(&layout);
        dialog.exec();
    }

    void equipment_home::on_searchbtn_clicked() //索引
    {
        // TODO: 实现搜索功能
    }
} // view::equipment

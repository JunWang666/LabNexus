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
#include "module/data/data_Booking.h"
#include "module/data/data_EquipmentManage.h"
#include <QStringConverter> // Qt 6 需包含

namespace view::equipment {
equipment_home::equipment_home(QWidget *parent) : QWidget(parent), ui(new view::equipment::Ui::equipment_home) {
    ui->setupUi(this);
     data::Equipment::buildDB();
    data::Equipment::EquipmentClass::createEquipmentClassTable();
     data::Equipment::EquipmentInstnace::createEquipmentInstanceTable();

}

equipment_home::~equipment_home() {
    delete ui;
}

void equipment_home::on_zclose_clicked() //关闭应用
{
    this->close();
}
// equipment_home.cpp


void equipment_home::on_kadd_clicked() //添加器材
{
    // 创建对话框并设置父对象（自动内存管理）
    kaddmanage *dialog = new kaddmanage(this);

    // 连接对话框的dataAdded信号到主界面的刷新函数（on_kreall_clicked）
    connect(dialog, &kaddmanage::dataAdded, this, [this]() {
        on_kreall_clicked(); // 调用整理库数据功能，刷新tableView
    });

    // 显示对话框（模态或非模态均可，此处用show()非模态）
    dialog->show();
}


void equipment_home::on_kreall_clicked() //整理库数据
{
    // 1. 确保数据库及表结构存在（原有功能）


    // 2. 加载所有设备完整记录
    QList<data::Equipment::fullEquipmentRecord> records = data::Equipment::loadFullEquipmentRecords();

    // 3. 按 "item" 字段升序排序（假设 "item" 对应设备名称 "name" 字段）
    // 使用 Qt 的字符串比较（支持本地化，类似Excel排序）
    std::sort(records.begin(), records.end(), [](const data::Equipment::fullEquipmentRecord& a, const data::Equipment::fullEquipmentRecord& b) {
        return QString::localeAwareCompare(a.name, b.name) < 0; // 升序排列
    });

    // 4. 将排序后的数据展示到 tableView（假设 UI 中构件名为 tableView）
    // 清空原有数据
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (!model) {
        model = new QStandardItemModel(this);
        // 设置表头（仅首次初始化时设置）
        model->setHorizontalHeaderLabels({
            "ID", "设备名称", "状态", "创建时间", "类别ID", "类别名称"
        });
        ui->tableView->setModel(model);
    }
    model->removeRows(0, model->rowCount()); // 清空旧数据

    // 填充排序后的数据
    for (const auto& rec : records) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(rec.id))         // ID
              << new QStandardItem(rec.name)                        // 设备名称（排序字段）
              << new QStandardItem(rec.status)                      // 状态
              << new QStandardItem(rec.inDate.toString("yyyy-MM-dd hh:mm:ss")) // 创建时间
              << new QStandardItem(QString::number(rec.class_id))   // 类别ID
              << new QStandardItem(rec.type);                       // 类别名称
        model->appendRow(items);
    }

    // 可选：自动调整列宽
    ui->tableView->resizeColumnsToContents();
    log(service::LogLevel::INFO) << "数据库整理完成";
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

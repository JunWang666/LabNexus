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
#include <QSqlTableModel>
#include <QTableView>
#include "view/bookingService/managercheck.h"
#include <QWidget>
#include "module/model/EquipmentDataModel.h"
#include "module/model/filterproxymdel.h"
#include "module/model/RepairStatusDelegate.h"
#include <QInputDialog>

namespace view::equipment {
equipment_home::equipment_home(QWidget *parent) : QWidget(parent), ui(new view::equipment::Ui::equipment_home)
{
    ui->setupUi(this);
     data::Equipment::buildDB();
     data::Equipment::EquipmentClass::createEquipmentClassTable();
     data::Equipment::EquipmentInstnace::createEquipmentInstanceTable();


     //初始化模型
     modelRent = new dataModel::EquipmentDataModel(this);
     rentFilterProxyMdel = new fliterModel::FilterProxyMdel(this);
     rentFilterProxyMdel->setSourceModel(modelRent);
     //给视图指定模型

     ui->tableView->setModel(rentFilterProxyMdel);
     ui->tableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
     ui->tableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
     ui->tableView->hideColumn(dataModel::EquipmentDataModel::Col_RentId);
     ui->tableView->hideColumn(dataModel::EquipmentDataModel::Col_ClassId);

      // 调整列宽以显示数据
      ui->tableView->resizeColumnsToContents();
      ui->tableView->horizontalHeader()->setStretchLastSection(true);

      // 加载数据
      modelRent->fetchData();

      // 调试信息
      qDebug() << "Model row count:" << modelRent->rowCount();
      qDebug() << "Model column count:" << modelRent->columnCount();
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



    // 2. 加载所有设备完整记录
    QList<data::Equipment::fullEquipmentRecord> records = data::Equipment::loadFullEquipmentRecords();

    // 3. 按 "item" 字段升序排序（ "item" 对应设备名称 "name" 字段）
    // 使用 Qt 的字符串比较（支持本地化，类似Excel排序）
    std::sort(records.begin(), records.end(), [](const data::Equipment::fullEquipmentRecord& a, const data::Equipment::fullEquipmentRecord& b) {
        return QString::localeAwareCompare(a.name, b.name) < 0; // 升序排列
    });

    // 4. 将排序后的数据展示到 tableView
    // 清空原有数据

    QStandardItemModel* model = nullptr; /*= qobject_cast<QStandardItemModel*>(ui->tableView->model());*/

    if (!model) {
        model = new QStandardItemModel(this);
        // 设置表头（仅首次初始化时设置）
        model->setHorizontalHeaderLabels({
            "ID", "设备名称", "状态", "创建时间","类别名称"
        });
        ui->tableView->setModel(model);
    }
    // model->removeRows(0, model->rowCount()); // 清空旧数据
    ui->tableView->hideColumn(dataModel::EquipmentDataModel::Col_ID);
    ui->tableView->hideColumn(dataModel::EquipmentDataModel::Col_Count);
    ui->tableView->hideColumn(dataModel::EquipmentDataModel::Col_RentId);
    ui->tableView->hideColumn(dataModel::EquipmentDataModel::Col_ClassId);

    // 填充排序后的数据
    for (const auto& rec : records) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(rec.id))         // ID
              << new QStandardItem(rec.name)                        // 设备名称（排序字段）
              << new QStandardItem(rec.status)                      // 状态
              << new QStandardItem(rec.inDate.toString("yyyy-MM-dd hh:mm:ss")) // 创建时间
              << new QStandardItem(QString::number(rec.class_id));   // 类别名称

        model->appendRow(items);
    }

    // 可选：自动调整列宽
    ui->tableView->resizeColumnsToContents();
    log(service::LogLevel::INFO) << "数据库整理完成";
}


void equipment_home::on_kchange_clicked() //修改库数据
{
    // 创建对话框实例（父对象设为当前窗口，自动管理内存）
    kchangemanage *dialog = new kchangemanage(this);
    // 设置对话框关闭时自动销毁，避免内存泄漏
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    // 连接对话框的dataAdded信号到主界面的刷新函数（on_kreall_clicked）
    connect(dialog, &kchangemanage::dataChanged, this, [this]() {
        on_kreall_clicked(); // 调用整理库数据功能，刷新tableView
    });
    dialog->show();

}

void equipment_home::on_kdel_clicked() //操作记录查看
{
    kdeletemanage *dialog = new kdeletemanage(this);

    if (!dialog || !dialog->isVisible()) { // 检查窗口是否存在且是否可见
        dialog = new kdeletemanage(this);
        dialog->show();
    } else {
        // 如果窗口已经存在但被隐藏，则显示它
        dialog->show();
    }

}

void equipment_home::on_kexam_clicked() //检查和审批审查
{
    view::Order::ManagerCheck *a = new view::Order::ManagerCheck();
    service::style::setMica(a);
    service::MutiWindow::manager().addWindow(a);
    a->show();
}

void equipment_home::on_kseeborrow_clicked()//借用记录
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

void equipment_home::on_seefix_clicked()//维修记录
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
    rentFilterProxyMdel->clearFilters();

    // 获取UI输入值
    QString className = ui->expclassline->text().trimmed();
    QString status = ui->expstateline->text().trimmed();


    // 应用仪器类别过滤（按名称）
    if (!className.isEmpty()) {
        //抓取数据
        modelRent->fetchData();
        //过滤状态
        rentFilterProxyMdel->setNameColunm(dataModel::EquipmentDataModel::Col_Name);
        rentFilterProxyMdel->setnameFilter(className);
    }
        if (!status.isEmpty()) {
        modelRent->fetchData();
        //过滤状态
        rentFilterProxyMdel->setStatusColumn(dataModel::EquipmentDataModel::Col_Status);
        rentFilterProxyMdel->setStatusFilter(status);
        }

        if(className.isEmpty()&&status.isEmpty()){
            QDialog *a = new QDialog(this);
            a->setWindowTitle("过滤字段");
            a->setModal(true);
            a->setAttribute(Qt::WA_DeleteOnClose);   // 关闭后自动释放

            QLabel *label = new QLabel("请输入要过滤字段输入：", a);
            label->setAlignment(Qt::AlignCenter);

            QPushButton *okBtn = new QPushButton("确定", a);
            connect(okBtn, &QPushButton::clicked, a, &QDialog::accept);

            QVBoxLayout *lay = new QVBoxLayout(a);
            lay->addWidget(label);
            lay->addWidget(okBtn);

            a->resize(300, 120);
            a->exec();          // 模态弹出
        }


    // 更新统计信息
    int visibleCount = rentFilterProxyMdel->rowCount();
    ui->fstatisticline->setText(QString("找到 %1 条记录").arg(visibleCount));

    // 刷新视图
    ui->tableView->viewport()->update();
    }

    }



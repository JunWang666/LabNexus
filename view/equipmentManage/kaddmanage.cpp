#include "view/equipmentManage/kaddmanage.h"
#include <QMessageBox>
#include <QDate> // 用于获取当前日期
#include "service/database/databaseManager.h" // 数据库操作类
#include "module/data/data_EquipmentManage.h"
kaddmanage::kaddmanage(QWidget *parent)
    : QDialog(parent), ui(new view::equipment::Ui::kaddmanage) {
    ui->setupUi(this);
    loadClassrooms();        // 加载“仪器类”下拉框数据（新增）
    initStatusOptions();     // 初始化状态下拉框（原逻辑）

    connect(ui->addButton, &QPushButton::clicked, this, &kaddmanage::on_addButton_clicked);
}

kaddmanage::~kaddmanage() {
    delete ui;
}

void kaddmanage::loadClassrooms() {
    service::DatabaseManager db(data::Equipment::path);
    QString query = "SELECT id, name FROM equipment_class"; // 仪器类信息存储在 classroom 表
    auto results = db.executeQueryAndFetchAll(query); // 从数据库查询教室数据

    ui->comboBox->clear(); // 清空下拉框原有内容


    if (results.isEmpty()) {

        ui->comboBox->addItem("无可用仪器类",-1);
    } else {
        // 数据库有数据时，正常加载数据库中的仪器类
        ui->comboBox->addItem("请选择所在仪器类", -1);
        for (const auto &row : results) {
            int classroomId = row["id"].toInt();
            QString classroomName = row["name"].toString();
            ui->comboBox->addItem(classroomName, classroomId); // 添加“名称-ID”对
        }
    }
}

// 初始化状态下拉框（原逻辑）
void kaddmanage::initStatusOptions() {
    ui->comboBox_2->clear();
    ui->comboBox_2->addItems({"可用", "维修中", "借出","delete"});
    ui->comboBox_2->setCurrentText("可用");
}

void kaddmanage::on_addButton_clicked() {
    // 从 UI 控件获取输入（注意转义单引号防 SQL 注入）
    QString instrumentName = ui->equipmentNameEdit->text().trimmed().replace("'", "''"); // 设备名称（如“显微镜”）
    QString batchNumber = ui->batchNumberEdit->text();       // 批次号（如“2024001”）
    QString quantityStr = ui->quantityEdit->text().trimmed();                             // 添加个数（如“3”）
    QString statuss = ui->comboBox_2->currentText().replace("'", "''");                    // 设备状态（如“可用”）
    int classId = ui->comboBox->currentData().toInt();                                    // 设备类别ID（从下拉框获取）
    QString className = ui->comboBox->currentText().replace("'", "''");                   // 设备类别名称（用于提示）

    // 输入校验
    if (instrumentName.isEmpty()) {
        QMessageBox::warning(this, "错误", "设备名称不能为空！");
        return;
    }
    if (batchNumber.isEmpty()) {
        QMessageBox::warning(this, "错误", "批次号不能为空！");
        return;
    }
    bool isNum;
    int quantity = quantityStr.toInt(&isNum);
    if (!isNum || quantity <= 0) {
        QMessageBox::warning(this, "错误", "添加个数必须为正整数！");
        return;
    }
    if (classId == -1) {
        QMessageBox::warning(this, "错误", "请选择有效的设备类别！");
        return;
    }

    // 生成设备编号（规则：名称+日期+批次+序号，如“显微镜_20240708_2024001_1”）
    QDateTime currentDate = QDateTime::currentDateTime();
    QString dateStr = currentDate.toString("yyyy-MM-dd hh:mm:ss");
    QString baseNumber = QString("%1_%2_%3").arg(instrumentName).arg(dateStr).arg(batchNumber);
    QString rebaseNumber = QString("%1_%2_%3").arg(instrumentName).arg(batchNumber);

    // 数据库操作
    service::DatabaseManager db(data::Equipment::path);
    db.beginTransaction();
    int successCount = 0;
    int skipCount = 0;
    QString errorDetails;

    for (int i = 0; i < quantity; ++i) {
        // 生成最终设备编号（序号从_1开始）
        QString finalEquipmentNumber = (quantity > 1)
                                           ? QString("%1_%2").arg(baseNumber).arg(i + 1)
                                           : baseNumber;

        // 步骤1：检查设备编号是否已存在（避免冲突）
        QString checkQuery = QString(R"(
            SELECT COUNT(*) AS count FROM equipment_instance
            WHERE equipment_number = '%1'
        )").arg(finalEquipmentNumber);
        auto checkResult = db.executeQueryAndFetchAll(checkQuery);


        // 步骤2：插入新记录（使用数据库自增id，不手动指定）
        QString insertQuery = QString(R"(
            INSERT INTO equipment_instance (
                 name, status,created_at,class_id
            ) VALUES (
                '%1', '%2', '%3' , %4
            )
        )").arg(instrumentName).arg(statuss).arg(dateStr).arg(classId);              // 设备状态（下拉框选择）


        if (db.executeNonQuery(insertQuery)) {
            successCount++;
        } else {
            errorDetails += QString("第%1条记录插入失败：%2\n").arg(i + 1).arg(db.getLastError());
        }
    }

    // 提交事务并反馈结果
    if (successCount > 0 || skipCount > 0) {
        db.commitTransaction();
        QString msg = QString("成功添加%1个设备，跳过%2个重复设备！")
                          .arg(successCount).arg(skipCount);
        if (!errorDetails.isEmpty()) {
            msg += QString("\n错误详情：%1").arg(errorDetails);
        }
        QMessageBox::information(this, "结果", msg);
    } else {
        db.rollbackTransaction();
        QMessageBox::critical(this, "失败", QString("所有记录插入失败：%1").arg(errorDetails));
    }

    if (successCount > 0) {
        emit dataAdded(); // 通知主界面刷新
    }

    QFile file("kadd_log.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        file.write(QString(rebaseNumber)
                       .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                       .toUtf8());
        file.close();
    }

    this->close();
}

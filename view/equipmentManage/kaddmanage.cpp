#include "view/equipmentManage/kaddmanage.h"
#include <QMessageBox>
#include <QDate> // 用于获取当前日期
#include "service/database/databaseManager.h" // 数据库操作类

kaddmanage::kaddmanage(QWidget *parent)
    : QDialog(parent), ui(new view::equipment::Ui::kaddmanage) {
    ui->setupUi(this);
    loadClassrooms();        // 加载“所在教室”下拉框数据（新增）
    initStatusOptions();     // 初始化状态下拉框（原逻辑）
    connect(ui->addButton, &QPushButton::clicked, this, &kaddmanage::on_addButton_clicked);
}

kaddmanage::~kaddmanage() {
    delete ui;
}

void kaddmanage::loadClassrooms() {
    service::DatabaseManager db(data::Equipment::path);
    QString query = "SELECT id, name FROM classroom"; // 假设教室信息存储在 classroom 表
    auto results = db.executeQueryAndFetchAll(query); // 从数据库查询教室数据

    ui->comboBox->clear(); // 清空下拉框原有内容
    ui->comboBox->addItem("请选择所在教室", -1); // 默认提示项（值为-1表示未选择）

    if (results.isEmpty()) {
        // 数据库无教室数据时，添加示例教室（硬编码）
        ui->comboBox->addItem("101实验室", -101); // 示例1：名称+ID（ID设为负数避免冲突）
        ui->comboBox->addItem("202教室", -202);   // 示例2
        ui->comboBox->addItem("303准备室", -303);  // 示例3
    } else {
        // 数据库有数据时，正常加载数据库中的教室
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
    ui->comboBox_2->addItems({"可用", "维修中", "借出"});
    ui->comboBox_2->setCurrentText("可用");
}

// 添加按钮点击事件（核心逻辑）
void kaddmanage::on_addButton_clicked() {
    // 从 UI 控件获取输入（注意转义单引号防 SQL 注入）
    QString instrumentName = ui->equipmentNameEdit->text().trimmed().replace("'", "''"); // 仪器名称（原“仪器名称”输入框）
    QString batchNumber = ui->batchNumberEdit->text().trimmed().replace("'", "''");       // 批次号（原“批次号”输入框）
    QString quantityStr = ui->quantityEdit->text().trimmed();                             // 添加个数（原“添加个数”输入框）
    QString status = ui->comboBox_2->currentText().replace("'", "''");                    // 设备状态（原状态下拉框）
    int classroomId = ui->comboBox->currentData().toInt();                                // 所在教室 ID（新增）
    QString classroomName = ui->comboBox->currentText().replace("'", "''");               // 所在教室名称（用于显示）

    // 输入校验（新增所在教室校验）
    if (instrumentName.isEmpty()) {
        QMessageBox::warning(this, "错误", "仪器名称不能为空！");
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
    if (classroomId == -1) {
        QMessageBox::warning(this, "错误", "请选择所在教室！");
        return;
    }

    // 生成仪器编号（规则：仪器名称+添加时间(年月日)+批次号+序号）
    QDate currentDate = QDate::currentDate();
    QString dateStr = currentDate.toString("yyyyMMdd"); // 格式：20240708
    QString baseNumber = QString("%1_%2_%3").arg(instrumentName).arg(dateStr).arg(batchNumber);

    // 数据库插入（适配单参数 executeNonQuery）
    service::DatabaseManager db(data::Equipment::path);
    db.beginTransaction();
    bool allSuccess = true;
    QString errorDetails;

    for (int i = 0; i < quantity; ++i) {
        // 生成最终仪器编号（序号从_1开始）
        QString finalEquipmentId = (quantity > 1)
                                       ? QString("%1_%2").arg(baseNumber).arg(i + 1)
                                       : baseNumber;

        // 拼接 SQL 插入语句（包含新增的 classroom 字段）
        QString insertQuery = QString(R"(
            INSERT INTO equipment_instance (
                id, name, type, status, batch_number, class_id, in_date
            ) VALUES (
                '%1', '%2', '%3', '%4', '%5', '%6', '%7'
            )
        )").arg(
                                      finalEquipmentId,    // 仪器编号（生成的唯一值）
                                      instrumentName,      // 仪器名称（用户输入）
                                      "待补充类型",         // 注：需根据实际设备类型逻辑填充（如从其他下拉框获取）
                                      status,              // 状态（下拉框选择）
                                      batchNumber,         // 批次号（用户输入）
                                      classroomName,       // 所在教室（下拉框名称，或根据需求存 ID）
                                      dateStr              // 入库时间（年月日，与编号时间一致）
                                      );

        if (!db.executeNonQuery(insertQuery)) {
            allSuccess = false;
            errorDetails += QString("第%1条记录插入失败：%2\n").arg(i + 1).arg(db.getLastError());
            break;
        }
    }

    if (allSuccess) {
        db.commitTransaction();
        QMessageBox::information(this, "成功", QString("已成功添加%1个设备实例！").arg(quantity));
        emit dataAdded(); // 通知主界面刷新
        this->close();
    } else {
        db.rollbackTransaction();
        QMessageBox::critical(this, "失败",
                              QString("添加失败（%1/%2条成功）：\n%3").arg(quantity - errorDetails.count('\n'))
                                  .arg(quantity)
                                  .arg(errorDetails));
    }
}

//
// Created by gouzuang on 25-7-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EquipmentClassDetail.h" resolved

#include "equipmentclassdetail.h"
#include "ui_EquipmentClassDetail.h"
#include "module/data/data_EquipmentManage.h"

namespace view::EquipmentClass {
    EquipmentClassDetail::EquipmentClassDetail(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::EquipmentClassDetail),
        m_maxDescriptionHeight(200),
        m_isEditMode(false),
        m_classId(-1)
    {
        ui->setupUi(this);
        service::style::setMica(this);

        setWindowTitle("新建设备类别");
        ui->UpdateButton->setText("创建");
        ui->UpdateButton->setEnabled(false);
        ui->message->setVisible(false);

        // 清空字段并设置默认值
        ui->EquName->clear();
        ui->EquDescription->clear();
        ui->lineEdit->setText("0");
        ui->TotalNum->setText("0");
        ui->UsableNum->setText("0");
        ui->listWidget->clear();

        data::Equipment::EquipmentClass::recalculateClassCounts(m_classId);


        // 连接信号，当用户输入名称时，检查是否可以创建
        connect(ui->EquName, &QLineEdit::textChanged, this, &EquipmentClassDetail::check_can_create);
    }

    EquipmentClassDetail::EquipmentClassDetail(int classId, QWidget *parent) : QWidget(parent),
                                                                               ui(new Ui::EquipmentClassDetail),
                                                                               m_maxDescriptionHeight(200),m_isEditMode(true), m_classId(classId) {
        ui->setupUi(this);
        service::style::setMica(this);

        ui->UpdateButton->setEnabled(false);
        ui->message->setVisible(false);

        // --- 1. 获取并填充设备类别的基本信息 ---
        data::Equipment::EquipmentClass::EquipmentClassRecord currentClass;
        bool classFound = false;

        currentClass = data::Equipment::EquipmentClass::getEquipmentClassById(classId);
        classFound = (currentClass.id != -1);

        // 如果找到了对应的设备类别，就用它的数据填充UI
        if (classFound) {
            m_originalRecord = currentClass;
            ui->EquName->setText(currentClass.name);
            ui->EquDescription->setText(currentClass.description);
            ui->TotalNum->setText(QString::number(currentClass.total_amount));
            ui->UsableNum->setText(QString::number(currentClass.usable_amount));
            ui->lineEdit->setText(QString::number(currentClass.alarm_amount)); // 对应预警数量的输入框
        } else {
            // 如果没找到，显示错误界面
            ui->EquName->setText("未找到该设备类别");
            ui->EquDescription->clear();
            ui->TotalNum->setText("0");
            ui->UsableNum->setText("0");
            ui->lineEdit->setText("0");
        }

        // --- 2. 获取并填充属于该类别的设备实例列表 ---
        ui->listWidget->clear(); // 先清空列表
        QList<data::Equipment::fullEquipmentRecord> allInstances = data::Equipment::loadFullEquipmentRecords();

        for (const auto &instance: allInstances) {
            if (instance.class_id == classId) {
                // 创建一个格式化的字符串用于显示
                QString itemText = QString("ID: %1 | 名称: %2 | 状态: %3")
                        .arg(instance.id)
                        .arg(instance.name)
                        .arg(instance.status);

                // 将设备实例添加到QListWidget中
                ui->listWidget->addItem(itemText);
            }
        }

        connect(ui->EquName, &QLineEdit::textChanged, this, &EquipmentClassDetail::check_for_modifications);
        connect(ui->EquDescription, &QTextEdit::textChanged, this, &EquipmentClassDetail::check_for_modifications);
        connect(ui->lineEdit, &QLineEdit::textChanged, this, &EquipmentClassDetail::check_for_modifications);
    }

    EquipmentClassDetail::~EquipmentClassDetail() {
        delete ui;
    }

    void EquipmentClassDetail::showEvent(QShowEvent *event) {
        QWidget::showEvent(event);
        adjustEquDescriptionHeight();
    }

    void EquipmentClassDetail::on_EquDescription_textChanged() {

    }

    void EquipmentClassDetail::adjustEquDescriptionHeight() {
        int minHeight = ui->EquDescription->fontMetrics().height() + (ui->EquDescription->frameWidth() * 2) + 2; // 加一点余量

        // 计算当前内容所需的高度
        qreal contentHeight = ui->EquDescription->document()->size().height();
        int targetHeight = static_cast<int>(contentHeight) + (ui->EquDescription->frameWidth() * 2);

        // 确保 targetHeight 至少是单行的高度
        targetHeight = qMax(minHeight, targetHeight);

        if (targetHeight > m_maxDescriptionHeight)
        {
            ui->EquDescription->setFixedHeight(m_maxDescriptionHeight);
            ui->EquDescription->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else
        {
            ui->EquDescription->setFixedHeight(targetHeight);
            ui->EquDescription->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    void EquipmentClassDetail::on_UpdateButton_clicked() {
        if (m_isEditMode) {
            data::Equipment::EquipmentClass::EquipmentClassRecord recordToUpdate = m_originalRecord;
            recordToUpdate.name = ui->EquName->text();
            recordToUpdate.description = ui->EquDescription->toPlainText();
            recordToUpdate.alarm_amount = ui->lineEdit->text().toInt();

            if (!data::Equipment::EquipmentClass::updateEquipmentClass(recordToUpdate)) {
                show_message("更新设备信息失败，请重试。");
                return;
            }
            if (!data::Equipment::EquipmentClass::recalculateClassCounts(m_originalRecord.id)) {
                show_message("设备信息已更新，但刷新数量统计时失败。");
            }
            data::Equipment::EquipmentClass::EquipmentClassRecord refreshedRecord =
                data::Equipment::EquipmentClass::getEquipmentClassById(m_originalRecord.id);

            if (refreshedRecord.id != -1) {
                m_originalRecord = refreshedRecord;
                ui->EquName->setText(m_originalRecord.name);
                ui->EquDescription->setText(m_originalRecord.description);
                ui->lineEdit->setText(QString::number(m_originalRecord.alarm_amount));
                ui->TotalNum->setText(QString::number(m_originalRecord.total_amount));

                ui->UsableNum->setText(QString::number(m_originalRecord.usable_amount));
                m_originalRecord = refreshedRecord;
                show_message("信息更新与数量统计均已完成。");
            } else {
                show_message("数据已尝试更新，但在刷新界面时无法重新获取数据。请手动确认更改已完成。");
            }
            ui->UpdateButton->setEnabled(false);

        } else {
            data::Equipment::EquipmentClass::EquipmentClassRecord newRecord;
            newRecord.name = ui->EquName->text();
            newRecord.description = ui->EquDescription->toPlainText();
            newRecord.alarm_amount = ui->lineEdit->text().toInt();

            int newId = data::Equipment::EquipmentClass::addEquipmentClass(newRecord);

            if (newId != -1) {
                show_message(QString("新的设备类别“%1”已成功创建！").arg(newRecord.name));
                m_classId = newId;
                m_isEditMode = true;
                m_originalRecord = newRecord;
                ui->UpdateButton->setText("更新");
                ui->UpdateButton->setEnabled(false);
                disconnect(ui->EquName, &QLineEdit::textChanged, this, &EquipmentClassDetail::check_can_create);
                connect(ui->EquName, &QLineEdit::textChanged, this, &EquipmentClassDetail::check_for_modifications);
                connect(ui->EquDescription, &QTextEdit::textChanged, this, &EquipmentClassDetail::check_for_modifications);
                connect(ui->lineEdit, &QLineEdit::textChanged, this, &EquipmentClassDetail::check_for_modifications);
            } else {
                show_message("创建新类别失败，请检查名称是否重复或联系管理员。");
            }
        }
    }

    void EquipmentClassDetail::check_for_modifications() {
        // 比较当前UI的值和存储的原始值
        bool nameModified = ui->EquName->text() != m_originalRecord.name;
        bool descriptionModified = ui->EquDescription->toPlainText() != m_originalRecord.description;
        bool alarmModified = ui->lineEdit->text().toInt() != m_originalRecord.alarm_amount;

        // 只要有任何一项被修改，就启用按钮
        ui->UpdateButton->setEnabled(nameModified || descriptionModified || alarmModified);
    }

    void EquipmentClassDetail::check_can_create() {
        ui->UpdateButton->setEnabled(!ui->EquName->text().trimmed().isEmpty());
    }

    void EquipmentClassDetail::show_message(QString message, int timeout) {
        ui->message->setText(message);
        ui->message->setVisible(true);
        QTimer::singleShot(timeout, this, [this]() {
            ui->message->setVisible(false);
        });
    }
} // view::EquipmentClass

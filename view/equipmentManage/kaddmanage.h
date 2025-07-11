#ifndef KADDMANAGE_H
#define KADDMANAGE_H

#include <QDialog>
#include "module/data/data_EquipmentManage.h" // 包含数据层头文件
#include "ui_kaddmanage.h"        // 包含生成的UI头文件
#include <QComboBox>

namespace view::equipment::Ui {
class kaddmanage; // 与 ui_kaddmanage.h 命名空间对齐
}

class kaddmanage : public QDialog {
    Q_OBJECT

public:
    explicit kaddmanage(QWidget *parent = nullptr);
    ~kaddmanage() override;

signals:
    void dataAdded(); // 通知主界面刷新设备列表

private slots:
    void on_addButton_clicked(); // 绑定UI中的 "添加" 按钮

private:
    view::equipment::Ui::kaddmanage *ui;// UI指针（命名空间对齐）
    void extracted(QList<QVariantMap> &results);
    void loadEquipmentClasses();         // 从数据库加载设备类别到下拉框
    void initStatusOptions();        // 初始化设备状态下拉框选项
    void loadClassrooms();           //加载设备类
};

#endif // KADDMANAGE_H

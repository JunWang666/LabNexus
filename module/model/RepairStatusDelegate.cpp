//
// Created by Nana7mi_ on 25-7-9.
//

#include "RepairStatusDelegate.h"

namespace delegateModel {
    RepairStatusDelegate::RepairStatusDelegate(QObject *parent) {

    }

    QWidget * RepairStatusDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const {
        Q_UNUSED(option);
        Q_UNUSED(index);

        // 创建一个 QComboBox
        auto *editor = new QComboBox(parent);
        // 添加 "可用" 和 "报修" 选项
        editor->addItem("可用");
        editor->addItem("报修");

        return editor;
    }

    void RepairStatusDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
        // 从模型获取当前设备状态
        QString currentStatus = index.model()->data(index, Qt::EditRole).toString();

        // 将编辑器转换为 QComboBox 并设置当前文本
        auto *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(currentStatus);
    }

    void RepairStatusDelegate::
    setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
        // 将编辑器转换为 QComboBox
        auto *comboBox = static_cast<QComboBox*>(editor);
        // 获取用户选择的值
        QString value = comboBox->currentText();

        // 调用模型的 setData 函数更新数据。
        // 这将触发 EquipmentDataModel 中的逻辑，进而更新数据库。
        model->setData(index, value, Qt::EditRole);
    }

    void RepairStatusDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
        const QModelIndex &index) const {
        Q_UNUSED(index);
        // 设置编辑器的位置和大小，使其填充整个单元格
        editor->setGeometry(option.rect);
    }
} // delegateModel
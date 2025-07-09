//
// Created by Nana7mi_ on 25-7-9.
//

#include "ApprovalStatusDelegate.h"

namespace delegateModel {
    ApprovalStatusDelegate::ApprovalStatusDelegate(QObject *parent)
    : QItemDelegate(parent) {
        // 在此定义审批状态的选项
        m_statusOptions << "待审批" << "同意" << "拒绝";
    }

    QWidget * ApprovalStatusDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const {
        // 创建一个 QComboBox
        auto *editor = new QComboBox(parent);
        // 添加选项
        editor->addItems(m_statusOptions);
        return editor;
    }

    void ApprovalStatusDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
        // 从模型获取当前值
        QString currentStatus = index.model()->data(index, Qt::EditRole).toString();

        // 将编辑器转换为 QComboBox 并设置当前文本
        auto *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(currentStatus);
    }

    void ApprovalStatusDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const {
        // 将编辑器转换为 QComboBox
        auto *comboBox = static_cast<QComboBox*>(editor);
        // 获取用户选择的值
        QString value = comboBox->currentText();

        // 调用模型的 setData 函数更新数据。
        // 您在 BookingDataModel.cpp 中实现的 setData 逻辑将在此被触发，从而同步到数据库。
        model->setData(index, value, Qt::EditRole);
    }

    void ApprovalStatusDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
        const QModelIndex &index) const {
        Q_UNUSED(index)
        // 设置编辑器的位置和大小，使其填充整个单元格
        editor->setGeometry(option.rect);
    }
} // delegateModel
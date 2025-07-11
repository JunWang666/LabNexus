//
// Created by Nana7mi_ on 25-7-9.
//

#ifndef REPAIRSTATUSDELEGATE_H
#define REPAIRSTATUSDELEGATE_H
#include <QItemDelegate>
#include <QComboBox>
namespace delegateModel {

    class RepairStatusDelegate : public QItemDelegate {
        Q_OBJECT

    public:
        explicit RepairStatusDelegate(QObject *parent = nullptr);

        // 创建编辑器
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

        // 设置编辑器数据
        void setEditorData(QWidget *editor, const QModelIndex &index) const override;

        // 将数据写回模型
        void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const override;

        // 更新编辑器布局
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const override;
    };

} // delegateModel

#endif //REPAIRSTATUSDELEGATE_H

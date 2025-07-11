//
// Created by Nana7mi_ on 25-7-9.
//

#ifndef APPROVALSTATUSDELEGATE_H
#define APPROVALSTATUSDELEGATE_H
#include <QItemDelegate>
#include <QComboBox>
namespace delegateModel {

    class ApprovalStatusDelegate : public QItemDelegate {
        Q_OBJECT

    public:
        explicit ApprovalStatusDelegate(QObject *parent = nullptr);

        // 当双击单元格时，为此单元格创建编辑器
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

        // 将模型的数据设置到编辑器中
        void setEditorData(QWidget *editor, const QModelIndex &index) const override;

        // 当编辑完成后，将编辑器的数据写回到模型中
        void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const override;

        // 更新编辑器的几何布局，确保它填满整个单元格
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const override;

    private:
        QStringList m_statusOptions; // 存储选项列表，使其更具可配置性
    };

} // delegateModel

#endif //APPROVALSTATUSDELEGATE_H

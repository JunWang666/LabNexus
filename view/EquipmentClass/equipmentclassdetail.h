//
// Created by gouzuang on 25-7-12.
//

#ifndef EQUIPMENTCLASSDETAIL_H
#define EQUIPMENTCLASSDETAIL_H

#include <QWidget>

#include "module/data/data_EquipmentManage.h"

namespace view::EquipmentClass {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class EquipmentClassDetail;
    }

    QT_END_NAMESPACE

    class EquipmentClassDetail : public QWidget {
        Q_OBJECT

    public:
        explicit EquipmentClassDetail(QWidget *parent = nullptr);

        explicit EquipmentClassDetail(int classId, QWidget *parent = nullptr);

        ~EquipmentClassDetail() override;

    protected:
        void showEvent(QShowEvent *event) override;

    public slots:
        void on_EquDescription_textChanged();

        void adjustEquDescriptionHeight();

        /**
         * @brief 当用户点击“更新”按钮时调用。
         */
        void on_UpdateButton_clicked();

        /**
         * @brief 当输入框内容变化时调用，检查是否需要启用更新按钮。
         */
        void check_for_modifications();

        /**
         * @brief 新建模式下，检查是否可以点击创建按钮
         */
        void check_can_create();

    private:
        Ui::EquipmentClassDetail *ui;
        bool m_isEditMode; // true为编辑模式, false为新建模式
        int m_classId; // 存储当前页面的类别ID
        data::Equipment::EquipmentClass::EquipmentClassRecord m_originalRecord;
        int m_maxDescriptionHeight{};
    };
} // view::EquipmentClass

#endif //EQUIPMENTCLASSDETAIL_H

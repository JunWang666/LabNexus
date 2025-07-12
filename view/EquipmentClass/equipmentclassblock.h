//
// Created by gouzuang on 25-7-12.
//

#ifndef EQUIPMENTCLASSBLOCK_H
#define EQUIPMENTCLASSBLOCK_H

#include <QWidget>

#include "equipmentclassdetail.h"
#include "module/data/data_EquipmentManage.h"

namespace view::EquipmentClass {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class EquipmentClassBlock;
    }

    QT_END_NAMESPACE

    class EquipmentClassBlock : public QWidget {
        Q_OBJECT

    public:
        explicit EquipmentClassBlock(QWidget *parent = nullptr);

        explicit EquipmentClassBlock(const data::Equipment::EquipmentClass::EquipmentClassRecord &record,
                                     QWidget *parent = nullptr);

        ~EquipmentClassBlock() override;

    public slots:
        void on_frame_clicked();

    private:
        Ui::EquipmentClassBlock *ui;
        data::Equipment::EquipmentClass::EquipmentClassRecord record;
    };
} // view::EquipmentClass

#endif //EQUIPMENTCLASSBLOCK_H

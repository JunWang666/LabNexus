//
// Created by gouzuang on 25-7-12.
//

#ifndef EQUIPMENTINSTANCEBUTTON_H
#define EQUIPMENTINSTANCEBUTTON_H

#include <QWidget>

namespace view::EquipmentClass {
QT_BEGIN_NAMESPACE
namespace Ui { class EquipmentInstanceButton; }
QT_END_NAMESPACE

class EquipmentInstanceButton : public QWidget {
Q_OBJECT

public:
    explicit EquipmentInstanceButton(QWidget *parent = nullptr);
    ~EquipmentInstanceButton() override;

private:
    Ui::EquipmentInstanceButton *ui;
};
} // view::EquipmentClass

#endif //EQUIPMENTINSTANCEBUTTON_H

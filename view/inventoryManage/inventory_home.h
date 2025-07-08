//
// Created by gouzuang on 25-7-6.
//

#ifndef INVENTORY_HOME_H
#define INVENTORY_HOME_H

#include "pch.h"

namespace view::inventory {
QT_BEGIN_NAMESPACE
namespace Ui { class inventory_home; }
QT_END_NAMESPACE

class inventory_home : public QWidget {
Q_OBJECT

public:
    explicit inventory_home(QWidget *parent = nullptr);
    ~inventory_home() override;

private:
    Ui::inventory_home *ui;
};
} // view::inventory

#endif //INVENTORY_HOME_H

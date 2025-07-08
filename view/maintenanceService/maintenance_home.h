//
// Created by gouzuang on 25-7-6.
//

#ifndef MAINTENANCE_HOME_H
#define MAINTENANCE_HOME_H

#include "pch.h"

namespace view::maintenance {
QT_BEGIN_NAMESPACE
namespace Ui { class maintenance_home; }
QT_END_NAMESPACE

class maintenance_home : public QWidget {
Q_OBJECT

public:
    explicit maintenance_home(QWidget *parent = nullptr);
    ~maintenance_home() override;

private:
    Ui::maintenance_home *ui;
};
} // view::maintenance

#endif //MAINTENANCE_HOME_H

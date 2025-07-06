//
// Created by gouzuang on 25-7-6.
//

#ifndef EQUIPMENT_HOME_H
#define EQUIPMENT_HOME_H

#include <QWidget>

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class equipment_home; }
QT_END_NAMESPACE

class equipment_home : public QWidget {
Q_OBJECT

public:
    explicit equipment_home(QWidget *parent = nullptr);
    ~equipment_home() override;

private:
    Ui::equipment_home *ui;
};
} // view::equipment

#endif //EQUIPMENT_HOME_H

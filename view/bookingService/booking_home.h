//
// Created by gouzuang on 25-7-6.
//

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace view::booking {
QT_BEGIN_NAMESPACE
namespace Ui { class booking_home; }
QT_END_NAMESPACE

class booking_home : public QWidget {
Q_OBJECT

public:
    explicit booking_home(QWidget *parent = nullptr);
    ~booking_home() override;

private:
    Ui::booking_home *ui;
};
} // view::booking

#endif //HOMEPAGE_H

//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef RENTTEACHER_H
#define RENTTEACHER_H

#include <QWidget>
#include "apply.h"
#include "sendrent.h"
namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class RentTeacher; }
QT_END_NAMESPACE

class RentTeacher : public QWidget {
Q_OBJECT

public:
    explicit RentTeacher(QWidget *parent = nullptr);
    ~RentTeacher() override;
public slots:
    void on_btnsend_clicked();
    void on_btncheck_clicked();
private:
    Ui::RentTeacher *ui;
    SendRent* sendRent;
    Apply* apply;
};
} // view::Order

#endif //RENTTEACHER_H

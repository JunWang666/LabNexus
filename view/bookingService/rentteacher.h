//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef RENTTEACHER_H
#define RENTTEACHER_H

#include <QWidget>
#include "apply.h"
#include "sendrent.h"
#include <QStandardItemModel>
#include <QStandardItem>
namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class RentTeacher; }
QT_END_NAMESPACE

class RentTeacher : public QWidget {
Q_OBJECT

public:
    explicit RentTeacher(QWidget *parent = nullptr);
    ~RentTeacher() override;
    void loadData();
    void setUpModel_device();
    void setUpModel_request();
    void setColEditable(QStandardItemModel *model,int col,bool editable);
public slots:
    void on_btnSend_clicked();
    void on_btnCheck_clicked();
private:
    Ui::RentTeacher *ui;
    SendRent* sendRent;
    Apply* apply;
    QStandardItemModel* modelDevice;
    QStandardItemModel* modelRequest;
};
} // view::Order

#endif //RENTTEACHER_H

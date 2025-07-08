//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef APPLY_H
#define APPLY_H

#include <QWidget>
#include "module/model/BookingDataModel.h"
namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class Apply; }
QT_END_NAMESPACE

class Apply : public QWidget {
Q_OBJECT

public:
    explicit Apply(QWidget *parent = nullptr);
    ~Apply() override;
    void loadData();
    void setUpModel();
public slots:
    void on_btnClose_clicked();
private:
    Ui::Apply *ui;
    dataModel::BookingDataModel *model;
};
} // view::Order

#endif //APPLY_H

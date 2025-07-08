//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef MANAGERCHECK_H
#define MANAGERCHECK_H

#include <QWidget>
#include "module/model/BookingDataModel.h"
#include "module/model/filterproxymdel.h"

namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class ManagerCheck; }
QT_END_NAMESPACE

class ManagerCheck : public QWidget {
Q_OBJECT

public:
    explicit ManagerCheck(QWidget *parent = nullptr);
    ~ManagerCheck() override;
    void loadData();
    void setUpModel();
    // void setColEditable(QStandardItemModel *model,int col,bool editable);
public slots:
    void on_btnClose_clicked();
private:
    Ui::ManagerCheck *ui;
    dataModel::BookingDataModel *model;
    fliterModel::FilterProxyMdel * fliterModel;
};
} // view::Order

#endif //MANAGERCHECK_H

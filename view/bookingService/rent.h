//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef RENT_H
#define RENT_H

#include <QWidget>

#include "apply.h"
#include "sendrent.h"
#include "module/model/EquipmentDataModel.h"
#include "module/model/filterproxymdel.h"
namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class Rent; }
QT_END_NAMESPACE

class Rent : public QWidget {
Q_OBJECT

public:
    enum Column {
        Col_Rent,
        Col_Repair,
        Col_Return
    };
    explicit Rent(QWidget *parent = nullptr);
    explicit Rent(const QString &name,const QString&id,QWidget *parent = nullptr);
    ~Rent() override;
    void loadData();
    void setUpModel();
    // void setColEditable(QStandardItemModel *model,int col,bool editable);
public slots:
    void on_btnSend_clicked();
    void on_btnCheck_clicked();
private:
    Ui::Rent *ui;
    SendRent* sendRent;
    Apply* checkApply;
    dataModel::EquipmentDataModel* modelRent;
    dataModel::EquipmentDataModel* modelRepair;
    dataModel::EquipmentDataModel* modelReturn;
    fliterModel::FilterProxyMdel* rentFilterProxyMdel;
    fliterModel::FilterProxyMdel* repairFilterProxyMdel;
    fliterModel::FilterProxyMdel* returnFilterProxyMdel;
    QString name;
    QString id;
};
} // view::Order

#endif //RENT_H

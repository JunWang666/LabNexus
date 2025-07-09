//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef RENTTEACHER_H
#define RENTTEACHER_H

#include <QWidget>
#include <QString>
#include "apply.h"
#include "sendrent.h"
#include "module/model/EquipmentDataModel.h"
#include "module/model/BookingDataModel.h"
#include <QListWidget>
#include "module/model/filterproxymdel.h"
#include "module/model/ApprovalStatusDelegate.h"
#include "module/model/RepairStatusDelegate.h"
namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class RentTeacher; }
QT_END_NAMESPACE

class RentTeacher : public QWidget {
Q_OBJECT

public:
    enum Column {
        Col_Rent,
        Col_Check,
        Col_Repair,
        Col_Return
    };

    explicit RentTeacher(QWidget *parent = nullptr);
    explicit RentTeacher(const QString& name,const QString& id,QWidget *parent = nullptr);
    ~RentTeacher() override;
    void loadData();
    void setUpModel_device();
    void setUpModel_request();
    void setUpModel_repair();
    void setUpModel_return();
    // void setColEditable(QStandardItemModel *model,int col,bool editable);
    void setIndex(int row);
public slots:
    void on_btnSend_clicked();
    void on_btnCheck_clicked();
private:
    Ui::RentTeacher *ui;
    SendRent* sendRent;
    Apply* apply;
    dataModel::EquipmentDataModel* modelDevice;
    dataModel::BookingDataModel* modelRequest;
    dataModel::EquipmentDataModel* modelRepair;
    dataModel::EquipmentDataModel* modelReturn;
    fliterModel::FilterProxyMdel* deviceFilterProxyMdel;
    fliterModel::FilterProxyMdel* requestFilterProxyMdel;
    fliterModel::FilterProxyMdel* repairFilterProxyMdel;
    fliterModel::FilterProxyMdel* returnFilterProxyMdel;
    QString name;
    QString id;
};
} // view::Order

#endif //RENTTEACHER_H

//
// Created by Nana7mi_ on 25-7-7.
//

#ifndef APPLY_H
#define APPLY_H

#include <QWidget>
#include "module/model/BookingDataModel.h"
#include "module/model/filterproxymdel.h"

namespace view::Order {
QT_BEGIN_NAMESPACE
namespace Ui { class Apply; }
QT_END_NAMESPACE

class Apply : public QWidget {
Q_OBJECT

public:
    explicit Apply(QWidget *parent = nullptr);
    explicit Apply(const QString & name,const QString & id,QWidget *parent = nullptr);
    ~Apply() override;
    void loadData();
    void setUpModel();
public slots:
    void on_btnClose_clicked();
private:
    Ui::Apply *ui;
    dataModel::BookingDataModel *model;
    QString name;
    QString id;
    fliterModel::FilterProxyMdel * fliterModel;
};
} // view::Order

#endif //APPLY_H

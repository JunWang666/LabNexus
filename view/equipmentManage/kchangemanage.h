#ifndef KCHANGEMANAGE_H
#define KCHANGEMANAGE_H

#include "module/model/EquipmentDataModel.h"
#include "module/model/filterproxymdel.h"
#include "pch.h"

namespace view::equipment {
QT_BEGIN_NAMESPACE
namespace Ui { class kchangemanage; }
QT_END_NAMESPACE

}

namespace view::equipment {

class kchangemanage : public QDialog {
    Q_OBJECT

public:
    explicit kchangemanage(QWidget *parent = nullptr);
    void setModel(QAbstractItemModel *model); // 声明setModel
    ~kchangemanage() override;

signals:
    void dataChanged(); // 通知主界面刷新设备列表

private slots:
    void on_deleteButton_clicked();

    void on_changeButton_clicked();

    void on_statusButton_clicked();

private:

    Ui::kchangemanage *ui;
    dataModel::EquipmentDataModel *modelRent ;
    fliterModel::FilterProxyMdel *rentFilterProxyMdel;

};

} // view::equipment

#endif // KCHANGEMANAGE_H

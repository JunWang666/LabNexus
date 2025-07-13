//
// Created by gouzuang on 25-7-7.
//

#ifndef EQUIPMENT_HOME_H
#define EQUIPMENT_HOME_H

#include <QWidget>
#include "module/data/data_EquipmentManage.h"
#include "module/model/EquipmentDataModel.h"
#include "view/bookingService/managercheck.h"

namespace view::equipment {

QT_BEGIN_NAMESPACE

namespace Ui {
class equipment_home;
}

QT_END_NAMESPACE
}

namespace view::equipment {
class equipment_home : public QWidget {
    Q_OBJECT

public:
    explicit equipment_home(QWidget *parent = nullptr);
    ~equipment_home() override;

private slots:
    void on_zclose_clicked();       // 关闭应用
    void on_kadd_clicked();         // 添加器材
    void on_kreall_clicked();       // 整理库数据
    void on_kchange_clicked();      // 修改库数据
    void on_kdel_clicked();         // 删除库数据
    void on_kexam_clicked();        // 检查和审批审查
    void on_kseeborrow_clicked();   // 查看借用日志
    void on_seefix_clicked();       // 查看维修日志
    void on_outdata_clicked();      // 超期预警
    void on_blacklist_clicked();    // 查看黑名单
    void on_searchbtn_clicked();    // 搜索功能

    void on_zreturn_clicked();

private:
    view::equipment::Ui::equipment_home *ui;
    dataModel::EquipmentDataModel *modelRent;        // 数据模型
    fliterModel::FilterProxyMdel *rentFilterProxyMdel;
};

} // view::equipment


#endif // EQUIPMENT_HOME_H

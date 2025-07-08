//
// Created by gouzuang on 25-7-6.
//

#include "equipment_home.h"
#include "ui_equipment_home.h"
#include "kaddmanage.h"
#include "kchangemanage.h"
#include "kdeletemanage.h"
#include "applysee.h"
#include "exam.h"

namespace view::equipment {
    equipment_home::equipment_home(QWidget *parent) : QWidget(parent), ui(new view::equipment::Ui::equipment_home) {
        ui->setupUi(this);
    }

    equipment_home::~equipment_home() {
        delete ui;
    }

    void equipment_home::on_toapply_clicked() //预约申请
    {
        applysee *dialog = new applysee(this);
        dialog->show();
    }

    void equipment_home::on_stuexam_clicked() //老师用，用于学生材料递送
    {
        exam *dialog = new exam(this);
        dialog->show();
    }

    void equipment_home::on_adfix_clicked() //添加报修
    {
        // TODO: 实现添加报修功能
    }

    void equipment_home::on_zreturn_clicked() //回到主页
    {
        this->close();
    }

    void equipment_home::on_zclose_clicked() //关闭应用
    {
        this->close();
    }

    void equipment_home::on_kadd_clicked() //添加器材
    {
        kaddmanage *dialog = new kaddmanage(this);
        dialog->show();
    }

    void equipment_home::on_kreall_clicked() //整理库数据
    {
        // TODO: 实现整理库数据功能
    }

    void equipment_home::on_kchange_clicked() //修改库数据
    {
        kchangemanage *dialog = new kchangemanage(this);
        dialog->show();
    }

    void equipment_home::on_kdel_clicked() //删除库数据
    {
        kdeletemanage *dialog = new kdeletemanage(this);
        dialog->show();
    }

    void equipment_home::on_kexam_clicked() //检查和审批审查
    {
        // TODO: 实现检查和审批审查功能
    }

    void equipment_home::on_kseeborrow_clicked() //看查借用日志
    {
        // TODO: 实现查看借用日志功能
    }

    void equipment_home::on_seefix_clicked() //看查维修日志
    {
        // TODO: 实现查看维修日志功能
    }

    void equipment_home::on_outdata_clicked() //超期预警
    {
        // TODO: 实现超期预警功能
    }

    void equipment_home::on_blacklist_clicked() //访问黑名单
    {
        // TODO: 实现访问黑名单功能
    }

    void equipment_home::on_searchbtn_clicked() //索引
    {
        // TODO: 实现搜索功能
    }
} // view::equipment

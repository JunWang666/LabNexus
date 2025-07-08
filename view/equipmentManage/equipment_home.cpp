//
// Created by gouzuang on 25-7-6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_equipment_home.h" resolved

#include "equipment_home.h"
#include "ui_equipment_home.h"


namespace view::equipment {
equipment_home::equipment_home(QWidget *parent) :
    QWidget(parent), ui(new Ui::equipment_home) {
    ui->setupUi(this);
}

equipment_home::~equipment_home() {
    delete ui;
}

void equipment_home::on_toapply_clicked()//预约申请
{

}

void equipment_home::on_stuexam_clicked()//老师用，用于学生材料递送
{

}

void equipment_home::on_adfix_clicked()//添加报修
{

}

void equipment_home::on_zreturn_clicked()//回到主页
{

}

void equipment_home::on_zclose_clicked()//关闭应用
{

}



//对管理员的功能按钮

void equipment_home::on_kadd_clicked()//添加器材
{

}

void equipment_home::on_kreall_clicked()//整理库数据
{

}

void equipment_home::on_kchange_clicked()//修改库数据
{
    view::equipment::kchangemanage equipmentHome;
    equipmentHome.show();
}

void equipment_home::on_kdel_clicked()//删除库数据
{

}

void equipment_home::on_kexam_clicked()//检查和审批审查
{

}

void equipment_home::on_kseeborrow_clicked()//看查借用日志
{

}

void equipment_home::on_seefix_clicked()//看查维修日志
{

}

void equipment_home::on_outdata_clicked()//超期预警
{

}

void equipment_home::on_blacklist_clicked()//访问黑名单
{

}

//库查询
void equipment_home::on_searchbtn_clicked()//索引
{

}

} // view::equipment

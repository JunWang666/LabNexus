#include "pch.h"
#include "module/data/data_Booking.h"
#include <QTableView>
#include "module/model/BookingDataModel.h"
#include "view/homepage/teacherhomepage.h"
#include "view/loginPage/loginpage.h"
#include "view/loginPage/registerpage.h"
#include "view/equipmentManage/equipment_home.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 初始化并测试日志输出
    service::logger::instance().setLogFile(
        QString("app_%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")).toStdString());
    service::logger::instance().setDataLogFile("data.log");
    service::log() << "程序启动";

    data::UserControl::buildDB();
    data::Booking::buildDB();

    view::equipment::equipment_home equipmentHome;
    equipmentHome.show();

    // 测试Booking表格视图
    // auto *bookingModel = new dataModel::BookingDataModel(nullptr);
    // QTableView *tableView = new QTableView;
    // tableView->setModel(bookingModel);
    // tableView->setWindowTitle("Booking Records");
    // tableView->resize(800, 400);
    // tableView->show();

    view::login::loginPage b;
    b.show();

    return QApplication::exec();
}

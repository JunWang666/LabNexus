// main.cpp

#include "pch.h"
#include "module/data/data_Booking.h"
#include <QTableView>
#include "module/data/data_mail.h"
#include "module/model/BookingDataModel.h"
#include "view/bookingService/booking_home.h"
#include "view/bookingService/rent.h"
#include "view/homepage/teacherhomepage.h"
#include "view/loginPage/loginpage.h"
#include "view/loginPage/registerpage.h"
#include "view/messageCenter/messagewindow.h"
#include "view/equipmentManage/equipment_home.h"


void setup_tasks() {
    // 后台定时每1分钟扫描库存告警
    service::taskManager::getTimer().scheduleTask(60000, []() {
        bot::InventoryAlert::sendAlert();
    });
}

void initDB() {
    data::mail::buildDB();
    data::UserControl::buildDB();
    // 创建用户组
    if (auto r = data::UserControl::permission::createGroup("Student", "学生组"); !r) {
        log(LogLevel::ERR) << "创建组 Student 失败, 错误码:" << static_cast<int>(r.error());
    }
    if (auto r = data::UserControl::permission::createGroup("Teacher", "教师组"); !r) {
        log(LogLevel::ERR) << "创建组 Teacher 失败, 错误码:" << static_cast<int>(r.error());
    }
    if (auto r = data::UserControl::permission::createGroup("Admin", "系统管理员组"); !r) {
        log(LogLevel::ERR) << "创建组 Admin 失败, 错误码:" << static_cast<int>(r.error());
    }
    if (auto r = data::UserControl::permission::createGroup("System", "系统预留账号"); !r) {
        log(LogLevel::ERR) << "创建组 System 失败, 错误码:" << static_cast<int>(r.error());
    }
    data::mail::registerSystemUser();
    data::UserControl::Login::createNewUser("0","Admin","Admin",
                                            data::UserControl::permission::searchGroupIdByName("Admin").first());
    data::Booking::buildDB();
    data::Equipment::buildDB();
    data::mail::findSystemUser();
}

int main(int argc, char *argv[]) {
    service::logger::instance().setLogFile(
        QString("log/app_%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")).toStdString());
    service::logger::instance().setDataLogFile("log/data.log");
    service::logger::instance().setLevel(LogLevel::WARN);
    service::logger::instance().enableStackTrace(false);
    service::log() << "程序启动";

    QApplication a(argc, argv);

    initDB();

    // QFile styleFile(":/styles/fluent.qss");
    // if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
    //     QString styleSheet = QLatin1String(styleFile.readAll());
    //     a.setStyleSheet(styleSheet);
    //     styleFile.close();
    // } else {
    //     log(LogLevel::ERR) << "无法加载样式表文件: " << styleFile.fileName();
    // }

    view::login::loginPage b;

    b.show();

    setup_tasks();
    service::taskManager::getTimer().startAll();

    return QApplication::exec();
}

// main.cpp

#include "pch.h"
#include "module/data/data_Booking.h"
#include <QTableView>
#include <qtimezone.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "module/data/data_mail.h"
#include "module/model/BookingDataModel.h"
#include "service/stastic/sharedFunctions.h"
#include "view/bookingService/booking_home.h"
#include "view/bookingService/rent.h"
#include "view/EquipmentClass/equipmentclassblock.h"
#include "view/EquipmentClass/equipmentclassmanagehomepage.h"
#include "view/homepage/teacherhomepage.h"
#include "view/loginPage/loginpage.h"
#include "view/loginPage/registerpage.h"
#include "view/messageCenter/messagewindow.h"
#include "view/equipmentManage/equipment_home.h"
#include "view/RegisterCenter/checknewuser.h"
#include "view/SplashScreen/nativesplash.h"
//#include "view/SplashScreen/splashscreen.h"
#include "module/scanner/WorkAlert.h"

void setup_tasks() {
    // 后台定时每1分钟扫描库存告警
    service::taskManager::getTimer().scheduleTask(60000, []() {
        bot::InventoryAlert::sendAlert();
        bot::WorkAlert::sendAdminCheckNewUserAlert();
    });
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    qInstallMessageHandler(service::customMessageHandler); //拦截部分错误，真的修不好了喵眼不见心不烦

    view::SplashScreen::nativeSplash splash;
    splash.show();


    service::logger::instance().setLogFile(
        QString("log/app_%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")).toStdString());
    service::logger::instance().setDataLogFile("log/data.log");
    service::logger::instance().setLevel(LogLevel::WARN);
    service::logger::instance().enableStackTrace(false);
    service::log() << "程序启动";

    service::initDB();



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

    splash.closeSplash();

    return QApplication::exec();
}

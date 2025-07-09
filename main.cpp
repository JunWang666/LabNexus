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
#ifdef Q_OS_WIN
#include <windows.h>
#include <WinUser.h>
#include "assets/style/blurredTranslucent.h"
#endif


void applyBlurEffect(HWND hwnd)
{
#ifdef Q_OS_WIN
    // 确保这里的 HMODULE 和函数指针定义也在 #ifdef Q_OS_WIN 内部
    using pfnSetWindowCompositionAttribute = BOOL(WINAPI *)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

    HMODULE hUser = GetModuleHandle(L"user32.dll");
    if (hUser)
    {
        auto setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
        if (setWindowCompositionAttribute)
        {
            ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
            WINDOWCOMPOSITIONATTRIBDATA data;
            data.Attrib = WCA_ACCENT_POLICY;
            data.pvData = &accent;
            data.cbData = sizeof(accent);
            setWindowCompositionAttribute(hwnd, &data);
        }
    }
    log(LogLevel::INFO)<< "应用模糊效果成功";
#endif
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    service::logger::instance().setLogFile(
        QString("app_%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")).toStdString());
    service::logger::instance().setDataLogFile("data.log");
    service::log() << "程序启动";

    data::UserControl::buildDB();
    data::Booking::buildDB();
    data::mail::buildDB();
    data::Equipment::buildDB();

    QFile styleFile(":/styles/fluent.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    } else {
        log(LogLevel::ERR)<< "无法加载样式表文件: " << styleFile.fileName();
    }

    data::UserControl::currentUserId = 2;
    view::messageCenter::MessageWindow b;

#ifdef Q_OS_WIN
      // 从窗口实例 b 获取句柄，而不是用 this
      HWND hwnd = (HWND)b.winId();
      //applyBlurEffect(hwnd);
#endif
    //b.setAttribute(Qt::WA_NoSystemBackground);
    // 显示窗口
    b.show();

    return QApplication::exec();
}
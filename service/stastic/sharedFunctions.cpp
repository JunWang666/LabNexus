//
// Created by gouzuang on 25-7-11.
//
#include "pch.h"
#include "sharedFunctions.h"

#include "module/data/data_Booking.h"
#include "module/data/data_mail.h"

namespace service {
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
        auto id = data::UserControl::Login::createNewUser("0","Admin","Admin",
                                                data::UserControl::permission::searchGroupIdByName("Admin").first());
        if (id.has_value()) {
            data::UserControl::check::allowUserRegister(id.value());
        }
        data::Booking::buildDB();
        data::Equipment::buildDB();
        data::mail::findSystemUser();
    }

    void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {

        // 拦截 qrc:/SplashScreen.qml 相关的日志
        if (QString(context.file).contains("qrc:/SplashScreen.qml")) {
            return;
        }

        LogLevel logLevel;

        // 根据Qt消息类型映射到你的LogLevel
        switch (type) {
            case QtDebugMsg:
                logLevel = LogLevel::DEBUG;
                break;
            case QtInfoMsg:
                logLevel = LogLevel::INFO;
                break;
            case QtWarningMsg:
                logLevel = LogLevel::WARN;
                break;
            case QtCriticalMsg:
                logLevel = LogLevel::ERR; // QtCriticalMsg 对应你的ERR
                break;
            case QtFatalMsg:
                logLevel = LogLevel::ERR; // QtFatalMsg 也对应你的ERR，致命错误通常会导致程序终止
                break;
            default:
                logLevel = LogLevel::DEBUG;
                break;
        }

        // 构建日志消息内容，包含文件、行号、函数信息
        QString logMessage = QString("[%1:%2] (%3) %4").arg(context.file).arg(context.line).arg(context.function).arg(msg);

        log(logLevel)<< logMessage;
    }
}

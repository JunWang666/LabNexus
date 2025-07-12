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
        data::UserControl::Login::createNewUser("0","Admin","Admin",
                                                data::UserControl::permission::searchGroupIdByName("Admin").first());
        data::Booking::buildDB();
        data::Equipment::buildDB();
        data::mail::findSystemUser();
    }
}

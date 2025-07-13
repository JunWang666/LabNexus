//
// Created by gouzuang on 25-7-13.
//

#include "WorkAlert.h"

#include "module/data/data_mail.h"

namespace bot::WorkAlert {
    void sendAdminCheckNewUserAlert() {
        if (!data::UserControl::permission::isUserInGroup(data::UserControl::currentUserId, "Admin")) return;
        auto count = data::UserControl::check::getUncheckedUserCount();
        if (count == 0) return;
        data::mail::send_mail(data::mail::systemReservedAccounts["工作提醒"],
                              data::UserControl::currentUserId,
                              "还有" + QString::number(count) + "个新用户待审核",
                              "请前往注册中心查看新用户信息并进行审核。",
                              "{}");
    }
}

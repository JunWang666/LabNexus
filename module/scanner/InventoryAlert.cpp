//
// Created by gouzuang on 25-7-10.
//
#include "pch.h"
#include "InventoryAlert.h"
#include "module/data/data_mail.h"


namespace bot::InventoryAlert {
    void sendAlert() {
        if (!data::UserControl::permission::isUserInGroup(data::UserControl::currentUserId,"Admin")) return;

            service::DatabaseManager db(service::Path::equipment());

        QString query = R"(
            SELECT
                ec.id,
                ec.name,
                ec.usable_amount,
                ec.alarm_amount
            FROM
                equipment_class ec
            WHERE
                ec.usable_amount < ec.alarm_amount
                AND ec.alarm_amount > 0;
        )";

            auto alertedRows = db.executeQueryAndFetchAll(query);

            if (alertedRows.isEmpty()) {
                log(service::LogLevel::INFO) << "没有设备库存低于阈值";
                return;
            }

            QString subject = "有" + QString::number(alertedRows.size()) + "类设备库存低于阈值";
            QString message = "以下设备类别库存低于阈值:\n";

            for (const auto &row : alertedRows) {
                int id = row["id"].toInt();
                QString name = row["name"].toString();
                int currentCount = row["usable_amount"].toInt();
                int threshold = row["alarm_amount"].toInt();

                message += name + "\t当前数量: " + QString::number(currentCount) +
                          "\t阈值: " + QString::number(threshold) + "\n";
            }

            data::mail::send_mail(data::mail::systemReservedAccounts["库存预警"],
                                  data::UserControl::currentUserId, subject, message, "{}");
        }
}
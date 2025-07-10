//
// Created by gouzuang on 25-7-10.
//

#include "InventoryAlert.h"

namespace bot::InventoryAlert {
    void sendAlert() {
            // 打开数据库
            service::DatabaseManager db("./equipment.db");

            QString query = R"(
                SELECT ec.id, ec.name,
                       (SELECT COUNT(*) FROM equipment_instance WHERE class_id = ec.id) AS current_count,
                       ec.alert_amount
                FROM equipment_class ec
                WHERE (SELECT COUNT(*) FROM equipment_instance WHERE class_id = ec.id) < ec.alert_amount
                AND ec.alert_amount > 0
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
                int currentCount = row["current_count"].toInt();
                int threshold = row["alert_amount"].toInt();

                message += name + "\t当前数量: " + QString::number(currentCount) +
                          "\t阈值: " + QString::number(threshold) + "\n";
            }

            data::mail::send_mail(data::mail::systemReservedAccounts["库存预警"],
                                  data::UserControl::currentUserId, subject, message, "{}");
        }
}
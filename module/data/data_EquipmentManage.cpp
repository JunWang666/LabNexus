//
// Created by gouzuang on 25-7-7.
//

#include "data_EquipmentManage.h"

namespace data::Equipment {
    void dropDB() {
        QFile dbFile("./equipment.db");
        if (dbFile.exists()) {
            if (dbFile.remove()) {
                log(LogLevel::INFO) << "数据库文件删除成功";
            } else {
                log(LogLevel::ERR) << "数据库文件删除失败";
            }
        } else {
            log(LogLevel::INFO) << "数据库文件不存在";
        }
    }

    void buildDB() {
        QFile dbFile("./user.db");
        if (!dbFile.exists()) {
            if (dbFile.open(QIODevice::WriteOnly)) {
                dbFile.close();
                log(service::LogLevel::INFO) << "数据库文件创建成功";
            } else {
                log(service::LogLevel::ERR) << "数据库文件创建失败";
            }
        } else {
            log(service::LogLevel::INFO) << "数据库文件已存在";
        }
        EquipmentClass::createEquipmentClassTable();
    }

    namespace EquipmentClass {
        void createEquipmentClassTable() {
            service::DatabaseManager db("./equipment.db");
            if (!db.tableExists("equipment_class")) {
                QString createTableQuery = R"(
                    CREATE TABLE equipment_class (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        name TEXT NOT NULL,
                        description TEXT,
                        created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                        amount INTEGER NOT NULL DEFAULT 0,
                    )
                )";
                db.executeNonQuery(createTableQuery);
                log(service::LogLevel::DATA) << "设备类别表创建成功";
            } else {
                log(service::LogLevel::INFO) << "设备类别表已存在";
            }
        }
    }
}

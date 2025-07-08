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
        QFile dbFile("./equipment.db");
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
        EquipmentInstnace::createEquipmentInstanceTable();
    }

        /**
         * @brief 从数据库加载所有设备实例的完整记录，通过 JOIN 关联类别表。
         * @return 返回包含所有设备记录的 QList。
         */
    QList<fullEquipmentRecord> loadFullEquipmentRecords() {
        QList<fullEquipmentRecord> records;
        service::DatabaseManager db("./equipment.db");

        // 使用 JOIN 查询将实例表和类别表关联起来
        // i 是 instance 的别名, c 是 class 的别名
        QString queryString = R"(
            SELECT
                i.id,
                i.name,
                i.status,
                i.created_at,
                i.class_id,
                c.name AS type_name
            FROM
                equipment_instance AS i
            JOIN
                equipment_class AS c ON i.class_id = c.id
        )";

        QSqlQuery query = db.executeQuery(queryString);
        if (!query.isActive()) {
            log(service::LogLevel::ERR) << "加载设备记录失败: " << query.lastError().text();
            return records; // 返回空列表
        }

        while (query.next()) {
            fullEquipmentRecord rec;
            rec.id = query.value("id").toInt();
            rec.name = query.value("name").toString();
            rec.status = query.value("status").toString();
            rec.inDate = query.value("created_at").toDateTime();
            rec.class_id = query.value("class_id").toInt();
            rec.type = query.value("type_name").toString();
            records.append(rec);
        }
        return records;
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
                        amount INTEGER NOT NULL DEFAULT 0
                    )
                )";
                    db.executeNonQuery(createTableQuery);
                    log(service::LogLevel::DATA) << "设备类别表创建成功";
                } else {
                    log(service::LogLevel::INFO) << "设备类别表已存在";
                }
            }
        }

    namespace EquipmentInstnace {
        void createEquipmentInstanceTable() {
            service::DatabaseManager db("./equipment.db");
            if (!db.tableExists("equipment_instance")) {
                QString createTableQuery = R"(
                CREATE TABLE equipment_instance (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        name TEXT NOT NULL,
                        class_id INTEGER NOT NULL,
                        status TEXT NOT NULL DEFAULT '可用',
                        created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                        FOREIGN KEY (class_id) REFERENCES equipment_class (id) ON DELETE CASCADE
                    )
                )";
                if (db.executeNonQuery(createTableQuery)) {
                    log(service::LogLevel::DATA) << "设备实例表创建成功";
                }
                else {
                    log(service::LogLevel::ERR) << "设备实例表创建失败";
                }
            }
            else {
                log(service::LogLevel::INFO) << "设备实例表已经存在";
            }

        }
    }
}

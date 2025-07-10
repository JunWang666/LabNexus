//
// Created by gouzuang on 25-7-7.
//

#include "data_EquipmentManage.h"

namespace data::Equipment {
    void dropDB() {
        QFile dbFile(path);
        if (dbFile.exists()) {
            if (dbFile.remove()) {
                log(LogLevel::INFO) << "数据库文件删除成功"<<path;
            } else {
                log(LogLevel::ERR) << "数据库文件删除失败";
            }
        } else {
            log(LogLevel::INFO) << "数据库文件不存在";
        }
    }

    void buildDB() {
        QFile dbFile(path);
        if (!dbFile.exists()) {
            if (dbFile.open(QIODevice::WriteOnly)) {
                dbFile.close();
                log(service::LogLevel::INFO) << "数据库文件创建成功"<<path;
            } else {
                log(service::LogLevel::ERR) << "数据库文件创建失败";
            }
            EquipmentClass::createEquipmentClassTable();
            EquipmentInstnace::createEquipmentInstanceTable();
        } else {
            log(service::LogLevel::INFO) << "数据库文件已存在";
        }
        

    }

        /**
         * @brief 从数据库加载所有设备实例的完整记录，通过 JOIN 关联类别表。
         * @return 返回包含所有设备记录的 QList。
         */
    QList<fullEquipmentRecord> loadFullEquipmentRecords() {
        QList<fullEquipmentRecord> records;
        service::DatabaseManager db(path);

        // 使用 JOIN 查询将实例表和类别表关联起来
        // i 是 instance 的别名, c 是 class 的别名
        QString queryString = R"(
            SELECT
                i.id,
                i.name,
                i.status,
                i.rentId,
                i.created_at,
                i.class_id,
                c.name AS type_name
            FROM
                equipment_instance AS i
            JOIN
                equipment_class AS c ON i.class_id = c.id
        )";

        auto results = db.executeQueryAndFetchAll(queryString);
        for (const auto &row : results) {
            fullEquipmentRecord rec;
            rec.id = row["id"].toInt();
            rec.name = row["name"].toString();
            rec.status = row["status"].toString();
            rec.rentId = row["rentId"].toInt();
            rec.inDate = row["created_at"].toDateTime();
            rec.class_id = row["class_id"].toInt();
            rec.type = row["type_name"].toString();
            records.append(rec);
        }
        return records;
    }

    bool updateEquipmentOnReturn(int id) {
        service::DatabaseManager db("./equipment.db");
        QString queryString = R"(
        UPDATE equipment_instance
        SET status = ?, rentId = ?
        WHERE Id = ?)";
        QVariantList parmas;
        parmas << "可用" << "" << id;
        bool success = db.executePreparedNonQuery(queryString, parmas);
        if (!success) {
            log(LogLevel::ERR) << "归还设备失败:" << db.getLastError();
        }
        return success;
    }

    bool updateEquipmentOnRepair(int id,const QString& status ) {
        service::DatabaseManager db("./equipment.db");
        QString queryString = R"(
        UPDATE equipment_instance
        SET status = ? WHERE id = ?)";
        QVariantList parmas;
        parmas << status << id;
        bool success = db.executePreparedNonQuery(queryString, parmas);
        if (!success) {
            log(LogLevel::ERR) << "修改失败" << db.getLastError();
        }
        return success;
    }

    QStringList getEquipmentOnStatus(const QString& status) {
        QStringList list;
        QList<fullEquipmentRecord> records;
        records = loadFullEquipmentRecords();
        for (const auto& record : records ) {
            if (record.status == status) {
                list.append(record.type);
            }
        }
        return list;
    }

    EquipmentIds getEquipmentIdsByName(const QString &devName) {
        EquipmentIds result;
        service::DatabaseManager db("./equipment.db");
        if (!db.isConnected()) {
            return result; // 返回无效的ID
        }

        QString queryString = "SELECT id, equipment_class_id FROM equipment WHERE name = ?";
        QVariantList params = {devName};

        // 使用你已有的 executePreparedQueryAndFetchAll 函数
        auto queryResult = db.executePreparedQueryAndFetchAll(queryString, params);

        if (!queryResult.isEmpty()) {
            // 假设设备名称是唯一的，我们只取第一个结果
            QVariantMap row = queryResult.first();
            result.id = row["id"].toInt();
            result.class_id = row["equipment_class_id"].toInt();
        }

        return result;
    }

    namespace EquipmentClass {
            void createEquipmentClassTable() {
                service::DatabaseManager db(path);
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
            service::DatabaseManager db(path);
            if (!db.tableExists("equipment_instance")) {
                QString createTableQuery = R"(
                CREATE TABLE equipment_instance (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        name TEXT NOT NULL,
                        class_id INTEGER NOT NULL,
                        status TEXT NOT NULL DEFAULT '可用',
                        rentId INTEGER,
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

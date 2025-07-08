//
// Created by gouzuang on 25-7-6.
//

#ifndef DATABASESINGLETON_H
#define DATABASESINGLETON_H

#include "databaseManager.h"
#include <QMutex>
#include <memory>

namespace service {

class DatabaseSingleton {
public:
    // 获取单例实例
    static DatabaseSingleton& getInstance();

    // 获取数据库管理器
    DatabaseManager& getManager(const QString& databasePath = "./user.db");

    // 清理所有连接
    void cleanup();

private:
    DatabaseSingleton() = default;
    ~DatabaseSingleton();

    // 禁用拷贝和移动
    DatabaseSingleton(const DatabaseSingleton&) = delete;
    DatabaseSingleton& operator=(const DatabaseSingleton&) = delete;
    DatabaseSingleton(DatabaseSingleton&&) = delete;
    DatabaseSingleton& operator=(DatabaseSingleton&&) = delete;

private:
    QMutex m_mutex;
    std::unique_ptr<DatabaseManager> m_manager;
    QString m_currentPath;
};

} // service

#endif //DATABASESINGLETON_H

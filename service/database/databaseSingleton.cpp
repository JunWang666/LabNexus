//
// Created by gouzuang on 25-7-6.
//

#include "databaseSingleton.h"
#include <QMutexLocker>

namespace service {

DatabaseSingleton& DatabaseSingleton::getInstance()
{
    static DatabaseSingleton instance;
    return instance;
}

DatabaseManager& DatabaseSingleton::getManager(const QString& databasePath)
{
    QMutexLocker locker(&m_mutex);

    // 如果管理器不存在或者路径改变了，创建新的管理器
    if (!m_manager || m_currentPath != databasePath) {
        m_manager = std::make_unique<DatabaseManager>(databasePath);
        m_currentPath = databasePath;
    }

    return *m_manager;
}

void DatabaseSingleton::cleanup()
{
    QMutexLocker locker(&m_mutex);
    m_manager.reset();
    m_currentPath.clear();
}

DatabaseSingleton::~DatabaseSingleton()
{
    cleanup();
}

} // service

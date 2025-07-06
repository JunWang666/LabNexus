//
// Created by gouzuang on 25-7-6.
//

#include "databaseManager.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QThread>

namespace service {
    // SQLite构造函数 - 自动连接
    DatabaseManager::DatabaseManager(const QString &databasePath)
        : m_databasePath(databasePath)
          , m_connected(false) {
        initialize();
        if (!setupAndConnect()) {
            qDebug() << "Failed to connect to database during construction:" << m_lastError;
        }
    }

    // 析构函数 - 自动断开连接
    DatabaseManager::~DatabaseManager() {
        safeCleanup();
    }

    // 初始化
    void DatabaseManager::initialize() {
        m_connectionName = generateConnectionName();
        m_lastError.clear();
    }

    // 安全清理资源
    void DatabaseManager::safeCleanup() {
        // 首先清理所有活跃查询
        clearAllQueries();

        // 等待一小段时间确保查询完全释放
        QThread::msleep(10);

        // 如果连接存在且打开，先关闭连接
        if (m_connected && m_database.isOpen()) {
            m_database.close();
        }
        m_connected = false;

        // 安全移除数据库连接
        if (!m_connectionName.isEmpty()) {
            if (QSqlDatabase::contains(m_connectionName)) {
                // 获取数据库实例并确保完全关闭
                QSqlDatabase db = QSqlDatabase::database(m_connectionName, false);
                if (db.isValid()) {
                    if (db.isOpen()) {
                        db.close();
                    }
                    // 等待数据库完全关闭
                    QThread::msleep(50);
                }

                // 尝试移除连接
                QSqlDatabase::removeDatabase(m_connectionName);
            }
            m_connectionName.clear();
        }
    }

    // 传统清理方法（保持兼容性）
    void DatabaseManager::cleanup() {
        safeCleanup();
    }

    // 查询跟踪方法
    void DatabaseManager::registerQuery(QSqlQuery *query) {
        QMutexLocker locker(&m_queryMutex);
        m_activeQueries.insert(query);
    }

    void DatabaseManager::unregisterQuery(QSqlQuery *query) {
        QMutexLocker locker(&m_queryMutex);
        m_activeQueries.remove(query);
    }

    void DatabaseManager::clearAllQueries() {
        QMutexLocker locker(&m_queryMutex);

        // 强制完成所有活跃查询
        for (QSqlQuery *query: m_activeQueries) {
            if (query && query->isActive()) {
                query->finish();
                query->clear();
            }
        }

        m_activeQueries.clear();
    }

    // 生成唯一连接名
    QString DatabaseManager::generateConnectionName() {
        return QString("DB_Connection_%1").arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    }

    // 设置并连接数据库
    bool DatabaseManager::setupAndConnect() {
        if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
            m_lastError = "SQLite driver is not available";
            return false;
        }

        m_database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);

        // 确��目录存在
        QFileInfo fileInfo(m_databasePath);
        QDir dir = fileInfo.absoluteDir();
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                m_lastError = QString("Failed to create directory: %1").arg(dir.absolutePath());
                return false;
            }
        }

        m_database.setDatabaseName(m_databasePath);

        if (!m_database.open()) {
            m_lastError = m_database.lastError().text();
            return false;
        }

        m_connected = true;
        m_lastError.clear();
        return true;
    }

    // 检查连接状态
    bool DatabaseManager::isConnected() const {
        return m_connected && m_database.isOpen();
    }

    // 获取最后错误信息
    QString DatabaseManager::getLastError() const {
        return m_lastError;
    }

    // 执行查询
    QSqlQuery DatabaseManager::executeQuery(const QString &queryString) {
        QSqlQuery query(m_database);
        registerQuery(&query);

        if (!query.exec(queryString)) {
            m_lastError = query.lastError().text();
            qDebug() << "Query execution failed:" << m_lastError;
        } else {
            m_lastError.clear();
        }

        // 注意：不在这里unregister，因为返回的query可能还要使用
        // 将在析构时或clearAllQueries时处理
        return query;
    }

    // 执行非查询语句
    bool DatabaseManager::executeNonQuery(const QString &queryString) {
        QSqlQuery query(m_database);
        registerQuery(&query);

        bool success = false;
        if (query.exec(queryString)) {
            m_lastError.clear();
            success = true;
        } else {
            m_lastError = query.lastError().text();
            qDebug() << "Query execution failed:" << m_lastError;
        }

        // 立即清理并取消注册
        query.finish();
        query.clear();
        unregisterQuery(&query);

        return success;
    }

    // 执行预处理查询
    QSqlQuery DatabaseManager::executePreparedQuery(const QString &queryString, const QVariantList &parameters) {
        QSqlQuery query(m_database);
        registerQuery(&query);

        query.prepare(queryString);

        for (const QVariant &param: parameters) {
            query.addBindValue(param);
        }

        if (!query.exec()) {
            m_lastError = query.lastError().text();
            qDebug() << "Prepared query execution failed:" << m_lastError;
        } else {
            m_lastError.clear();
        }

        return query;
    }

    // 执行预处理非查询语句
    bool DatabaseManager::executePreparedNonQuery(const QString &queryString, const QVariantList &parameters) {
        QSqlQuery query(m_database);
        registerQuery(&query);

        query.prepare(queryString);

        for (const QVariant &param: parameters) {
            query.addBindValue(param);
        }

        bool success = false;
        if (query.exec()) {
            m_lastError.clear();
            success = true;
        } else {
            m_lastError = query.lastError().text();
            qDebug() << "Prepared query execution failed:" << m_lastError;
        }

        // 立即清理并取消注册
        query.finish();
        query.clear();
        unregisterQuery(&query);

        return success;
    }

    // 开始事务
    bool DatabaseManager::beginTransaction() {
        if (!m_database.transaction()) {
            m_lastError = m_database.lastError().text();
            return false;
        }
        m_lastError.clear();
        return true;
    }

    // 提交事务
    bool DatabaseManager::commitTransaction() {
        if (!m_database.commit()) {
            m_lastError = m_database.lastError().text();
            return false;
        }
        m_lastError.clear();
        return true;
    }

    // 回滚事务
    bool DatabaseManager::rollbackTransaction() {
        if (!m_database.rollback()) {
            m_lastError = m_database.lastError().text();
            return false;
        }
        m_lastError.clear();
        return true;
    }

    // 检查表是否存在
    bool DatabaseManager::tableExists(const QString &tableName) {
        QStringList tables = m_database.tables();
        return tables.contains(tableName, Qt::CaseInsensitive);
    }

    // 获取所有表名
    QStringList DatabaseManager::getTableNames() {
        return m_database.tables();
    }

    // 获取表结构
    QSqlRecord DatabaseManager::getTableStructure(const QString &tableName) {
        return m_database.record(tableName);
    }

    // 批量执行查询
    bool DatabaseManager::executeBatch(const QStringList &queries) {
        if (!beginTransaction()) {
            return false;
        }

        for (const QString &queryString: queries) {
            if (!executeNonQuery(queryString)) {
                rollbackTransaction();
                return false;
            }
        }

        return commitTransaction();
    }

    // 处理查询结果（回调方式）
    void DatabaseManager::processQueryResults(const QString &queryString,
                                              std::function<void(const QSqlRecord &)> processor) {
        QSqlQuery query = executeQuery(queryString);

        if (m_lastError.isEmpty()) {
            while (query.next()) {
                processor(query.record());
            }
        }
    }
} // service

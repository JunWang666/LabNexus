// databaseManager.cpp

#include "databaseManager.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QUuid>
#include <QThread>
#include <QCoreApplication>

namespace service {
    DatabaseManager::DatabaseManager(const QString &databasePath)
        : m_databasePath(databasePath)
          , m_connected(false) {
        initialize();
        if (!setupAndConnect()) {
            qDebug() << "Failed to connect to database during construction:" << m_lastError;
        }
    }

    DatabaseManager::~DatabaseManager() {
        cleanup();
    }

    void DatabaseManager::initialize() {
        m_connectionName = generateConnectionName();
        m_lastError.clear();
    }

    void DatabaseManager::cleanup() {
        // 设置连接状态为假，阻止新的查询
        m_connected = false;

        // 如果连接对象有效且是打开状态，则关闭
        if (m_database.isValid() && m_database.isOpen()) {
            m_database.close();
        }

        // 清空数据库对象，这会释放所有内部资源
        m_database = QSqlDatabase();

        // 只有在应用程序仍在运行时才尝试移除连接
        // 在程序退出时，静态对象析构顺序不确定，Qt可能已经开始清理
        if (QCoreApplication::instance() && !QCoreApplication::instance()->closingDown()) {
            if (QSqlDatabase::contains(m_connectionName)) {
                QSqlDatabase::removeDatabase(m_connectionName);
            }
        }
    }

    QString DatabaseManager::generateConnectionName() {
        return QString("DB_Connection_%1").arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    }

    bool DatabaseManager::setupAndConnect() {
        if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
            m_lastError = "SQLite driver is not available";
            return false;
        }

        m_database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);

        QFileInfo fileInfo(m_databasePath);
        QDir dir = fileInfo.absoluteDir();
        if (!dir.exists() && !dir.mkpath(".")) {
            m_lastError = QString("Failed to create directory: %1").arg(dir.absolutePath());
            return false;
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

    bool DatabaseManager::isConnected() const {
        return m_connected && m_database.isOpen();
    }

    QString DatabaseManager::getLastError() const {
        return m_lastError;
    }

    QSqlQuery DatabaseManager::executeQuery(const QString &queryString) {
        //log(LogLevel::INFO)<<"注意：正在使用过时的数据库查询接口";
        if (!isConnected()) {
            m_lastError = "Database is not connected.";
            return QSqlQuery(m_database);
        }
        QSqlQuery query(m_database);
        if (!query.exec(queryString)) {
            m_lastError = query.lastError().text();
            log(LogLevel::ERR) << "Query execution failed:" << m_lastError;
        } else {
            m_lastError.clear();
        }
        return query;
    }

    // 执行非查询语句
    bool DatabaseManager::executeNonQuery(const QString &queryString) {
        if (!isConnected()) {
            m_lastError = "Database is not connected.";
            return false;
        }
        QSqlQuery query(m_database);
        if (query.exec(queryString)) {
            m_lastError.clear();
            return true;
        } else {
            m_lastError = query.lastError().text();
            qDebug() << "NonQuery execution failed:" << m_lastError;
            return false;
        }
    }

    QList<QVariantMap> DatabaseManager::executeQueryAndFetchAll(const QString &queryString) {
        QList<QVariantMap> resultList;

        this->processQueryResults(queryString, [&resultList](const QSqlRecord &record) {
            QVariantMap row;
            for (int i = 0; i < record.count(); ++i) {
                row.insert(record.fieldName(i), record.value(i));
            }
            resultList.append(row);
        });

        return resultList;
    }

    QList<QVariantMap> DatabaseManager::executePreparedQueryAndFetchAll(const QString &queryString,
        const QVariantList &parameters) {
        QList<QVariantMap> resultList;

        QSqlQuery query = this->executePreparedQuery(queryString, parameters);
        if (!m_lastError.isEmpty()) {
            return resultList;
        }

        while(query.next()) {
            QVariantMap row;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                row.insert(record.fieldName(i), record.value(i));
            }
            resultList.append(row);
        }

        return resultList;
    }


    // 执行预处理查询
    QSqlQuery DatabaseManager::executePreparedQuery(const QString &queryString, const QVariantList &parameters) {
        //log(LogLevel::INFO)<<"注意：正在使用过时的数据库查询接口";
        if (!isConnected()) {
            m_lastError = "Database is not connected.";
            return QSqlQuery(m_database);
        }
        QSqlQuery query(m_database);
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
        if (!isConnected()) {
            m_lastError = "Database is not connected.";
            return false;
        }
        QSqlQuery query(m_database);
        query.prepare(queryString);
        for (const QVariant &param: parameters) {
            query.addBindValue(param);
        }
        if (query.exec()) {
            m_lastError.clear();
            return true;
        } else {
            m_lastError = query.lastError().text();
            qDebug() << "Prepared non-query execution failed:" << m_lastError;
            return false;
        }
    }

    // 事务管理
    bool DatabaseManager::beginTransaction() {
        return m_database.transaction();
    }

    bool DatabaseManager::commitTransaction() {
        return m_database.commit();
    }

    bool DatabaseManager::rollbackTransaction() {
        return m_database.rollback();
    }

    // 便捷方法
    bool DatabaseManager::tableExists(const QString &tableName) {
        return m_database.tables().contains(tableName, Qt::CaseInsensitive);
    }

    QStringList DatabaseManager::getTableNames() {
        return m_database.tables();
    }


    QSqlRecord DatabaseManager::getTableStructure(const QString &tableName) {
        return m_database.record(tableName);
    }

    // 批量执行
    bool DatabaseManager::executeBatch(const QStringList &queries) {
        if (!beginTransaction()) return false;
        for (const QString &queryString: queries) {
            if (!executeNonQuery(queryString)) {
                rollbackTransaction();
                return false;
            }
        }
        return commitTransaction();
    }

    // 回调方式处理查询结果
    void DatabaseManager::processQueryResults(const QString &queryString,
                                              std::function<void(const QSqlRecord &)> processor) {
        // 这个方法是生命周期安全的，因为它不向外暴露QSqlQuery对象
        QSqlQuery query = executeQuery(queryString);
        if (m_lastError.isEmpty()) {
            while (query.next()) {
                processor(query.record());
            }
        }
    }

    // 移除查询跟踪功能的实现，因为它们不再需要
    void DatabaseManager::registerQuery(QSqlQuery *query) {
        // 空实现，保持接口兼容性
        Q_UNUSED(query)
    }

    void DatabaseManager::unregisterQuery(QSqlQuery *query) {
        // 空实现，保持接口兼容性
        Q_UNUSED(query)
    }

    void DatabaseManager::clearAllQueries() {
        // 空实现，保持接口兼容性
    }
}

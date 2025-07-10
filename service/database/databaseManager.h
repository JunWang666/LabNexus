//
// Created by gouzuang on 25-7-6.
//

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QString>
#include <QUuid>
#include <QSet>
#include <QMutex>
#include <functional>

namespace service {
    class DatabaseManager {
    public:
        explicit DatabaseManager(const QString &databasePath,
                                 const QString &driverName = "QSQLITE",
                                 const QString &hostName = QString(),
                                 quint16 port = 0,
                                 const QString &userName = QString(),
                                 const QString &password = QString());

        ~DatabaseManager();

        // 禁用拷贝和移动操作
        DatabaseManager(const DatabaseManager &) = delete;

        DatabaseManager &operator=(const DatabaseManager &) = delete;

        DatabaseManager(DatabaseManager &&) = delete;

        DatabaseManager &operator=(DatabaseManager &&) = delete;

        [[nodiscard]] bool isConnected() const;

        [[nodiscard]] QString getLastError() const;

        // 查询执行
        QSqlQuery executeQuery(const QString &queryString);

        bool executeNonQuery(const QString &queryString);

        // 执行查询并获取所有结果到内存中
        QList<QVariantMap> executeQueryAndFetchAll(const QString &queryString);

        // 为预处理查询也增加一个对应的版本
        QList<QVariantMap> executePreparedQueryAndFetchAll(const QString &queryString, const QVariantList &parameters);

        // 带参数的查询
        QSqlQuery executePreparedQuery(const QString &queryString, const QVariantList &parameters);

        bool executePreparedNonQuery(const QString &queryString, const QVariantList &parameters);

        // 事务管理
        bool beginTransaction();

        bool commitTransaction();

        bool rollbackTransaction();

        // 便捷方法
        bool tableExists(const QString &tableName);

        QStringList getTableNames();

        QSqlRecord getTableStructure(const QString &tableName);

        // 批量操作
        bool executeBatch(const QStringList &queries);

        // 回调方式处理查询结果
        void processQueryResults(const QString &queryString,
                                 std::function<void(const QSqlRecord &)> processor);

    private:
        void initialize();

        void cleanup();

        void safeCleanup();

        QString generateConnectionName();

        bool setupAndConnect();

        // 查询跟踪方法
        void registerQuery(QSqlQuery *query);

        void unregisterQuery(QSqlQuery *query);

        void clearAllQueries();

    private:
        QString m_connectionName; // 唯一连接名
        QSqlDatabase m_database; // 数据库连接对象
        QString m_databasePath; // SQLite数据库路径
        QString m_driverName; // 数据库驱动名称
        QString m_hostName; // 主机名或IP
        quint16 m_port; // 端口号
        QString m_userName; // 用户名
        QString m_password; // 密码
        QString m_lastError; // 最后一次错误信息
        bool m_connected; // 连接状态

        // 查询跟踪
        QSet<QSqlQuery *> m_activeQueries; // 活跃查询集合
        mutable QMutex m_queryMutex; // 查询集合保护锁
    };
} // service

#endif //DATABASEMANAGER_H

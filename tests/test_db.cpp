// test_database_manager.cpp

#include <QtTest>
#include <QFile>
#include "service/database/databaseManager.h" // 仅包含你需要测试的头文件

class TestDatabaseManager : public QObject {
    Q_OBJECT

private:
    // 定义测试用的数据库路径
    const QString testDbPath = "./test_suite.db";

private slots:
    // === 1. 初始化与清理：确保测试环境隔离 ===

    // 在每个测试函数执行前调用
    void init() {
        // 如果上次测试残留了文件，先删除，确保从零开始
        QFile::remove(testDbPath);
    }

    // 在每个测试函数执行后调用
    void cleanup() {
        // 清理本次测试生成的数据库文件
        QFile::remove(testDbPath);
    }

    // === 2. 核心功能测试 ===

    void testConnectionAndDisconnection() {
        // Arrange & Act
        // 使用一个代码块来控制 dbManager 的生命周期
        {
            service::DatabaseManager dbManager(testDbPath);

            // Assert
            QVERIFY2(dbManager.isConnected(), "数据库管理器在构造后应该处于连接状态");
            QVERIFY2(QFile::exists(testDbPath), "数据库文件应该被创建");
        } // dbManager 在这里被销毁，析构函数被调用

        // Assert (after destruction)
        // 验证连接是否已被清理。一个简单的验证方法是再次尝试使用相同的连接名（内部实现），
        // 如果清理成功，则不会有问题。或者，我们可以假设析构函数工作正常。
        // 对于这个测试，我们主要信任 isConnected 的准确性。
    }
    
    void testNonQueryExecution() {
        // Arrange
        service::DatabaseManager dbManager(testDbPath);
        QVERIFY(dbManager.isConnected());

        // Act & Assert for CREATE TABLE
        bool createSuccess = dbManager.executeNonQuery(
            "CREATE TABLE employees (id INTEGER PRIMARY KEY, name TEXT, role TEXT);"
        );
        QVERIFY2(createSuccess, "执行 CREATE TABLE 语句应该成功");
        QVERIFY2(dbManager.getLastError().isEmpty(), "成功执行后，错误信息应该为空");

        // Act & Assert for INSERT
        bool insertSuccess = dbManager.executeNonQuery(
            "INSERT INTO employees (id, name, role) VALUES (1, 'Alice', 'Engineer');"
        );
        QVERIFY2(insertSuccess, "执行 INSERT 语句应该成功");
    }
    
    void testQueryAndFetchAll() {
        // Arrange
        service::DatabaseManager dbManager(testDbPath);
        dbManager.executeNonQuery("CREATE TABLE employees (id INTEGER, name TEXT);");
        dbManager.executeNonQuery("INSERT INTO employees VALUES (101, 'Bob');");
        dbManager.executeNonQuery("INSERT INTO employees VALUES (102, 'Charlie');");

        // Act
        QList<QVariantMap> results = dbManager.executeQueryAndFetchAll("SELECT id, name FROM employees ORDER BY id;");

        // Assert
        QVERIFY2(dbManager.getLastError().isEmpty(), "成功获取数据后，错误信息应该为空");
        QCOMPARE(results.size(), 2);
        QCOMPARE(results[0]["id"].toInt(), 101);
        QCOMPARE(results[0]["name"].toString(), "Bob");
        QCOMPARE(results[1]["id"].toInt(), 102);
        QCOMPARE(results[1]["name"].toString(), "Charlie");
    }

    void testTransactions() {
        // Arrange
        service::DatabaseManager dbManager(testDbPath);
        dbManager.executeNonQuery("CREATE TABLE tasks (id INTEGER PRIMARY KEY, description TEXT);");

        // Act 1: Test COMMIT
        QVERIFY(dbManager.beginTransaction());
        QVERIFY(dbManager.executeNonQuery("INSERT INTO tasks (description) VALUES ('Task A');"));
        QVERIFY(dbManager.commitTransaction());

        // Assert 1
        auto resultsAfterCommit = dbManager.executeQueryAndFetchAll("SELECT * FROM tasks;");
        QCOMPARE(resultsAfterCommit.size(), 1);
        QCOMPARE(resultsAfterCommit[0]["description"].toString(), "Task A");

        // Act 2: Test ROLLBACK
        QVERIFY(dbManager.beginTransaction());
        QVERIFY(dbManager.executeNonQuery("INSERT INTO tasks (description) VALUES ('Task B');"));
        QVERIFY(dbManager.rollbackTransaction());

        // Assert 2
        auto resultsAfterRollback = dbManager.executeQueryAndFetchAll("SELECT * FROM tasks;");
        QCOMPARE(resultsAfterRollback.size(), 1); // 应该仍然只有 Task A
    }


    // === 3. 错误处理测试 ===

    void testInvalidQueryExecution() {
        // Arrange
        service::DatabaseManager dbManager(testDbPath);
        
        // Act: 执行一个语法错误的SQL
        bool success = dbManager.executeNonQuery("CREAT TABLE invalid_sql (id INTEGER);");
        
        // Assert
        QVERIFY2(!success, "执行语法错误的SQL应该失败");
        QVERIFY2(!dbManager.getLastError().isEmpty(), "失败后，getLastError() 应该包含错误信息");
        qDebug() << "Expected error message:" << dbManager.getLastError();
    }
    
    void testFetchFromNonExistentTable() {
        // Arrange
        service::DatabaseManager dbManager(testDbPath);

        // Act
        auto results = dbManager.executeQueryAndFetchAll("SELECT * FROM non_existent_table;");

        // Assert
        QVERIFY2(!dbManager.getLastError().isEmpty(), "从不存在的表中查询应该产生错误");
        QVERIFY2(results.isEmpty(), "发生错误时，结果列表应该为空");
    }

};

// 自定义 main 函数以运行测试
#include <QCoreApplication>
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    // 在测试环境中，通常不需要设置插件路径，因为Qt Test会处理好
    // 但如果你的环境特殊，保留这些行是无害的
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");
    
    TestDatabaseManager tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "test_db.moc"
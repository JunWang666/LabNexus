#include <QtTest>
#include <QFile>
#include "pch.h"

// 声明要测试的全局函数
extern void testLogs();
extern void test_db();

class TestLabNexus : public QObject {
    Q_OBJECT

private slots:
    void testLogs() {
        // 设置日志输出文件
        service::logger::instance().setLogFile("test_app.log");
        service::logger::instance().setDataLogFile("test_data.log");
        ::testLogs();
        QVERIFY(true);
    }

    void testDb() {
        // 重置并初始化数据库
        data::UserControl::dropDB();
        data::UserControl::buildDB();
        ::test_db();
        // 简单检查数据库文件是否存在
        QVERIFY(QFile::exists("user.db"));
    }
};

QTEST_MAIN(TestLabNexus)
#include "test_labnexus.moc"

#include <QtTest>
#include <QFile>
#include "pch.h"
#include "service/logger/logger.h"
#include <QDateTime>

class TestLogs : public QObject {
    Q_OBJECT

private slots:
    void testLogLevels() {
        // 设置日志输出文件
        service::logger::instance().setLogFile("test_app.log");
        service::logger::instance().setDataLogFile("test_data.log");
        // 调用各日志级别
        log(LogLevel::DEBUG) << "Debug级别测试";
        log(LogLevel::INFO) << "Info级别测试";
        log(LogLevel::WARN) << "Warn级别测试";
        log(LogLevel::ERR) << "Err级别测试";
        log(LogLevel::DATA) << "Data级别测试";
        // 检查日志文件是否生成
        QVERIFY(QFile::exists("test_app.log"));
        QVERIFY(QFile::exists("test_data.log"));
    }
};

QTEST_APPLESS_MAIN(TestLogs)
#include "test_logs.moc"


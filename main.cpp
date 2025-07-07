#include "pch.h"

// 测试各级别日志输出函数
void testLogs() {
    // 调用各日志级别
    log(LogLevel::DEBUG) << "Debug级别测试";
    log(LogLevel::INFO) << "Info级别测试";
    log(LogLevel::WARN) << "Warn级别测试";
    log(LogLevel::ERR) << "Err级别测试";
    log(LogLevel::DATA) << "Data级别测试";
}

void init_db() {
    data::UserControl::dropDB();
    data::UserControl::buildDB();
    data::UserControl::permission::createGroup("Student","");
    data::UserControl::permission::createGroup("Teacher","");
    data::UserControl::Login::createNewUser("123", "123", "123","Teacher");
    log(LogLevel::DEBUG)<<data::UserControl::permission::isUserInGroup(1, "Teacher");
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();

    // 初始化并测试日志输出
    service::logger::instance().setLogFile(
        QString("app_%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")).toStdString());
    service::logger::instance().setDataLogFile("data.log");
    service::log() << "程序启动";
    //testLogs();

    init_db();
    log(LogLevel::DEBUG) << data::UserControl::Login::isUserPasswordValid("123", "123");

    return QApplication::exec();
}


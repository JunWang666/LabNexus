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

void test_db() {
    // 重置并初始化数据库
    data::UserControl::dropDB();
    data::UserControl::buildDB();
    // 创建用户并添加到 Teacher 组
    auto userRes = data::UserControl::Login::createNewUser("123", "123", "123", "Teacher");
    int userId = -1;
    if (!userRes) {
        log(LogLevel::ERR) << "创建用户失败, 错误码:" << static_cast<int>(userRes.error());
    } else {
        userId = userRes.value();
        log(LogLevel::INFO) << "用户创建成功, ID=" << userId;
    }
    // 检查用户是否在组中
    bool inGroup = data::UserControl::permission::isUserInGroup(userId, "Teacher");
    log(LogLevel::DEBUG) << "用户 " << userId << (inGroup ? " 在 Teacher 组" : " 不在 Teacher 组");

    auto res = data::UserControl::Login::isUserPasswordValid("123", "123");
    if (res) {
        log(LogLevel::DEBUG) << "用户密码验证成功, 用户ID:" << res.value();
    } else {
        log(LogLevel::DEBUG) << "用户密码验证失败, 错误码:" << static_cast<int>(res.error());
    }

    data::UserControl::Login::updateUserPassword(userId, "new_password");
    data::UserControl::Login::deleteUserById(userId);
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

    test_db();

    return QApplication::exec();
}

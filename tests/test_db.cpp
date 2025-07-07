#include <QtTest>
#include "pch.h"
#include "service/database/databasePathProvidor.h"
#include "service/database/databaseManager.h"
#include "module/data/data_UserControl.h"

class TestDB : public QObject {
    Q_OBJECT

private slots:
    void testDatabaseOperations() {
        // 重置并初始化数据库
        data::UserControl::dropDB();
        data::UserControl::buildDB();

        // 创建用户并添加到 Teacher 组
        auto userRes = data::UserControl::Login::createNewUser("123", "123", "123", "Teacher");
        QVERIFY(userRes);
        int userId = userRes.value();

        // 检查用户是否在组中
        QVERIFY(data::UserControl::permission::isUserInGroup(userId, "Teacher"));

        // 验证用户密码
        auto res = data::UserControl::Login::isUserPasswordValid("123", "123");
        QVERIFY(res);
        QCOMPARE(res.value(), userId);

        // 更新用户密码
        auto updateRes = data::UserControl::Login::updateUserPassword(userId, "new_password");
        QVERIFY(updateRes);

        // 删除用户
        auto deleteRes = data::UserControl::Login::deleteUserById(userId);
        QVERIFY(deleteRes);
    }
};

// 自定义 main，设置插件路径
#include <QCoreApplication>
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    // 添加插件搜索路径
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");
    TestDB tc;
    return QTest::qExec(&tc, argc, argv);
}
#include "test_db.moc"

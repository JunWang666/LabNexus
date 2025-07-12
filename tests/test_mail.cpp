#include <QTest>
#include "module/data/data_mail.h"
#include "module/data/data_UserControl.h"
#include "service/logger/logger.h"
#include "service/stastic/sharedFunctions.h"

class TestMail : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();

    void cleanupTestCase();

    void testSendAndReceiveMail();
};

void TestMail::initTestCase() {
    data::mail::path = "test_mail.db";
    data::UserControl::path = "test_user.db";
    data::UserControl::dropDB();
    data::mail::dropDB();
    data::UserControl::buildDB();
    data::mail::buildDB();
    service::initDB();


    auto senderResult = data::UserControl::Login::createNewUser("sender_id", "sender", "password", "Student");
    QVERIFY(senderResult.has_value());
    auto receiverResult = data::UserControl::Login::createNewUser("receiver_id", "receiver", "password", "Student");
    QVERIFY(receiverResult.has_value());
}

void TestMail::cleanupTestCase() {
}

void TestMail::testSendAndReceiveMail() {
    auto senderIdOpt = data::UserControl::Login::foundUserIdByIdNumber("sender_id");
    auto receiverIdOpt = data::UserControl::Login::foundUserIdByIdNumber("receiver_id");

    QVERIFY(senderIdOpt.has_value());
    QVERIFY(receiverIdOpt.has_value());

}

QTEST_MAIN(TestMail)
#include "test_mail.moc"



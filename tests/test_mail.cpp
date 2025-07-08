#include <QTest>
#include "module/data/data_mail.h"
#include "module/data/data_UserControl.h"
#include "service/logger/logger.h"

class TestMail : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();

    void cleanupTestCase();

    void testSendAndReceiveMail();
};

void TestMail::initTestCase() {
    data::UserControl::dropDB();
    data::mail::dropDB();
    data::UserControl::buildDB();
    data::mail::buildDB();
    // Create test users
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

    int senderId = senderIdOpt.value();
    int receiverId = receiverIdOpt.value();

    data::mail::send_mail(senderId, receiverId, "Test Subject 1", "Test Content 1", "{}");
    data::mail::send_mail(senderId, receiverId, "Test Subject 2", "Test Content 2", "{}");
    QCOMPARE(data::mail::getUnreadMailCount(receiverId,5), 2);

    auto mails = data::mail::getAllMails(receiverId, 1, 10);

    QCOMPARE(mails.size(), 2);
    QCOMPARE(mails[0].subject, "Test Subject 1");
    QCOMPARE(mails[1].subject, "Test Subject 2");

    auto unreadMails = data::mail::getUnreadMails(receiverId, 1, 10);

    QCOMPARE(unreadMails.size(), 2);

    QCOMPARE(data::mail::getMailPageCount(receiverId, 1), 2);
}

QTEST_MAIN(TestMail)
#include "test_mail.moc"



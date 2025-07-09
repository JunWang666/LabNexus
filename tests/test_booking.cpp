#include <QTest>
#include "../module/data/data_Booking.h"
#include "../module/data/data_UserControl.h"
#include "../service/logger/logger.h"
#include "../service/database/databaseManager.h"

class TestBooking : public QObject {
Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testLoadBookingFullRecords();

private:
};

void TestBooking::initTestCase() {
    data::Booking::path = "./test_booking.db";
    data::UserControl::path = "./test_user.db";
    log(LogLevel::DEBUG)<<"data::Booking::path "<<data::Booking::path<<"   data::UserControl::path "<<data::UserControl::path;

    data::Booking::dropDB();
    data::UserControl::dropDB();

    data::UserControl::buildDB();
    data::Booking::buildDB();

    data::UserControl::Login::createNewUser("123", "Stu1", "123", "Student");
    data::UserControl::Login::createNewUser("011", "Tea1", "456", "Teacher");

    auto user1IdOpt = data::UserControl::Login::foundUserIdByIdNumber("123");
    auto user2IdOpt = data::UserControl::Login::foundUserIdByIdNumber("011");

    QVERIFY(user1IdOpt.has_value());
    QVERIFY(user2IdOpt.has_value());
    int user1Id = *user1IdOpt;
    int user2Id = *user2IdOpt;

    QDateTime createDate = QDateTime::fromString("2025-07-07 10:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime requestStartTime = QDateTime::fromString("2025-07-08 09:00:00", "yyyy-MM-dd hh:mm:ss");
    QDateTime requestEndTime = QDateTime::fromString("2025-07-08 11:00:00", "yyyy-MM-dd hh:mm:ss");
    bool success = data::Booking::createFullBookingRecord(1, user1Id, createDate, 1, 1, requestStartTime, requestEndTime, "Pending", user2Id);
    QVERIFY(success);
}

void TestBooking::cleanupTestCase() {
}

void TestBooking::testLoadBookingFullRecords() {
    log(LogLevel::INFO) << "Testing loadBookingFullRecords...";
    QList<data::Booking::fullBookingRecord> records = data::Booking::loadBookingFullRecords();
    log(LogLevel::INFO) << "Loaded booking records count:" << records.size();
    // Log the records for debugging
    for (const auto& rec : records) {
        log(LogLevel::INFO) << "Booking Record - ID:" << rec.id
                 << " User:" << rec.userName
                 << " User Group:" << rec.userGroup
                 << " Request Start:" << rec.requestStartDate.toString(Qt::ISODate)
                 << " Request End:" << rec.requestEndDate.toString(Qt::ISODate)
                 << " Status:" << rec.approvalStatus
                 << " Approver:" << rec.approverName;
    }

    QCOMPARE(records.size(), 1);
    QVERIFY(!records.isEmpty());
    if (!records.isEmpty()) {
        const auto& rec = records.first();
        auto user1IdOpt = data::UserControl::Login::foundUserIdByIdNumber("123");
        auto user2IdOpt = data::UserControl::Login::foundUserIdByIdNumber("011");
        QVERIFY(user1IdOpt.has_value());
        QVERIFY(user2IdOpt.has_value());
        QCOMPARE(rec.userId, *user1IdOpt);
        QCOMPARE(rec.approverID, *user2IdOpt);
    }
}

QTEST_MAIN(TestBooking)
#include "test_booking.moc"

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
    // Setup test databases
    data::Booking::dropDB();
    data::UserControl::dropDB();

    data::UserControl::buildDB();
    data::Booking::buildDB();

    service::DatabaseManager userDb("./test_user.db");
    service::DatabaseManager bookingDb("./test_booking.db");

    // Setup: Create some users
    data::UserControl::Login::createNewUser("123", "Stu1", "123", "Student");
    data::UserControl::Login::createNewUser("011", "Tea1", "456", "Teacher");

    auto user1IdOpt = data::UserControl::Login::foundUserIdByIdNumber("123");
    auto user2IdOpt = data::UserControl::Login::foundUserIdByIdNumber("011");

    QVERIFY(user1IdOpt.has_value());
    QVERIFY(user2IdOpt.has_value());
    int user1Id = *user1IdOpt;
    int user2Id = *user2IdOpt;

    // We will directly insert into booking tables.
    service::DatabaseManager db("./booking.db");
    db.executeNonQuery(QString("INSERT INTO booking_info (id, user_id, create_date) VALUES (1, %1, '2025-07-07 10:00:00')").arg(user1Id));
    db.executeNonQuery("INSERT INTO booking_time (booking_id, request_start_time, request_end_time) VALUES (1, '2025-07-08 09:00:00', '2025-07-08 11:00:00')");
    db.executeNonQuery(QString("INSERT INTO booking_approval (booking_id, approval_status, approver_id) VALUES (1, 'Pending', %1)").arg(user2Id));
}

void TestBooking::cleanupTestCase() {
}

void TestBooking::testLoadBookingFullRecords() {
    QList<data::Booking::fullBookingRecord> records = data::Booking::loadBookingFullRecords();

    QCOMPARE(records.size(), 1);
    if (!records.isEmpty()) {
        const auto& rec = records.first();
        QCOMPARE(rec.userName, QString("Stu1"));
        QCOMPARE(rec.approverName, QString("Tea1"));
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

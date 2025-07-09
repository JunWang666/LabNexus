#include "pch.h"
#include <QTest>
#include "../module/data/data_EquipmentManage.h"
#include "../service/logger/logger.h"
#include "../service/database/databaseManager.h"

class TestEquipmentManage : public QObject {
Q_OBJECT
private slots:
    void initTestCase();
    void testLoadFullEquipmentRecords();

private:
};

void TestEquipmentManage::initTestCase() {
    data::Equipment::dropDB();
    data::Equipment::buildDB();

    service::DatabaseManager db("./equipment.db");

    // Setup: Create some equipment classes and instances
    db.executeNonQuery("INSERT INTO equipment_class (id, name, description, amount) VALUES (1, 'Microscope', 'For viewing small things', 1)");
    db.executeNonQuery("INSERT INTO equipment_class (id, name, description, amount) VALUES (2, 'Spectrometer', 'For measuring spectra', 1)");

    db.executeNonQuery("INSERT INTO equipment_instance (id, name, class_id, status) VALUES (1, 'M-001', 1, 'Available')");
    db.executeNonQuery("INSERT INTO equipment_instance (id, name, class_id, status) VALUES (2, 'S-001', 2, 'In Use')");
}


void TestEquipmentManage::testLoadFullEquipmentRecords() {
    QList<data::Equipment::fullEquipmentRecord> records = data::Equipment::loadFullEquipmentRecords();

    QCOMPARE(records.size(), 2);

    bool foundM001 = false;
    bool foundS001 = false;

    for (const auto& rec : records) {
        if (rec.name == "M-001") {
            foundM001 = true;
            QCOMPARE(rec.type, QString("Microscope"));
            QCOMPARE(rec.status, QString("Available"));
            QCOMPARE(rec.class_id, 1);
        } else if (rec.name == "S-001") {
            foundS001 = true;
            QCOMPARE(rec.type, QString("Spectrometer"));
            QCOMPARE(rec.status, QString("In Use"));
            QCOMPARE(rec.class_id, 2);
        }
    }

    QVERIFY(foundM001);
    QVERIFY(foundS001);
}

QTEST_MAIN(TestEquipmentManage)
#include "test_equipment_manage.moc"

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

}


void TestEquipmentManage::testLoadFullEquipmentRecords() {

}

QTEST_MAIN(TestEquipmentManage)
#include "test_equipment_manage.moc"

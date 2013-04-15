#include <QtCore/QPointer>

#include "../libconnman-qt/networkmanager.h"
#include "testbase.h"

namespace Tests {

class UtBasicWithTechnology : public TestBase
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testProperties_data();
    void testProperties();
    void testGetTechnologies();
    void testTechnologiesList();
    void testTechnologyPathForType();
    void testTechnologyProperties_data();
    void testTechnologyProperties();
    void testGetTechnology();

private:
    QPointer<NetworkManager> m_manager;
};

} // namespace Tests

using namespace Tests;

void UtBasicWithTechnology::initTestCase()
{
    m_manager = NetworkManagerFactory::createInstance();
    QVERIFY(m_manager->isAvailable());
}

void UtBasicWithTechnology::cleanupTestCase()
{
    delete m_manager;
}

void UtBasicWithTechnology::testProperties_data()
{
    QTest::addColumn<QVariant>("expected");

    QTest::newRow("available") << QVariant(true);
    QTest::newRow("state") << QVariant("offline");
    QTest::newRow("offlineMode") << QVariant(false);
    QTest::newRow("sessionMode") << QVariant(true);
    QTest::newRow("defaultRoute") << QVariant::fromValue((NetworkService *)0);
}

void UtBasicWithTechnology::testProperties()
{
    QFETCH(QVariant, expected);

    QVERIFY(m_manager->property(QTest::currentDataTag()).isValid());
    QCOMPARE(m_manager->property(QTest::currentDataTag()), expected);
}

void UtBasicWithTechnology::testGetTechnologies()
{
    QCOMPARE(m_manager->getTechnologies().count(), 1);
}

void UtBasicWithTechnology::testTechnologiesList()
{
    QCOMPARE(m_manager->technologiesList(), QStringList("wifi"));
}

void UtBasicWithTechnology::testTechnologyPathForType()
{
    QCOMPARE(m_manager->technologyPathForType("wifi"), QString("/technology0"));
    QCOMPARE(m_manager->technologyPathForType("ethernet"), QString());
}

void UtBasicWithTechnology::testTechnologyProperties_data()
{
    QTest::addColumn<QVariant>("expected");

    QTest::newRow("name") << QVariant("Wi-Fi interface FOO");
    QTest::newRow("type") << QVariant("wifi");
    QTest::newRow("powered") << QVariant(true);
    QTest::newRow("connected") << QVariant(false);
    QTest::newRow("path") << QVariant("/technology0");
    QTest::newRow("idleTimetout") << QVariant(42);
    QTest::newRow("tethering") << QVariant(false);
    QTest::newRow("tetheringId") << QVariant("foo-tether-identifier");
    QTest::newRow("tetheringPassphrase") << QVariant("foo-tether-passwd");
}

void UtBasicWithTechnology::testTechnologyProperties()
{
    QFETCH(QVariant, expected);

    QCOMPARE(m_manager->getTechnologies().count(), 1);
    QVERIFY(m_manager->getTechnologies().at(0)->property(QTest::currentDataTag()).isValid());
    QCOMPARE(m_manager->getTechnologies().at(0)->property(QTest::currentDataTag()), expected);
}

void UtBasicWithTechnology::testGetTechnology()
{
    QVERIFY(m_manager->getTechnology("wifi") != 0);
    QVERIFY(m_manager->getTechnology("ethernet") == 0);
}

QTEST_MAIN(UtBasicWithTechnology)

#include "ut_basic_with_technology.moc"

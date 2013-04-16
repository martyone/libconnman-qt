#include <QtCore/QPointer>

#include "../libconnman-qt/networkmanager.h"
#include "testbase.h"

namespace Tests {

class UtBasicWithService : public TestBase
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testProperties_data();
    void testProperties();
    void testGetServices();
    void testServicesList();
    void testDefaultRoute();
    void testServiceProperties_data();
    void testServiceProperties();

private:
    QPointer<NetworkManager> m_manager;
};

} // namespace Tests

using namespace Tests;

void UtBasicWithService::initTestCase()
{
    m_manager = NetworkManagerFactory::createInstance();
    QVERIFY(m_manager->isAvailable());
}

void UtBasicWithService::cleanupTestCase()
{
    delete m_manager;
}

void UtBasicWithService::testProperties_data()
{
    QTest::addColumn<QVariant>("expected");

    QTest::newRow("available") << QVariant(true);
    QTest::newRow("state") << QVariant("offline");
    QTest::newRow("offlineMode") << QVariant(false);
    QTest::newRow("sessionMode") << QVariant(true);
    // defaultRoute property tested in testDefaultRoute()
}

void UtBasicWithService::testProperties()
{
    QFETCH(QVariant, expected);

    QVERIFY(m_manager->property(QTest::currentDataTag()).isValid());
    QCOMPARE(m_manager->property(QTest::currentDataTag()), expected);
}

void UtBasicWithService::testGetServices()
{
    QCOMPARE(m_manager->getServices().count(), 1);
    QCOMPARE(m_manager->getServices("wifi").count(), 1);
    QCOMPARE(m_manager->getServices(), m_manager->getServices("wifi"));
}

void UtBasicWithService::testServicesList()
{
    QCOMPARE(m_manager->servicesList(QString()), QStringList("/service0"));
    QCOMPARE(m_manager->servicesList("wifi"), QStringList("/service0"));
}

void UtBasicWithService::testDefaultRoute()
{
    QCOMPARE(m_manager->getServices().count(), 1);

    QVERIFY(m_manager->defaultRoute() != 0);
    QVERIFY(m_manager->defaultRoute() == m_manager->getServices().at(0));

    QVERIFY(m_manager->property("defaultRoute").isValid());
    QCOMPARE(m_manager->property("defaultRoute"),
            QVariant::fromValue(m_manager->getServices().at(0)));
}

void UtBasicWithService::testServiceProperties_data()
{
    QTest::addColumn<QVariant>("expected");

    QTest::newRow("name") << QVariant("Wireless BAR");
    QTest::newRow("state") << QVariant("failure");
    QTest::newRow("type") << QVariant("wifi");
    QTest::newRow("error") << QVariant("dhcp-error");
    QTest::newRow("security") << QVariant(QStringList() << "none" << "wep");
    QTest::newRow("strength") << QVariant("42");
    QTest::newRow("favorite") << QVariant(false);
    QTest::newRow("autoConnect") << QVariant(true);
    QTest::newRow("path") << QVariant("/service0");
    QTest::newRow("ipv4/Method") << QVariant("dhcp");
    QTest::newRow("ipv4/Address") << QVariant("10.0.0.42");
    QTest::newRow("ipv4/Netmask") << QVariant("255.255.255.0");
    QTest::newRow("ipv4/Gateway") << QVariant("10.0.0.1");
    QTest::newRow("ipv4Config/Method") << QVariant("manual");
    QTest::newRow("ipv4Config/Address") << QVariant("10.0.42.42");
    QTest::newRow("ipv4Config/Netmask") << QVariant("255.255.242.0");
    QTest::newRow("ipv4Config/Gateway") << QVariant("10.0.42.1");
    QTest::newRow("ipv6/Method") << QVariant("dhcp");
    QTest::newRow("ipv6/Address") << QVariant("fd30:84f5:4397:1676:d:e:a:d");
    QTest::newRow("ipv6/PrefixLength") << QVariant(64);
    QTest::newRow("ipv6/Gateway") << QVariant("fd30:84f5:4397:1676:d:e:a:1");
    QTest::newRow("ipv6/Privacy") << QVariant("prefered");
    QTest::newRow("ipv6Config/Method") << QVariant("manual");
    QTest::newRow("ipv6Config/Address") << QVariant("fd30:84f5:4397:1676:b:e:e:f");
    QTest::newRow("ipv6Config/PrefixLength") << QVariant(64);
    QTest::newRow("ipv6Config/Gateway") << QVariant("fd30:84f5:4397:1676:b:e:e:1");
    QTest::newRow("ipv6Config/Privacy") << QVariant("enabled");
    QTest::newRow("nameservers") << QVariant(QStringList() << "10.0.0.1" << "10.0.0.2");
    QTest::newRow("nameserversConfig") << QVariant(QStringList() << "127.0.0.1" << "127.0.0.2");
    QTest::newRow("domains") << QVariant(QStringList() << "foo.org" << "foo.com");
    QTest::newRow("domainsConfig") << QVariant(QStringList() << "bar.org" << "bar.com");
    QTest::newRow("proxy/Method") << QVariant("auto");
    QTest::newRow("proxy/URL") << QVariant("proxy.foo.org");
    QTest::newRow("proxy/Servers") << QVariant(QStringList());
    QTest::newRow("proxy/Excludes") << QVariant(QStringList());
    QTest::newRow("proxyConfig/Method") << QVariant("manual");
    QTest::newRow("proxyConfig/URL") << QVariant(QString());
    QTest::newRow("proxyConfig/Servers") << QVariant(QStringList() << "proxy42.foo.org");
    QTest::newRow("proxyConfig/Excludes") << QVariant(QStringList() << "drct1.org" << "drct2.org");
    QTest::newRow("ethernet/Method") << QVariant("auto");
    QTest::newRow("ethernet/Interface") << QVariant("eth123");
    QTest::newRow("ethernet/Address") << QVariant("de:ad:be:ef:de:ad");
    QTest::newRow("ethernet/MTU") << QVariant(1542);
    QTest::newRow("roaming") << QVariant(true);
}

void UtBasicWithService::testServiceProperties()
{
    Q_ASSERT(QString(QTest::currentDataTag()).count('/') <= 1);

    QFETCH(QVariant, expected);

    const QString propertyName = QString(QTest::currentDataTag()).section('/', 0, 0);
    const QString variantMapKey = QString(QTest::currentDataTag()).section('/', 1, 1);

    QCOMPARE(m_manager->getServices().count(), 1);

    NetworkService *const service = m_manager->getServices().at(0);
    QVERIFY(service != 0);

    QVERIFY(service->property(qPrintable(propertyName)).isValid());

    if (variantMapKey.isEmpty()) {
        QVERIFY(service->property(qPrintable(propertyName)).type() != QVariant::Map);
        QCOMPARE(service->property(qPrintable(propertyName)), expected);
    } else {
        QCOMPARE(service->property(qPrintable(propertyName)).type(), QVariant::Map);
        QVERIFY(service->property(qPrintable(propertyName)).toMap().contains(variantMapKey));
        QCOMPARE(service->property(qPrintable(propertyName)).toMap()[variantMapKey], expected);
    }
}

QTEST_MAIN(UtBasicWithService)

#include "ut_basic_with_service.moc"

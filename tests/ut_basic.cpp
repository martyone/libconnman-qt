#include <QtCore/QPointer>

#include "../libconnman-qt/networkmanager.h"
#include "testbase.h"

namespace Tests {

class UtBasic : public TestBase
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testProperties_data();
    void testProperties();
    void testWriteProperties_data();
    void testWriteProperties();

private:
    QPointer<NetworkManager> m_manager;
};

} // namespace Tests

using namespace Tests;

void UtBasic::initTestCase()
{
    m_manager = NetworkManagerFactory::createInstance();
    QVERIFY(m_manager->isAvailable());
}

void UtBasic::cleanupTestCase()
{
    delete m_manager;
}

void UtBasic::testProperties_data()
{
    QTest::addColumn<QVariant>("expected");

    QTest::newRow("available") << QVariant(true);
    QTest::newRow("state") << QVariant("offline");
    QTest::newRow("offlineMode") << QVariant(false);
    QTest::newRow("sessionMode") << QVariant(true);
    QTest::newRow("defaultRoute") << QVariant::fromValue((NetworkService *)0);
}

void UtBasic::testProperties()
{
    QFETCH(QVariant, expected);

    QVERIFY(m_manager->property(QTest::currentDataTag()).isValid());
    QCOMPARE(m_manager->property(QTest::currentDataTag()), expected);
}

void UtBasic::testWriteProperties_data()
{
    QTest::addColumn<QVariant>("newValue");

    QTest::newRow("offlineMode") << QVariant(true);
    QTest::newRow("sessionMode") << QVariant(false);
}

void UtBasic::testWriteProperties()
{
    QFETCH(QVariant, newValue);

    const QByteArray notifySignal = this->notifySignal(*m_manager, QTest::currentDataTag());

    SignalSpy spy(m_manager, notifySignal);

    QVERIFY(m_manager->setProperty(QTest::currentDataTag(), newValue));

    QVERIFY(waitForSignals(SignalSpyList() << &spy));

    QVERIFY(m_manager->property(QTest::currentDataTag()).isValid());
    QCOMPARE(m_manager->property(QTest::currentDataTag()), newValue);
}

QTEST_MAIN(UtBasic)

#include "ut_basic.moc"

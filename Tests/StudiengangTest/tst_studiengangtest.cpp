#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class StudiengangTest : public QObject
{
    Q_OBJECT

public:
    StudiengangTest();
    ~StudiengangTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

StudiengangTest::StudiengangTest()
{

}

StudiengangTest::~StudiengangTest()
{

}

void StudiengangTest::initTestCase()
{

}

void StudiengangTest::cleanupTestCase()
{

}

void StudiengangTest::test_case1()
{

}

QTEST_MAIN(StudiengangTest)

#include "tst_studiengangtest.moc"

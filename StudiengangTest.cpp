#include <QtTest/QTest>
#include <QSqlDatabase>

#include "DB.h"

class StudiengangTest : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase _db;

private slots:

    /*!
     * \brief initTestCase sets up the database for the tests.
     *
     * It adds a new SQLite database.
     */
    void initTestCase()
    {
        _db = QSqlDatabase::addDatabase("SQLITE");
        _db.setDatabaseName("test");
        QVERIFY(_db.isValid());
        QCOMPARE(_db.open(), true);
    }

    /*!
     * \brief initTestCase_data populates the test database.
     */
    void initTestCase_data()
    {
        QCOMPARE(DB::initialize(_db), true);


    }

    /*!
     * \brief cleanupTestCase is run after all other tests.
     */
    void cleanupTestCase()
    {
        DB::clean(_db);
    }

};

QTEST_APPLESS_MAIN(StudiengangTest)
//#include "studiengangTest.moc"

/* BOOST DIRECTIVES*/
#define BOOST_TEST_MODULE Test-case fixture studiengang_test
#include <boost/test/included/unit_test.hpp>

/* QT DIRECTIVES*/
#include <QSqlDatabase>

/* OWN DIRECTIVES*/
#include "DB.h"
#include "studiengang.h"    // the class to be tested

struct Fixture {
    Fixture() {
        BOOST_TEST_MESSAGE("setup fixture for studiengang_test");

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.open();
        DB::clean(db);      // clear all tables
        DB::initialize(db); // check if all tables exist
        DB::test(db);       // populate tables with data
    }

    ~Fixture() {
        BOOST_TEST_MESSAGE("teardown fixture for studiengang_test");
        DB::clean(db);      // clear all tables
        db.close();
    }

    QSqlDatabase db;
};

/*#####################################################################*/
BOOST_FIXTURE_TEST_SUITE(studiengang_suit, Fixture) // test suit beginn

BOOST_AUTO_TEST_CASE( query )
{
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END() // test suit end
/*#####################################################################*/

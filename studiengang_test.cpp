/* BOOST DIRECTIVES*/
#define BOOST_TEST_MODULE Test-case fixture studiengang_test
#include <boost/test/included/unit_test.hpp>

/* QT DIRECTIVES*/
#include <QSqlDatabase>
#include <vector>
#include <tuple>
#include <QString>

/* OWN DIRECTIVES*/
#include "DB.h"
#include "studiengang.h"    // the class to be tested

struct Fixture {
    Fixture() {
        BOOST_TEST_MESSAGE("setup fixture for studiengang_test");

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("test");
        db.open();

        BOOST_REQUIRE_EQUAL(db.isValid(), true);

        BOOST_REQUIRE_EQUAL(DB::clean(db), true);
        BOOST_REQUIRE_EQUAL(DB::initialize(db), true);
        BOOST_REQUIRE_EQUAL(DB::test(db), true);
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

BOOST_AUTO_TEST_CASE( query_all_1 )
{
    std::vector<Studiengang> vec = Studiengang::query_all();

    BOOST_CHECK_EQUAL(vec.size(), 12);
}

BOOST_AUTO_TEST_CASE( query_1 )
{
    QString s = "schwerpunkt=IN-AI";
    std::vector<Studiengang> vec = Studiengang::query(s);

    BOOST_CHECK_EQUAL(vec.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END() // test suit end
/*#####################################################################*/

BOOST_AUTO_TEST_CASE( split_1 )
{
    QString s = "name=bob,key=value,,franzi=love";
    vector<QString> keys = {"name", "key", "franzi"};
    vector<QString> vals = {"bob", "value", "love"};
    std::vector<std::tuple<QString, QString>> vec = key_val_split(s);
    int i = 0;

    for(auto kv : vec) {
        BOOST_CHECK(std::get<0>(kv) == keys[i]);
        BOOST_CHECK(std::get<1>(kv) == vals[i]);
        ++i;
    }
}

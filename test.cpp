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
#include "nutzer.h"
#include "studiengang.h"    // the class to be tested
#include "sha512.h"

struct GlobalFixture {
    GlobalFixture() {
        BOOST_TEST_MESSAGE("global fixture for studiengang_test");
    }

    void setup() {
        BOOST_TEST_MESSAGE("setup global fixture");
        DB::session();

        db = QSqlDatabase::database();

        BOOST_REQUIRE_EQUAL(db.isValid(), true);

        BOOST_REQUIRE_EQUAL(DB::clean(db), true);
        BOOST_REQUIRE_EQUAL(DB::initialize(db), true);
        BOOST_REQUIRE_EQUAL(DB::test(db), true);
    }

    void teardown() {
        BOOST_TEST_MESSAGE("teardown global fixture");
        db.close();
    }

    ~GlobalFixture() {
        BOOST_TEST_MESSAGE("end of global fixture for studiengang_test");
    }


    QSqlDatabase db;
};


BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);

/*#####################################################################
  #################    STUDIENGANG    ###############################*/
BOOST_AUTO_TEST_SUITE(studiengang_suit) // test suit beginn

BOOST_AUTO_TEST_CASE( query_all_1 )
{
    std::vector<Studiengang> vec = Studiengang::query_all();

    BOOST_CHECK_EQUAL(vec.size(), 12);
}

BOOST_AUTO_TEST_CASE( query_1 )
{
    QString s = "schwerpunkt = 'IN-AI'";
    std::vector<Studiengang> vec = Studiengang::query(s);

    BOOST_CHECK_EQUAL(vec.size(), 2);
}

BOOST_AUTO_TEST_CASE( query_2 )
{
    // an empty string should act like a query_all()
    QString s = "";
    std::vector<Studiengang> vec = Studiengang::query(s);

    BOOST_CHECK_EQUAL(vec.size(), 12);
}

BOOST_AUTO_TEST_CASE( query_3 )
{
    QString s = "schwerpunkt = 'IN-AI' and abschluss = 'Master'";
    std::vector<Studiengang> vec = Studiengang::query(s);

    BOOST_CHECK_EQUAL(vec.size(), 1);
    BOOST_CHECK(vec[0].schwerpunkt() == "IN-AI");
    BOOST_CHECK(vec[0].abschluss() == "Master");
}

BOOST_AUTO_TEST_CASE( query_4 )
{
    QString s = "schwerpunkt = 'IN-AI' or schwerpunkt = 'IN-IS'";
    std::vector<Studiengang> vec = Studiengang::query(s);

    BOOST_CHECK_EQUAL(vec.size(), 4);
}




BOOST_AUTO_TEST_SUITE_END() // test suit end
/*#####################################################################
  ###############          NUTZER         #############################
  #####################################################################*/
BOOST_AUTO_TEST_SUITE(nutzer_suit) // test suit beginn

BOOST_AUTO_TEST_CASE( pw_hashing_test_1 )
{
    QString input = "grape";
    QString ouput = sha512(input);
    QString expec = "9375d1abdb644a01955bccad12e2f5c2bd8a3e226187e548d99c559a99461453b980123746753d07c169c22a5d9cc75cb158f0e8d8c0e713559775b5e1391fc4";

    BOOST_CHECK(ouput == expec);
}

BOOST_AUTO_TEST_CASE( pw_hashing_test_2 )
{
    QString input = " !#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    std::cout << input.toStdString() << std::endl;
    QString ouput = sha512(input);
    QString expec = "339734dfadaffd7165e2abc2a0d3323b2dc4c1ebe491e216e8a441f8e1c7acbd03a6e3549e8d6cd6ae19df4e247c820559b6d4137cad86316ea359341c1db6f5";

    BOOST_CHECK(ouput == expec);
}

BOOST_AUTO_TEST_CASE( pw_hashing_test_3 )
{
    QString input = "";
    QString ouput = sha512(input);
    QString expec = "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e";

    BOOST_CHECK(ouput == expec);
}

BOOST_AUTO_TEST_CASE( pw_hashing_test_4 )
{
    QString input = "                     ";
    QString ouput = sha512(input);
    QString expec = "8d912bfd3922a2e7a17731dbfe6eeabfff60681c183b12b1d969765fcd480ebe676c7f676571bbe6c431c3844bd2b5530d44d07d33c5ce0457319d8775b7dbc6";

    BOOST_CHECK(ouput == expec);
}

BOOST_AUTO_TEST_CASE( pw_hashing_test_5 )
{
    QString input = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïð";
    QString ouput = sha512(input);
    QString expec = "8b542254efbe6d99556e5d161d7a7c3974a41dd87c2898876ed2bca9e63ab195b9d99f263b3a2258af785ae3f90df32e0b11d9c3e09b63b4120a2f5c1b95d117";

    BOOST_CHECK(ouput == expec);
}

BOOST_AUTO_TEST_CASE( password_accept_test_1 )
{
    Nutzer user;
    QString pw1 = "supersecure";
    QString pw2 = "supersecure";

    user.set_password(pw1);

    BOOST_CHECK_EQUAL(user.check_password(pw2), true);
}

BOOST_AUTO_TEST_CASE( password_accept_test_2 )
{
    Nutzer user;
    QString pw1 = " !#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    QString pw2 = " !#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    user.set_password(pw1);

    BOOST_CHECK_EQUAL(user.check_password(pw2), true);
}



BOOST_AUTO_TEST_CASE( password_deny_test_1 )
{
    Nutzer user;
    QString pw1 = "supersecure";
    QString pw2 = "supersacure";

    user.set_password(pw1);

    BOOST_CHECK_EQUAL(user.check_password(pw2), false);
}

BOOST_AUTO_TEST_CASE( password_special_chars_test1 )
{
    Nutzer user;
    QString pw1 = "#'.^`~12äöüß<,>}";
    QString pw2 = "#'.^`~12äöüß<,>}";
    QString pw3 = "#";
    QString pw4 = "";

    user.set_password(pw1);

    BOOST_CHECK_EQUAL(user.check_password(pw2), true);
    BOOST_CHECK_EQUAL(user.check_password(pw3), false);
    BOOST_CHECK_EQUAL(user.check_password(pw4), false);
}

BOOST_AUTO_TEST_CASE( role_test_1 )
{
    Nutzer user;
    user.setRole(Nutzer::Role::student);

    BOOST_CHECK_EQUAL(user.is_student(), true);
    BOOST_CHECK_EQUAL(user.is_dozent(), false);
    BOOST_CHECK_EQUAL(user.is_administrator(), false);
}

BOOST_AUTO_TEST_CASE( role_test_2 )
{
    Nutzer user;
    user.setRole(Nutzer::Role::dozent);

    BOOST_CHECK_EQUAL(user.is_student(), false);
    BOOST_CHECK_EQUAL(user.is_dozent(), true);
    BOOST_CHECK_EQUAL(user.is_administrator(), false);
}

BOOST_AUTO_TEST_CASE( role_test_3 )
{
    Nutzer user;
    user.setRole(Nutzer::Role::administrator);

    BOOST_CHECK_EQUAL(user.is_student(), false);
    BOOST_CHECK_EQUAL(user.is_dozent(), false);
    BOOST_CHECK_EQUAL(user.is_administrator(), true);
}

BOOST_AUTO_TEST_CASE( query_all_1 )
{
    std::vector<Nutzer> vec = Nutzer::query_all();

    BOOST_CHECK_EQUAL(vec.size(), 3);
}

BOOST_AUTO_TEST_CASE( query_1 )
{
    std::vector<Nutzer> vec;
    QString query_string = "email = 'dk@hs.aa'";
    vec = Nutzer::query(query_string);

    BOOST_CHECK_EQUAL(vec.size(), 1);
    BOOST_CHECK(vec[0].vname() == "Detlef");
    BOOST_CHECK(vec[0].nname() == "Küpper");
    BOOST_CHECK_EQUAL(vec[0].check_password("DK"), true);
}

BOOST_AUTO_TEST_SUITE_END() // test suit end

/*#####################################################################
  ###############          DB             #############################
  #####################################################################*/
BOOST_AUTO_TEST_SUITE(db_suit) // test suit beginn

BOOST_AUTO_TEST_CASE( db_add_studiengang_test_1 )
{
    Studiengang studiengang = Studiengang("IN-MI", "Bachelor");
    int id = DB::session().add(studiengang);

    BOOST_CHECK_EQUAL(id, 5);
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

BOOST_AUTO_TEST_CASE( split_2 )
{
    // check for empty string
    QString s = "";
    std::vector<std::tuple<QString, QString>> vec = key_val_split(s);

    BOOST_CHECK_EQUAL(vec.size(), 0);
}

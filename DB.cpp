#include "DB.h"
#include <QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>

using namespace std;

void log(const QString& type, const QString& msg) {
    qDebug() << "[" << QDateTime::currentDateTime().toString() << "][" << type << "]: " << msg;
}

/*!
 * \brief DB::DB is the default constructor.
 *
 * It sets up an SQLite database.
 *
 * After the db is set up it becomes the default connection
 * which can be retrieved using QSqlDatabase::database(), e.g.
 * without the connection name argument.
 *
 * If the specified SQLite databes couldn't be found, a new database
 * with the same name is created.
 */
DB::DB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dev.sqlite");   // name of the database
    bool ok = db.open();

    if( ok ) {
        qDebug() << "Data base set-up successfull.";
    } else {
        qDebug() << db.lastError();
    }

    initialize(); // init database
}

DB::~DB()
{}

void
DB::add(Studiengang &s)
{
    QSqlQuery query;
    QString qs;
    QSqlDatabase db = QSqlDatabase::database(); // retrieve database

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    qs = "INSERT INTO studiengang (schwerpunkt, abschluss) VALUES (" +
         db.driver()->escapeIdentifier(s.schwerpunkt(), QSqlDriver::FieldName) +
         ", " +
         db.driver()->escapeIdentifier(s.abschluss(), QSqlDriver::FieldName) + ");";

    log("Query", qs);
    if( !query.exec(qs) ) {
        throw DatabaseTransactionError(query.lastError().text());
    }
}

//DB::add(Arbeit &a)
//{
//    QSqlQuery query;
//    QString qs;
//    QSqlDatabase db = QSqlDatabase::database(); // retrieve database

//    if (!db.isValid())  throw InvalidDatabaseError("Invalid Database connection");

//    qs = "INSERT INTO arbeit (titel, status, erlaeuterung, studiengang"

//    log("Query", qs);
//    if( !query.exec(qs) ) {
//        qDebug() << query.lastError().type();
//        throw DatabaseTransactionError(query.lastError().text());
//    }
//}

bool
DB::initialize()
{
    QString studiengang = "CREATE TABLE studiengang ("
                          "     studiengangID INTEGER PRIMARY KEY,"
                          "     schwerpunkt TEXT NOT NULL CHECK(schwerpunkt IN"
                          "                 ('IN-AI','IN-IS','IN-MI','IN-SE','MIN','MLD')),"
                          "     abschluss TEXT NOT NULL CHECK(abschluss IN"
                          "               ('Bachelor', 'Master')),"
                          "     UNIQUE(schwerpunkt, abschluss));";

    QString arbeit = "CREATE TABLE arbeit ("
            " arbeitID INTEGER PRIMARY KEY,"
            " titel TEXT NOT NULL,"
            " status TEXT,"
            " erlaeuterung TEXT,"
            " studiengangID INTEGER,"
            " dozentID INTEGER,"
            " studentID INTEGER,"
            " UNIQUE(titel, dozentID, studentID),"
            " FOREIGN KEY(studiengangID) REFERENCES "
                " studiengang (studiengangID) ON DELETE CASCADE,"
            " FOREIGN KEY(dozentID) REFERENCES "
                " nutzer (nutzerID) ON DELETE CASCADE,"
            " FOREIGN KEY(studentID) REFERENCES "
                " nutzer (nutzerID) ON DELETE CASCADE);";

    QString stichworte = "CREATE TABLE stichworte ("
            " arbeitID INTEGER PRIMARY KEY,"
            " stichwort TEXT NOT NULL,"
            " FOREIGN KEY (arbeitID) REFERENCES"
            "    arbeit (arbeitID) ON DELETE CASCADE);";

    QString nutzer = "CREATE TABLE nutzer ("
                " nutzerID INTEGER PRIMARY KEY,"
                " VName TEXT NOT NULL,"
                " NName TEXT NOT NULL,"
                " email TEXT NOT NULL UNIQUE,"
                " passwort TEXT NOT NULL,"
                " rolle INTEGER NOT NULL,"
                " active INTEGER NOT NULL);";

    QString abschlussarbeit = "CREATE TABLE abschlussarbeit ("
            " arbeitID INTEGER PRIMARY KEY,"
            " beginn DATE NOT NULL,"
            " ende DATE NOT NULL,"
            " firma TEXT,"
            " FOREIGN KEY (arbeitID) REFERENCES"
            "    arbeit (arbeitID) ON DELETE CASCADE);";

    QString projektarbeit = "CREATE TABLE projektarbeit ("
            " arbeitID INTEGER PRIMARY KEY,"
            " semester INTEGER NOT NULL,"
            " FOREIGN KEY (arbeitID) REFERENCES"
            "   arbeit (arbeitID) ON DELETE CASCADE);";

    QSqlDatabase db = QSqlDatabase::database(); // retrieve database
    QSqlQuery query;

    if( !db.tables().contains("studiengang") ) {
        qDebug() << "Insert table studiengang into database";
        log("Query", studiengang);
        query.exec(studiengang);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::initialize: " << query.lastError().text();
            return false;
        }
    }

    if( !db.tables().contains("arbeit") ) {
        qDebug() << "Insert table arbeit into database";
        log("Query", arbeit);
        query.exec(arbeit);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::initialize: " << query.lastError().text();
            return false;
        }
    }

    if( !db.tables().contains("stichworte") ) {
        qDebug() << "Insert table stichworte into database";
        log("Query", stichworte);
        query.exec(stichworte);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::initialize: " << query.lastError().text();
            return false;
        }
    }

    if( !db.tables().contains("nutzer") ) {
        qDebug() << "Insert table nutzer into database";
        log("Query", nutzer);
        query.exec(nutzer);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::initialize: " << query.lastError().text();
            return false;
        }
    }

    if( !db.tables().contains("abschlussarbeit") ) {
        qDebug() << "Insert table abschlussarbeit into database";
        log("Query", abschlussarbeit);
        query.exec(abschlussarbeit);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::initialize: " << query.lastError().text();
            return false;
        }
    }

    if( !db.tables().contains("projektarbeit") ) {
        qDebug() << "Insert table projektarbeit into database";
        log("Query", projektarbeit);
        query.exec(projektarbeit);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::initialize: " << query.lastError().text();
            return false;
        }
    }

    qDebug() << "Blablub hat funktioniert:";
    return true;
}

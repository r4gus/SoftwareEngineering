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

bool
DB::initialize()
{
    QString studiengang = "CREATE TABLE studiengang ("
                          "     studiengangID INTEGER NOT NULL UNIQUE,"
                          "     schwerpunkt TEXT NOT NULL CHECK(schwerpunkt IN"
                          "                 ('IN-AI','IN-IS','IN-MI','IN-SE','MIN','MLD')),"
                          "     abschluss TEXT NOT NULL CHECK(abschluss IN"
                          "               ('Bachelor', 'Master')),"
                          "     PRIMARY KEY(studiengangID),"
                          "     UNIQUE(schwerpunkt, abschluss));";

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

    return true;
}

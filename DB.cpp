#include "DB.h"

#include <QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <fstream>

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
    std::ifstream conf;                                     // file stream for config file specified in _conf_path
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // create new SQlite database object
    string line;                                            // string to hold the path to the database
    QString name;
    QString path;

    // get username
    name = qgetenv("USER");
    if(name.isEmpty()) name = qgetenv("USERNAME");
    // get path to config file
    path = qgetenv("SE_CONFIG");
    qDebug() << path;

    /* Set the path to the config file using the
     * SE_CONFIG environment variable.
     *
     * If no path is specified, the default config path is used.
     */
    if(path.isEmpty()) {
        this->_conf_path = "C:\\Users\\" + name + "\\Documents\\db.conf";
    } else {
        this->_conf_path = path;
    }
    const QString DEFAULT_DB_PATH = "C:\\Users\\" + name + "\\Documents\\db.sqlite";

    conf.open(this->_conf_path.toStdString());          // open config file

    /* Read the first line of the config file and
     * set it as the path for the database to open.
     *
     * If the config file couldn't be opened, the standard path
     * 'DEFAULT_DB_PATH' is used.
    */
    if(conf.is_open()) {
        getline(conf, line);
        this->_db_path = QString::fromStdString(line);
        conf.close();
    } else {
        this->_db_path = DEFAULT_DB_PATH;
    }

    db.setDatabaseName(this->_db_path);   // set name of the database

    if( db.open() ) {
        qDebug() << "Data base set-up successfull.";
    } else {
        qDebug() << db.lastError();
    }

    initialize(db); // init database
}

DB::~DB()
{}

/*
int
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

    return query.lastInsertId().toInt();
}
*/

int
DB::add(Studiengang &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database(); // retrieve database

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    query.prepare("INSERT INTO studiengang (schwerpunkt, abschluss) "
                  "VALUES (:schwerpunk, :abschluss)");
    query.bindValue(":schwerpunk", s.schwerpunkt());
    query.bindValue(":abschluss", s.abschluss());

    if( !query.exec() ) {
        if(query.lastError().nativeErrorCode() == 19) { // 19 = SQLite constraint violated
            // check if object does already exist and retrieve it
        }
        qDebug() << query.lastError().nativeErrorCode();
        throw DatabaseTransactionError(query.lastError().text());
    }

    return query.lastInsertId().toInt();
}

int
DB::add(SonstigesProjekt &s)
{
    QSqlQuery query;
    QString qs;
    QSqlDatabase db = QSqlDatabase::database(); // retrieve database

    if (!db.isValid())  throw InvalidDatabaseError("Invalid Database connection");

    qs = "INSERT INTO arbeit (titel, status, erlaeuterung, studiengang, dozentid, studentid) VALUES (" +
         db.driver()->escapeIdentifier(s.titel(), QSqlDriver::FieldName) + ", " +
         db.driver()->escapeIdentifier(QString(s.abgeschlossen()), QSqlDriver::FieldName) + ", " +
         db.driver()->escapeIdentifier(s.erlaeuterung(), QSqlDriver::FieldName) + ", " +
         db.driver()->escapeIdentifier(QString(s.studiengang()->id()), QSqlDriver::FieldName) + ", " +
         "0, 0)" +
         ");";

    log("Query", qs);
    if( !query.exec(qs) ) {
        throw DatabaseTransactionError(query.lastError().text());
    }

    return query.lastInsertId().toInt();
}

/*!
 * \brief DB::initialize Creates all required tables if they do not exist.
 * \param db The database to operate on
 * \return true on success, false otherwise
 */
bool
DB::initialize(QSqlDatabase &db)
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

    QSqlQuery query;

    if( !db.isValid() ) return false;

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

    return true;
}

/*!
 * \brief DB::test is used to populate the database tables with test data.
 * \param db The database to populate
 * \return true on success, false otherwise
 */
bool
DB::test(QSqlDatabase &db)
{
    QSqlQuery query;
    if(!db.isValid()) return false;


    // add studiengänge
    QVector<QString> s_names = {"IN-AI", "IN-IS", "IN-MI", "IN-SE", "MIN", "MLD"};
    QVector<QString> s_types = {"Bachelor", "Master"};
    for(auto &sn : s_names) {
        for(auto &st : s_types) {
            qDebug() << sn << " : " << st;
            query.prepare("INSERT INTO studiengang (schwerpunkt, abschluss) "
                          "VALUES (:schwerpunk, :abschluss)");
            query.bindValue(":schwerpunk", sn);
            query.bindValue(":abschluss", st);
            query.exec();


            if( query.lastError().isValid() ) {
                qDebug() << "Database error in DB::test: " << query.lastError().text();
                return false;
            }
        }
    }


    return true;
}

/*!
 * \brief DB::clean is used to drop all existing tables of a database.
 * \param db The database to empty
 * \return true on success, false otherwise
 *
 * The method goes through all tables and drops every one of them.
 *
 */
bool DB::clean(QSqlDatabase &db)
{
    QSqlQuery query;
    QStringList tables = db.tables();

    if(!db.isValid()) return false;

    for(auto &t : tables) {
        qDebug() << t;
        query.exec("DROP TABLE " + t);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::clean: " << query.lastError().text();
            //return false;
        }
    }

    return true;
}

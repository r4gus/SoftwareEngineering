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

/*!
 * \brief DB::add Adds an Studiengang object to the database.
 * \param s The object to add
 * \return the ID of the Studiengang object inserted.
 *
 * The ID is set by the database itself. If the object does already exist, the
 * id of that object is returned to the caller.
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
        // check if object does already exist and retrieve it
        QString query_string = "schwerpunkt = '" + s.schwerpunkt() + "' and abschluss = '" + s.abschluss() + "'";
        std::vector<Studiengang> vec = Studiengang::query(query_string);

        if(vec.size() > 0) return vec[0].id();

        throw DatabaseTransactionError(query.lastError().text());
    }

    return query.lastInsertId().toInt();
}

int
DB::add(Nutzer &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    query.prepare("INSERT INTO nutzer (VName, NName, email, password, salt, workFactor, role, active) "
                  "VALUES (:vname, :nname, :email, :password, :salt, :workFactor, :role, :active)");
    query.bindValue(":vname", s.vname());
    query.bindValue(":nname", s.nname());
    query.bindValue(":email", s.email());
    query.bindValue(":password", s.password_hash());
    query.bindValue(":salt", s.password_salt());
    query.bindValue(":workFactor", s.personal_work_factor());
    query.bindValue(":role", s.role());
    query.bindValue(":active", s.active());

    if(!query.exec()) {
        // check if object does already exist and retrieve it
        QString query_string = "email = '" + s.email() +"'";
        std::vector<Nutzer> vec = Nutzer::query(query_string);

        if(vec.size() > 0) return vec[0].id();

        throw DatabaseTransactionError(query.lastError().text());
    }

    return query.lastInsertId().toInt();
}

int
DB::add(SonstigesProjekt &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();
    int studiengang_id, dozent_id, student_id, ret_val;

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");


    // add dozent to database if id <= -1
    try {
        if(s.professor().id() <= -1) {
            Nutzer p = s.professor();
            dozent_id = DB::session().add(p);
        } else {
            dozent_id = s.professor().id();
        }
    } catch(exception &e) {
        log("error", "in add(ARBEIT): While potentially inserting dozent into database");
        throw e;
    }

    // add studiengang to database if id <= -1
    try {
        if(s.studiengang().id() <= -1) {
            Studiengang stud = s.studiengang();
            studiengang_id = DB::session().add(stud);
        } else {
            studiengang_id = s.studiengang().id();
        }
    } catch(exception &e) {
        log("error", "in add(ARBEIT): While potentially inserting studiengang into database");
        throw e;
    }


    // add student to database if id <= -1
    try {
        if(s.bearbeiter().id() <= -1) {
            Nutzer b = s.bearbeiter();
            student_id= DB::session().add(b);
        } else {
            student_id = s.bearbeiter().id();
        }
    } catch(exception &e) {
        log("error", "in add(ARBEIT): While potentially inserting student into database");
        throw e;
    }

    query.prepare("INSERT INTO arbeit (titel, status, erlaeuterung, studiengangID, dozentID, studentID) "
                  "VALUES (:titel, :status, :erl, :studiengang_id, :d_id, :s_id)");
    query.bindValue(":titel", s.titel());
    query.bindValue(":status", s.abgeschlossen());
    query.bindValue(":erl", s.erlaeuterung());
    query.bindValue(":s_id", student_id);
    query.bindValue(":studiengang_id", studiengang_id);
    query.bindValue(":d_id", dozent_id);

    if(!query.exec()) {
        // check if object does already exist and retrieve it
        QString query_string = "titel = '" + s.titel() +"' and dozentID = " + s.professor().id() +
                " and studentID = " + s.bearbeiter().id() + " ";
        std::vector<SonstigesProjekt> vec = SonstigesProjekt::query(query_string);

        if(vec.size() > 0) ret_val = vec[0].id();
        else throw DatabaseTransactionError(query.lastError().text());
    } else {
        ret_val = query.lastInsertId().toInt();

    }

    for(auto e : s.stichwortliste()) {
        QString str = "INSERT INTO stichworte (arbeitID, stichwort) VALUES (" + QString::number(ret_val) + ", '" + e + "');";
        query.exec(str);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in add(SonstigesProjekt) [stichwortliste]: " << query.lastError().text();
        }
    }

    return ret_val;
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
            " arbeitID INTEGER,"
            " stichwort TEXT NOT NULL,"
            " FOREIGN KEY (arbeitID) REFERENCES"
            "    arbeit (arbeitID) ON DELETE CASCADE);";

    QString nutzer = "CREATE TABLE nutzer ("
                " nutzerID INTEGER PRIMARY KEY,"
                " VName TEXT NOT NULL,"
                " NName TEXT NOT NULL,"
                " email TEXT NOT NULL UNIQUE,"
                " password TEXT NOT NULL,"
                " salt TEXT NOT NULL,"
                " workFactor INTEGER NOT NULL,"
                " role INTEGER NOT NULL,"
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
            qDebug() << "Insert: " << sn << " : " << st;
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

    // ad nutzer
    qDebug() << "Add Nutzer ---------------------------";
    Nutzer nutzer1("Roland", "Dietrich", "rd@hs.aa", Nutzer::Role::dozent);
    Nutzer nutzer2("Christian", "Heinlein", "km@hs.aa", Nutzer::Role::dozent);
    Nutzer nutzer3("Detlef", "Küpper", "dk@hs.aa", Nutzer::Role::dozent);
    Nutzer nutzer4("David", "Sugar", "david.sugar@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer5("Max", "Maier", "max.maier@studmail@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer6("Moritz", "Busch", "moritz.busch@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer7("Frieda", "Bolte", "frieda.bolte@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer8("Paul", "Lämpel", "paul.lämpel@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer9("Franz", "Groß", "franz.groß@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer10("Fritz", "Klein", "fritz.klein@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer11("Hans", "Schmidt", "hans.schmidt@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer12("Heiner", "Müller", "heiner.müller@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer13("Maria", "Kurz", "maria.kurz@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer14("Ulrike", "Lang", "ulrike.lang@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer15("Ulrich", "Frei", "ulrich.frei@studmail.htw-aalen.de", Nutzer::Role::student);
    Nutzer nutzer16("Martin", "Fromm", "martin.fromm@studmail.htw-aalen.de", Nutzer::Role::student);
    nutzer1.set_password("RD");
    nutzer2.set_password("CH");
    nutzer3.set_password("DK");

    try {
        DB::session().add(nutzer1);
        DB::session().add(nutzer2);
        DB::session().add(nutzer3);
        DB::session().add(nutzer4);
        DB::session().add(nutzer5);
        DB::session().add(nutzer6);
        DB::session().add(nutzer7);
        DB::session().add(nutzer8);
        DB::session().add(nutzer9);
        DB::session().add(nutzer10);
        DB::session().add(nutzer11);
        DB::session().add(nutzer12);
        DB::session().add(nutzer13);
        DB::session().add(nutzer14);
        DB::session().add(nutzer15);
        DB::session().add(nutzer16);
    } catch(exception &e) {
        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::test: " << query.lastError().text();
            return false;
        }
    }

    // ad arbeit
    qDebug() << "Add Arbeit ----------------------------";
    QVector<QString> list1 = {"FAT32", "FAT16", "FAT12"};
    Studiengang s("IN-IS", "Bachelor");
    SonstigesProjekt arbeit1("FAT Data Recovery", list1, false, "PBS is your friend, rebuild the cluster chain!");
    arbeit1.setStudiengang(s);
    arbeit1.setProfessor(nutzer2);
    arbeit1.setBearbeiter(nutzer4);


    QVector<QString> list_e_motion = {"Automotive", "E-Mobilität"};
    Studiengang s_e_motion("IN-SE", "Bachelor");
    SonstigesProjekt e_motion("E-Motion Motorsteuerung", list_e_motion, false, "Wahlprojekt IN4, durchgeführt beim E-Motion-Rennteam der Hochschule");
    e_motion.setStudiengang(s_e_motion);
    e_motion.setProfessor(nutzer1);
    e_motion.setBearbeiter(nutzer8);

    try {
        DB::session().add(arbeit1);
        DB::session().add(e_motion);
    } catch(exception &e) {
        if( query.lastError().isValid() ) {
            qDebug() << "Database error in DB::test: " << query.lastError().text();
            return false;
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
            return false;
        }
    }

    return true;
}

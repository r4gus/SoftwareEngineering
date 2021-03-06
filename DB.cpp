#include "DB.h"

#include <QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <fstream>
#include <QtCore/QFile>

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
    this->_conf_path = ":/db.conf";
    QFile styleFile(this->_conf_path);
    if (styleFile.open(QFile::ReadOnly)) {
        QTextStream textStream(&styleFile);
        this->_db_path = textStream.readLine();
        styleFile.close();
    }

    db.setDatabaseName(this->_db_path);   // set name of the database

    if( db.open() ) {
        qDebug() << "Data base set-up successfull.";
    } else {
        qDebug() << db.lastError();
    }
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
DB::update(Nutzer &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    query.prepare("UPDATE nutzer SET VName = :vname, NName = :nname, email = :email, "
                  "password = :password, salt = :salt, workFactor = :workFactor, role = :role, "
                  "active = :active WHERE nutzerID = :id ");
    query.bindValue(":vname", s.vname());
    query.bindValue(":nname", s.nname());
    query.bindValue(":email", s.email());
    query.bindValue(":password", s.password_hash());
    query.bindValue(":salt", s.password_salt());
    query.bindValue(":workFactor", s.personal_work_factor());
    query.bindValue(":role", s.role());
    query.bindValue(":active", s.active());
    query.bindValue(":id", s.id());

    if(!query.exec()) {
        throw DatabaseTransactionError(query.lastError().text());
    }

    return s.id();
}

bool
DB::remove(Nutzer &s)
{
    bool ret = false;
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");
    if(s.id() < 0) return ret; // no valid id

    query.prepare("DELETE FROM nutzer WHERE nutzerID = " + QString::number(s.id()));

    if(query.exec()) ret = true;

    return ret;
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


int
DB::update(SonstigesProjekt &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();
    int ret_val = s.id();
    int dozent_id, student_id;

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    // add dozent to database if id <= -1
    try {
        Nutzer p = s.professor();
        if(s.professor().id() <= -1) {
            dozent_id = DB::session().add(p);
        } else {

            dozent_id = DB::session().update(p);
        }
    } catch(exception &e) {
        log("error", "in update(SonstigesProjekt): While potentially inserting or updating dozent into database");
        throw e;
    }

    // add student to database if id <= -1
    try {
        Nutzer b = s.bearbeiter();
        if(s.bearbeiter().id() <= -1) {
            student_id= DB::session().add(b);
        } else {
            student_id = DB::session().update(b);
        }
    } catch(exception &e) {
        log("error", "in add(ARBEIT): While potentially inserting or updating student into database");
        throw e;
    }

    query.prepare("UPDATE arbeit SET titel = :titel, status = :status, erlaeuterung = :erl, "
                  "studiengangID = :studiengang_id, dozentID = :d_id, studentID = :s_id "
                  "WHERE arbeitID = :arbeitID");
    query.bindValue(":titel", s.titel());
    query.bindValue(":status", s.abgeschlossen());
    query.bindValue(":erl", s.erlaeuterung());
    query.bindValue(":studiengang_id", s.studiengang().id());
    query.bindValue(":d_id", dozent_id);
    query.bindValue(":s_id", student_id);
    query.bindValue(":arbeitID", s.id());

    if(!query.exec()) {
        throw DatabaseTransactionError(query.lastError().text());
    }

    query.prepare("DELETE FROM stichworte WHERE arbeitID = :arbeitID");
    query.bindValue(":arbeitID", s.id());
    query.exec();

    for(auto e : s.stichwortliste()) {
        QString str = "INSERT INTO stichworte (arbeitID, stichwort) VALUES (" + QString::number(s.id()) + ", '" + e + "');";
        query.exec(str);

        if( query.lastError().isValid() ) {
            qDebug() << "Database error in add(SonstigesProjekt) [stichwortliste]: " << query.lastError().text();
        }
    }

    return ret_val;
}

bool
DB::remove(SonstigesProjekt &s)
{
    bool ret = false;
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");
    if(s.id() < 0) return ret; // no valid id

    query.exec("PRAGMA foreign_keys = ON");
    query.prepare("DELETE FROM arbeit WHERE arbeitID = " + QString::number(s.id()));

    if(query.exec()) ret = true;

    return ret;
}

int
DB::add(Projektarbeit &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();
    int id = -1;

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    try {
        SonstigesProjekt p = (SonstigesProjekt) s;
        id = DB::session().add(p);
    } catch(exception &e) {
        throw e;
    }

    query.prepare("INSERT INTO projektarbeit (arbeitID, semester) VALUES (:id, :semester)");
    query.bindValue(":id", id);
    query.bindValue(":semester", s.semester());

    if(!query.exec()) {
        log("error", query.lastError().text());
        throw DatabaseTransactionError(query.lastError().text());
    }

    return id;
}

int
DB::update(Projektarbeit &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    try {
        SonstigesProjekt p = (SonstigesProjekt) s;
        DB::session().update(p);
    } catch(exception &e) {
        throw e;
    }

    query.prepare("UPDATE projektarbeit SET semester = :semester WHERE arbeitID = :id");
    query.bindValue(":id", s.id());
    query.bindValue(":semester", s.semester());

    if(!query.exec()) {
        log("error", query.lastError().text());
        throw DatabaseTransactionError(query.lastError().text());
    }

    return s.id();
}

bool
DB::remove(Projektarbeit &s)
{
    SonstigesProjekt sons = (SonstigesProjekt) s;
    return DB::session().remove(sons);
}

int
DB::add(Abschlussarbeit &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();
    int id = -1;

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    try {
        SonstigesProjekt p = (SonstigesProjekt) s;
        id = DB::session().add(p);
    } catch(exception &e) {
        throw e;
    }

    query.prepare("INSERT INTO abschlussarbeit (arbeitID, beginn, ende, firma) VALUES (:id, :beginn, :ende, :firma);");
    query.bindValue(":id", id);
    query.bindValue(":beginn", s.begin());
    query.bindValue(":ende", s.end());
    query.bindValue(":firma", s.firma());

    if(!query.exec()) {
        log("error", query.lastError().text());
        throw DatabaseTransactionError(query.lastError().text());
    }

    return id;
}

int
DB::update(Abschlussarbeit &s)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();

    if( !db.isValid() ) throw InvalidDatabaseError("Invalid database connection.");

    try {
        SonstigesProjekt p = (SonstigesProjekt) s;
        DB::session().update(p);
    } catch(exception &e) {
        throw e;
    }

    query.prepare("UPDATE abschlussarbeit SET beginn = :beginn, ende = :ende, firma = :firma WHERE arbeitID = :id");
    query.bindValue(":id", s.id());
    query.bindValue(":beginn", s.begin());
    query.bindValue(":ende", s.end());
    query.bindValue(":firma", s.firma());

    if(!query.exec()) {
        log("error", query.lastError().text());
        throw DatabaseTransactionError(query.lastError().text());
    }

    return s.id();
}

bool
DB::remove(Abschlussarbeit &s)
{
    SonstigesProjekt sons = (SonstigesProjekt) s;
    return DB::session().remove(sons);
}



void addStudiengang() {
    QSqlQuery query;

    QStringList s_names = {"IN-AI", "IN-IS", "IN-MI", "IN-SE", "MIN", "MLD"};
    QStringList s_types = {"Bachelor", "Master"};
    for(auto &sn : s_names) {
        for(auto &st : s_types) {
            qDebug() << "Insert: " << sn << " : " << st;
            query.prepare("INSERT INTO studiengang (schwerpunkt, abschluss) "
                          "VALUES (:schwerpunk, :abschluss)");
            query.bindValue(":schwerpunk", sn);
            query.bindValue(":abschluss", st);
            try {
                query.exec();
            } catch (exception&) {

            }


            if( query.lastError().isValid() ) {
                qDebug() << "Database error in DB::test: " << query.lastError().text();
            }
        }
    }
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

    if(Nutzer::query("nutzer.role = " + QString::number(Nutzer::Role::administrator)).empty()) {
        Nutzer admin("admin", "admin", "Admin", Nutzer::administrator);
        admin.set_password("nimdA");
        DB::session().add(admin);
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
    // SonstigesProjekt
    QStringList list1 = {"FAT32", "FAT16", "FAT12"};
    Studiengang s("IN-IS", "Bachelor");
    SonstigesProjekt arbeit1("FAT Data Recovery", list1, false, "PBS is your friend, rebuild the cluster chain!");
    arbeit1.setStudiengang(s);
    arbeit1.setProfessor(nutzer2);
    arbeit1.setBearbeiter(nutzer4);


    QStringList list_e_motion = {"Automotive", "E-Mobilität"};
    Studiengang s_e_bachelor("IN-SE", "Bachelor");
    SonstigesProjekt e_motion("E-Motion Motorsteuerung", list_e_motion, false, "Wahlprojekt IN4, durchgeführt beim E-Motion-Rennteam der Hochschule");
    e_motion.setStudiengang(s_e_bachelor);
    e_motion.setProfessor(nutzer1);
    e_motion.setBearbeiter(nutzer8);


    // Projektarbeit
    QStringList list_sep_1 = {"Softwareentwicklung", "Softwarearchitektur"};
    Projektarbeit sep_1("Schichtenarchitektur mit Qt", list_sep_1, false, "Beispiel Seminarverwaltung");
    sep_1.setStudiengang(s_e_bachelor);
    sep_1.setProfessor(nutzer1);
    sep_1.setBearbeiter(nutzer7);
    sep_1.setSemester(4);

    QStringList list_mip_1 = {"Mensch-Maschine-Kommunikation"};
    Studiengang mi_bachelor("IN-MI", "Bachelor");
    Projektarbeit mip_1("Gestaltung von grafischen Benutzungsoberflächen", list_mip_1, false, "Demobeispiele mit Qt5");
    mip_1.setStudiengang(mi_bachelor);
    mip_1.setProfessor(nutzer3);
    mip_1.setBearbeiter(nutzer11);
    mip_1.setSemester(3);

    QStringList list_mp_1 = {"SQL", "Relationsmodell", "MongoDB", "MySQL"};
    Studiengang s_e_master("IN-SE", "Master");
    Projektarbeit mp_1("NoSql-Datenbanken vs. Relationale Datenbanken - Sinnvolle Einsatzszenarien", list_mp_1, true, "");
    mp_1.setStudiengang(s_e_master);
    mp_1.setProfessor(nutzer3);
    mp_1.setBearbeiter(nutzer12);
    mp_1.setSemester(9);

    QStringList list_sep_2 = {"Haskell", "Lisp (ist nicht funktional argh...)", "MOSTflexiPL", "QuickSort", "Insert-Sort", "Tree-Sort"};
    Projektarbeit sep_2("Funktionale Programmiersprachen im Vergleich", list_sep_2, false, "Anwendungsbeispiele verschiedener Suchalgorithmen");
    sep_2.setStudiengang(s_e_bachelor);
    sep_2.setProfessor(nutzer2);
    sep_2.setBearbeiter(nutzer15);
    sep_2.setSemester(6);

    QStringList list_sep_3 = {"Stack", "List", "Queue", "Tree", "Graph"};
    Projektarbeit sep_3("Abstrakte Datentypen in MOSTflexiPL", list_sep_3, true, "Programmbibliothek mit klassischen ADTs");
    sep_3.setStudiengang(s_e_bachelor);
    sep_3.setProfessor(nutzer2);
    sep_3.setBearbeiter(nutzer16);
    sep_3.setSemester(2);

    // Abschlussarbeit
    QStringList list_ba_1 = {"Softwareentwicklung", "Modellierung"};
    QDate begin(2020, 9, 1);
    QDate end(2021, 3, 1);
    Abschlussarbeit ba_1("Codegenerierung mit Enterprise Architect", list_ba_1, false, "");
    ba_1.setEnd(end);
    ba_1.setBegin(begin);
    ba_1.setStudiengang(s_e_bachelor);
    ba_1.setProfessor(nutzer1);
    ba_1.setBearbeiter(nutzer5);

    QStringList list_ba_2 = {"Softwareentwicklung", "Konfigurationsmanagement", "SQL"};
    QDate begin_ba_2(2020, 9, 1);
    QDate end_ba_2(2021, 3, 1);
    Abschlussarbeit ba_2("Datenbanken für die Software-Versionierung", list_ba_2, false, "");
    ba_2.setEnd(end_ba_2);
    ba_2.setBegin(begin_ba_2);
    ba_2.setStudiengang(s_e_bachelor);
    ba_2.setProfessor(nutzer3);
    ba_2.setBearbeiter(nutzer9);

    QStringList list_ba_3 = {"Funktionale Programmiersprachen"};
    QDate begin_ba_3(2016, 9, 1);
    QDate end_ba_3(2017, 3, 30);
    Abschlussarbeit ba_3("Ein 2-Pass-Compiler für MOSTflexiPL", list_ba_3, false, "Abgebrochen");
    ba_3.setEnd(end_ba_3);
    ba_3.setBegin(begin_ba_3);
    ba_3.setStudiengang(s_e_bachelor);
    ba_3.setProfessor(nutzer2);
    ba_3.setBearbeiter(nutzer13);

    QDate begin_ba_4(2017, 9, 1);
    QDate end_ba_4(2018, 3, 30);
    Abschlussarbeit ba_4("Ein 2-Pass-Compiler für MOSTflexiPL", list_ba_3, true, "");
    ba_4.setEnd(end_ba_4);
    ba_4.setBegin(begin_ba_4);
    ba_4.setStudiengang(s_e_master);
    ba_4.setProfessor(nutzer2);
    ba_4.setBearbeiter(nutzer14);

    QStringList list_ma_1 = {"Automotive", "Softwaretest"};
    QDate begin_ma_1(2019, 9, 1);
    QDate end_ma_1(2020, 3, 1);
    Abschlussarbeit ma_1("Testautomatisierung für Getriebesteuerungen", list_ma_1, true, "");
    ma_1.setEnd(end_ma_1);
    ma_1.setBegin(begin_ma_1);
    ma_1.setStudiengang(s_e_master);
    ma_1.setProfessor(nutzer1);
    ma_1.setBearbeiter(nutzer6);
    ma_1.setFirma("Bosch");

    QStringList list_ma_2 = {"Künstliche Intelligenz"};
    QDate begin_ma_2(2020, 3, 1);
    QDate end_ma_2(2020, 8, 28);
    Studiengang ai_master("IN-AI", "Master");
    Abschlussarbeit ma_2("Intelligent lernende Agenten", list_ma_2, false, "");
    ma_2.setEnd(end_ma_2);
    ma_2.setBegin(begin_ma_2);
    ma_2.setStudiengang(ai_master);
    ma_2.setProfessor(nutzer3);
    ma_2.setBearbeiter(nutzer10);

    try {
        DB::session().add(arbeit1);
        DB::session().add(e_motion);
        DB::session().add(mip_1);
        DB::session().add(mp_1);
        DB::session().add(sep_1);
        DB::session().add(sep_2);
        DB::session().add(sep_3);
        DB::session().add(ba_1);
        DB::session().add(ba_2);
        DB::session().add(ba_3);
        DB::session().add(ba_4);
        DB::session().add(ma_1);
        DB::session().add(ma_2);
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

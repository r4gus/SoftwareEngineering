#include "sonstigesprojekt.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "sstream"
#include <thread>
#include "DB.h"
#include "nutzer.h"
#include "studiengang.h"

/*!
* \brief query_all returns a vector with all objects from the database.
* \return A vector of SonstigesProjekt objects
*/
vector<SonstigesProjekt>
SonstigesProjekt::query_all()
{
    QString query_string = "";
    return SonstigesProjekt::query(query_string);
}

/*!
 * \brief query Get a subset of the database entries as objects
 * \param s The condition string
 * \return A vector of SonstigesProjekt objects
 *
 * The method expects a string of type: "key1 >= 'val1' and key2 = 'val2' or ..."
 *
 * The method acts like query_all() if an empty string is provided.
 */
vector<SonstigesProjekt>
SonstigesProjekt::query(QString s)
{
    vector<SonstigesProjekt> vec;

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery arbeit_query;
    QSqlQuery sw_query;
    QString qs;

    if(!db.isValid()) throw InvalidDatabaseError();

    qs = "SELECT DISTINCT arbeit.arbeitID, arbeit.titel, arbeit.status, arbeit.erlaeuterung, arbeit.studiengangID, arbeit.dozentID, arbeit.studentID "
    "FROM arbeit "
    "LEFT JOIN nutzer student on arbeit.studentID = student.nutzerID "
    "LEFT JOIN nutzer prof on arbeit.dozentID = prof.nutzerID "
    "JOIN stichworte s on arbeit.arbeitID = s.arbeitID ";
    if(!s.isEmpty()) qs += " WHERE " + s;
    qs += " EXCEPT "
    "SELECT DISTINCT arbeit.arbeitID, arbeit.titel, arbeit.status, arbeit.erlaeuterung, arbeit.studiengangID, arbeit.dozentID, arbeit.studentID "
    "FROM arbeit "
    "NATURAL JOIN "
    "(select projektarbeit.arbeitID FROM projektarbeit "
    "UNION ALL "
    "SELECT abschlussarbeit.arbeitID FROM abschlussarbeit) ";
    qs += ";";

    log("Query", qs);
    if( !arbeit_query.exec(qs) ) {
        throw DatabaseTransactionError(arbeit_query.lastError().text());
    }

    while(arbeit_query.next()) {
        int id = arbeit_query.value(0).toInt();
        QString titel = arbeit_query.value(1).toString();
        bool status = arbeit_query.value(2).toBool();
        QString erl = arbeit_query.value(3).toString();
        int studg_id = arbeit_query.value(4).toInt();
        int doz_id = arbeit_query.value(5).toInt();
        int student_id = arbeit_query.value(6).toInt();

        SonstigesProjekt sons(id, titel, status, erl);

        QString studiengang_qs = "studiengangID = " + QString::number(studg_id);
        QString dozent_qs = "nutzerID = " + QString::number(doz_id);
        QString student_qs = "nutzerID = " + QString::number(student_id);

        vector<Studiengang> studiengang = Studiengang::query(studiengang_qs);
        vector<Nutzer> dozent = Nutzer::query(dozent_qs);
        vector<Nutzer> student = Nutzer::query(student_qs);

        if(studiengang.size() > 0) {
            sons.setStudiengang(studiengang[0]);
        }

        if(dozent.size() > 0) {
            sons.setProfessor(dozent[0]);
        }

        if(student.size() > 0) {
            sons.setBearbeiter(student[0]);
        }

        QString stichwort_qs = "SELECT stichwort FROM stichworte WHERE arbeitID = " + QString::number(id);
        QStringList sw_vec;
        log("Query", stichwort_qs);
        if( !sw_query.exec(stichwort_qs) ) {
            log("error", sw_query.lastError().text());
        }
        while(sw_query.next()) {
            sw_vec.push_back(sw_query.value(0).toString());
        }
        sons.setStichwortliste(sw_vec);

        vec.push_back(sons);
    }

    return vec;
}

int SonstigesProjekt::id() const
{
    return _id;
}

void SonstigesProjekt::setId(int id)
{
    _id = id;
}

QString SonstigesProjekt::titel() const
{
    return _titel;
}

void SonstigesProjekt::setTitel(const QString &titel)
{
    _titel = titel;
}

QStringList SonstigesProjekt::stichwortliste() const
{
    return _stichwortliste;
}

void SonstigesProjekt::setStichwortliste(const QStringList &stichwortliste)
{
    _stichwortliste = stichwortliste;
}

bool SonstigesProjekt::abgeschlossen() const
{
    return _abgeschlossen;
}

void SonstigesProjekt::setAbgeschlossen(bool abgeschlossen)
{
    _abgeschlossen = abgeschlossen;
}

QString SonstigesProjekt::erlaeuterung() const
{
    return _erlaeuterung;
}

void SonstigesProjekt::setErlaeuterung(const QString &erlaeuterung)
{
    _erlaeuterung = erlaeuterung;
}

Studiengang SonstigesProjekt::studiengang() const
{
    return _studiengang;
}

void SonstigesProjekt::setStudiengang(const Studiengang &studiengang)
{
    _studiengang = studiengang;
}

Nutzer SonstigesProjekt::bearbeiter() const
{
    return _bearbeiter;
}

void SonstigesProjekt::setBearbeiter(const Nutzer &bearbeiter)
{
    _bearbeiter = bearbeiter;
}

Nutzer SonstigesProjekt::professor() const
{
    return _professor;
}

void SonstigesProjekt::setProfessor(const Nutzer &professor)
{
    _professor = professor;
}

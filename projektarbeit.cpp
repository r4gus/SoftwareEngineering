#include "projektarbeit.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <map>
#include "DB.h"


unsigned Projektarbeit::semester() const
{
    return _semester;
}

void Projektarbeit::setSemester(const unsigned &semester)
{
    _semester = semester;
}

/*!
* \brief query_all returns a vector with all objects from the database.
* \return A vector of Projektarbeit objects
*/
vector<Projektarbeit> Projektarbeit::query_all()
{
    QString query_string = "";
    return Projektarbeit::query(query_string);
}

/*!
 * \brief query Get a subset of the database entries as objects
 * \param s The condition string
 * \return A vector of Projektarbeit objects
 *
 * The method expects a string of type: "key1 >= 'val1' and key2 = 'val2' or ..."
 *
 * The method acts like query_all() if an empty string is provided.
 */
vector<Projektarbeit> Projektarbeit::query(QString s)
{
    vector<Projektarbeit> vec;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery arbeit_query;
    QSqlQuery sw_query;
    QString qs;

    if(!db.isValid()) throw InvalidDatabaseError();

    qs += "SELECT DISTINCT arbeit.arbeitID, arbeit.titel, arbeit.status, arbeit.erlaeuterung, arbeit.studiengangID, arbeit.dozentID, arbeit.studentID, semester "
          "FROM arbeit NATURAL JOIN projektarbeit "
          "LEFT JOIN nutzer student on arbeit.studentID = student.nutzerID "
          "LEFT JOIN nutzer prof on arbeit.dozentID = prof.nutzerID "
          "JOIN stichworte s on arbeit.arbeitID = s.arbeitID ";
          ;
    if(!s.isEmpty()) qs += " WHERE " + s;
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
        int semester = arbeit_query.value(7).toInt();

        Projektarbeit p(id, titel, status, erl);
        p.setSemester(semester);

        QString studiengang_qs = "studiengangID = " + QString::number(studg_id);
        QString dozent_qs = "nutzerID = " + QString::number(doz_id);
        QString student_qs = "nutzerID = " + QString::number(student_id);

        vector<Studiengang> studiengang = Studiengang::query(studiengang_qs);
        vector<Nutzer> dozent = Nutzer::query(dozent_qs);
        vector<Nutzer> student = Nutzer::query(student_qs);

        if(studiengang.size() > 0) {
            p.setStudiengang(studiengang[0]);
        }

        if(dozent.size() > 0) {
            p.setProfessor(dozent[0]);
        }

        if(student.size() > 0) {
            p.setBearbeiter(student[0]);
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
        p.setStichwortliste(sw_vec);

        vec.push_back(p);
    }

    return vec;
}

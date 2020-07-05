#include "sonstigesprojekt.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "DB.h"

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
 * \return A vector of Studiengang objects
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
    QSqlQuery query;
    QString qs;

    if(!db.isValid()) throw InvalidDatabaseError();

    qs += "SELECT arbeitID, titel, status, erlaeuterung, studiengangID, dozentID, studentID FROM nutzer ";
    if(!s.isEmpty()) qs += " WHERE " + s;
    qs += ";";

    log("Query", qs);
    if( !query.exec(qs) ) {
        throw DatabaseTransactionError(query.lastError().text());
    }

    return vec;
}

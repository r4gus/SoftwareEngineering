#include "studiengang.h"
#include "DB.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>

/*!
 * \brief key_val_split splits a komma seperated list of k:v pairs into a vector of tuples (k, v).
 * \param s The k:v string
 * \return a vector of tuples (key, value)
 *
 * The functione expects a string like: "key1=val1,key2=val2"
 *
 * If the pattern of the string differs from the pattern of the example above
 * the result is undefined.
 *
 * THIS FUNCTION IS DEPRECATED AND NO LONGER USED
 */
std::vector<std::tuple<QString, QString>>
key_val_split(QString &s)
{
    std::vector<std::tuple<QString, QString>> vec;
    QString lhs, delComma, delEq, key, val;
    QString rhs = s;
    QString::SectionFlag flag = QString::SectionSkipEmpty;

    while( rhs.size() > 0 ) {
        lhs = rhs.section(',', 0, 0, flag);
        rhs = rhs.section(',', 1, -1, flag);

        key = lhs.section('=', 0, 0);
        val = lhs.section('=', 1, 1);

        vec.push_back(std::make_tuple(key, val));
    }

    return vec;
}

/*!
 * \brief Studiengang::query_all returns a vector with all objects from the database.
 * \return A vector of Studiengang objects
 */
vector<Studiengang>
Studiengang::query_all()
{
    QString query_string = "";
    return Studiengang::query(query_string);
}

/*!
 * \brief Studiengang::query Get a subset of the database entries as objects
 * \param s The condition string
 * \return A vector of Studiengang objects
 *
 * The method expects a string of type: "key1 >= 'val1' and key2 = 'val2' or ..."
 *
 * The method acts like query_all() if an empty string is provided.
 */
vector<Studiengang>
Studiengang::query(QString &s)
{
    vector<Studiengang> vec;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString qs;

    if(!db.isValid()) throw InvalidDatabaseError();

    qs += "SELECT studiengangID, schwerpunkt, abschluss FROM studiengang ";
    if(!s.isEmpty()) qs += "WHERE " + s + ";";

    log("Query", qs);
    if( !query.exec(qs) ) {
        throw DatabaseTransactionError(query.lastError().text());
    }

    while( query.next() ) {
        int id = query.value(0).toInt();
        QString schwerpunkt = query.value(1).toString();
        QString abschluss = query.value(2).toString();
        vec.push_back(Studiengang(id, schwerpunkt, abschluss));
    }

    return vec;
}

Studiengang::operator QString() const
{
    return "{ schwerpunkt: " + _schwerpunkt + ", abschluss: " + _abschluss + "}";
}

std::ostream & operator << (std::ostream &o, const Studiengang &s)
{
    return o << QString(s).toLocal8Bit().constData();
}

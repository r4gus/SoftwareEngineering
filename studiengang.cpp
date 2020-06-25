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
    vector<Studiengang> vec;
    QSqlDatabase db = QSqlDatabase::database(); // retrieve database

    if(!db.isValid()) throw InvalidDatabaseError();

    QSqlQuery query("SELECT schwerpunkt, abschluss from studiengang;");
    while( query.next() ) {
        QString schwerpunkt = query.value(0).toString();
        QString abschluss = query.value(1).toString();
        vec.push_back(Studiengang(schwerpunkt, abschluss));
    }

    return vec;
}

/*!
 * \brief Studiengang::query Get a subset of the database entries as objects
 * \param s The condition string
 * \return A vector of Studiengang objects
 *
 * The method expects a string of type: "key1=val1,key2=val2"
 *
 * The method acts like query_all() if an empty string is provided.
 */
vector<Studiengang>
Studiengang::query(QString &s)
{
    vector<Studiengang> vec;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    std::vector<std::tuple<QString, QString>> kvs = key_val_split(s); // vector of k:v tuples
    QString qs;
    size_t i = 0;
    size_t l = kvs.size();

    if(!db.isValid()) throw InvalidDatabaseError();

    qs += "SELECT schwerpunkt, abschluss FROM studiengang ";
    if(l > 0) qs += "WHERE "; // only if arguments have been passed

    // build the where part of the string
    for(auto kv : kvs) {
        qs += std::get<0>(kv) + " = '" + std::get<1>(kv) + "' ";

        if(i < l - 1) qs += "AND "; // skip and for the end
        ++i;
    }
    qs += ";";

    log("Query", qs);
    if( !query.exec(qs) ) {
        throw DatabaseTransactionError(query.lastError().text());
    }

    while( query.next() ) {
        QString schwerpunkt = query.value(0).toString();
        QString abschluss = query.value(1).toString();
        vec.push_back(Studiengang(schwerpunkt, abschluss));
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

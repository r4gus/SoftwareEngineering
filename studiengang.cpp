#include "studiengang.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

vector<Studiengang>
Studiengang::query_all()
{
    vector<Studiengang> vec;
    QSqlDatabase db = QSqlDatabase::database(); // retrieve database

    QSqlQuery query("SELECT schwerpunkt, abschluss from studiengang;");
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

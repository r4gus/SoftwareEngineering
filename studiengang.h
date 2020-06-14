#ifndef STUDIENGANG_H
#define STUDIENGANG_H

#include <QString>
#include <vector>

using namespace std;

class Studiengang
{
public:
    int id() { return _id; }
    QString& abschluss() { return _abschluss; }
    QString& schwerpunkt() { return _schwerpunkt; }
    void set_abschluss(QString& abschluss) { _abschluss = abschluss; }
    void set_schwerpunkt(QString& schwerpunkt) { _schwerpunkt = schwerpunkt; }

    Studiengang(QString schwerpunkt, QString abschluss) :
        _schwerpunkt(schwerpunkt), _abschluss(abschluss){}

    Studiengang(int id, QString schwerpunkt, QString abschluss) :
        _id(id), _schwerpunkt(schwerpunkt), _abschluss(abschluss){}

    Studiengang(const Studiengang& rhs) :
        _id(rhs._id), _schwerpunkt(rhs._schwerpunkt), _abschluss(rhs._abschluss){}        // copy constructor

    static vector<Studiengang> query_all();

    operator QString () const;
private:
    int _id = -1;
    QString _schwerpunkt;
    QString _abschluss;
};

std::ostream & operator << (std::ostream &o, const Studiengang &s);

#endif // STUDIENGANG_H

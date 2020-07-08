#ifndef SONSTIGESPROJEKT_H
#define SONSTIGESPROJEKT_H

#include <QString>
#include <QVector>
#include <vector>
#include <QtCore/QStringList>

#include "studiengang.h"
#include "nutzer.h"

using namespace std;

class SonstigesProjekt
{
public:
    SonstigesProjekt() {}

    SonstigesProjekt(QString titel, QStringList stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        _titel(titel), _stichwortliste(stichwortliste), _abgeschlossen(abgeschlossen), _erlaeuterung(erlaeuterung) {}

    SonstigesProjekt(int id, QString titel, QStringList stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        _id(id), _titel(titel), _stichwortliste(stichwortliste), _abgeschlossen(abgeschlossen), _erlaeuterung(erlaeuterung) {}

    SonstigesProjekt(int id, QString titel, bool abgeschlossen, QString erlaeuterung) :
        _id(id), _titel(titel),  _abgeschlossen(abgeschlossen), _erlaeuterung(erlaeuterung) {}

    SonstigesProjekt(int id, QString titel, QStringList stichwortliste, bool abgeschlossen) :
        _id(id), _titel(titel), _stichwortliste(stichwortliste), _abgeschlossen(abgeschlossen) {}

    SonstigesProjekt(QString titel, QStringList stichwortliste, bool abgeschlossen) :
        _titel(titel), _stichwortliste(stichwortliste), _abgeschlossen(abgeschlossen) {}

    static vector<SonstigesProjekt> query_all();
    static vector<SonstigesProjekt> query(QString condition);

    int id() const;
    void setId(int id);

    QString titel() const;
    void setTitel(const QString &titel);

    QStringList stichwortliste() const;
    void setStichwortliste(const QStringList &stichwortliste);

    bool abgeschlossen() const;
    void setAbgeschlossen(bool abgeschlossen);

    QString erlaeuterung() const;
    void setErlaeuterung(const QString &erlaeuterung);

    Studiengang studiengang() const;
    void setStudiengang(const Studiengang &studiengang);

    Nutzer bearbeiter() const;
    void setBearbeiter(const Nutzer &bearbeiter);

    Nutzer professor() const;
    void setProfessor(const Nutzer &professor);

private:
    int _id = -1;
    QString _titel;
    QStringList _stichwortliste;
    bool _abgeschlossen;
    QString _erlaeuterung;

    Studiengang _studiengang;
    Nutzer _bearbeiter;
    Nutzer _professor;
};

#endif // SONSTIGESPROJEKT_H

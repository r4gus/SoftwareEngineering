#ifndef ARBEIT_H
#define ARBEIT_H

#include "studiengang.h"
#include "nutzer.h"

#include <QString>
#include <QVector>
#include <memory>

class Arbeit
{
    int _id = -1;
    QString _titel;
    QVector<QString> _stichwortliste;
    bool _abgeschlossen;
    QString _erlaeuterung;

    Studiengang _studiengang;
    Nutzer _bearbeiter;
    Nutzer _professor;

public:
    Arbeit() {}

    Arbeit(QString titel, QVector<QString> stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        _titel(titel), _stichwortliste(stichwortliste), _abgeschlossen(abgeschlossen), _erlaeuterung(erlaeuterung) {}

    Arbeit(int id, QString titel, QVector<QString> stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        _id(id), _titel(titel), _stichwortliste(stichwortliste), _abgeschlossen(abgeschlossen), _erlaeuterung(erlaeuterung) {}

    Arbeit(int id, QString titel, QVector<QString> stichwortliste, bool abgeschlossen) :
        _id(id), _titel(titel), _stichwortliste(stichwortliste), _abgeschlossen(abgeschlossen) {}

    Arbeit(QString titel, QVector<QString> stichwortliste, bool abgeschlossen) :
        _titel(titel), _stichwortliste(stichwortliste), _abgeschlossen(abgeschlossen) {}


    int id() const;
    void setId(int id);
    QString titel() const;
    void setTitel(const QString &titel);
    QVector<QString> stichwortliste() const;
    void setStichwortliste(const QVector<QString> &stichwortliste);
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
};

#endif // ARBEIT_H

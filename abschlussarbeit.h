#ifndef ABSCHLUSSARBEIT_H
#define ABSCHLUSSARBEIT_H

#include "sonstigesprojekt.h"
#include <QDate>

class Abschlussarbeit : public SonstigesProjekt
{
public:
    Abschlussarbeit() {}

    Abschlussarbeit(QString titel, QStringList stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        SonstigesProjekt(titel, stichwortliste, abgeschlossen, erlaeuterung) {}

    Abschlussarbeit(int id, QString titel, QStringList stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        SonstigesProjekt(id, titel, stichwortliste, abgeschlossen, erlaeuterung) {}

    Abschlussarbeit(int id, QString titel, bool abgeschlossen, QString erlaeuterung) :
        SonstigesProjekt(id, titel, abgeschlossen, erlaeuterung) {}

    Abschlussarbeit(int id, QString titel, QStringList stichwortliste, bool abgeschlossen) :
        SonstigesProjekt(id, titel, stichwortliste, abgeschlossen) {}

    Abschlussarbeit(QString titel, QStringList stichwortliste, bool abgeschlossen) :
        SonstigesProjekt(titel, stichwortliste, abgeschlossen) {}

    QDate begin() const;
    void setBegin(const QDate &begin);

    QDate end() const;
    void setEnd(const QDate &end);

    QString firma() const;
    void setFirma(const QString &firma);

    Abschlussarbeit(const SonstigesProjekt &rhs) {
        this->setId(rhs.id());
        this->setTitel(rhs.titel());
        this->setProfessor(rhs.professor());
        this->setBearbeiter(rhs.bearbeiter());
        this->setStudiengang(rhs.studiengang());
        this->setErlaeuterung(rhs.erlaeuterung());
        this->setAbgeschlossen(rhs.abgeschlossen());
        this->setStichwortliste(rhs.stichwortliste());
    }



    static vector<Abschlussarbeit> query_all();
    static vector<Abschlussarbeit> query(QString condition);

private:
    QDate _begin;
    QDate _end;
    QString _firma = "";
};

#endif // ABSCHLUSSARBEIT_H

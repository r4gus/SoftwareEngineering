#ifndef SONSTIGESPROJEKT_H
#define SONSTIGESPROJEKT_H

#include "arbeit.h"

class SonstigesProjekt : public Arbeit
{
public:
    SonstigesProjekt() {}

    SonstigesProjekt(QString titel, QVector<QString> stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        Arbeit(titel, stichwortliste, abgeschlossen, erlaeuterung) {}

    SonstigesProjekt(int id, QString titel, QVector<QString> stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        Arbeit(id, titel, stichwortliste, abgeschlossen, erlaeuterung) {}

    SonstigesProjekt(int id, QString titel, QVector<QString> stichwortliste, bool abgeschlossen) :
        Arbeit(id, titel, stichwortliste, abgeschlossen) {}

    SonstigesProjekt(QString titel, QVector<QString> stichwortliste, bool abgeschlossen) :
        Arbeit(titel, stichwortliste, abgeschlossen) {}

    static vector<SonstigesProjekt> query_all();
    static vector<SonstigesProjekt> query(QString condition);
};

#endif // SONSTIGESPROJEKT_H

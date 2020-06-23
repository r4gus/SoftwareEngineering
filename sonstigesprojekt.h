#ifndef SONSTIGESPROJEKT_H
#define SONSTIGESPROJEKT_H

#include "arbeit.h"

#include <QString>
#include <QVector>

class SonstigesProjekt : public Arbeit
{
public:
    SonstigesProjekt(QString titel, QVector<QString> stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        Arbeit(titel, stichwortliste, abgeschlossen, erlaeuterung) {}

    SonstigesProjekt(int id, QString titel, QVector<QString> stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        Arbeit(id, titel, stichwortliste, abgeschlossen, erlaeuterung) {}

    SonstigesProjekt(QString titel, QVector<QString> stichwortliste, bool abgeschlossen) :
        Arbeit(titel, stichwortliste, abgeschlossen) {}

    SonstigesProjekt(int id, QString titel, QVector<QString> stichwortliste, bool abgeschlossen) :
        Arbeit(id, titel, stichwortliste, abgeschlossen) {}
};

#endif // SONSTIGESPROJEKT_H

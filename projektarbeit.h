#ifndef PROJEKTARBEIT_H
#define PROJEKTARBEIT_H

#include "sonstigesprojekt.h"


class Projektarbeit : public SonstigesProjekt
{
public:
    Projektarbeit() {}

    Projektarbeit(QString titel, QStringList stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        SonstigesProjekt(titel, stichwortliste, abgeschlossen, erlaeuterung) {}

    Projektarbeit(int id, QString titel, QStringList stichwortliste, bool abgeschlossen, QString erlaeuterung) :
        SonstigesProjekt(id, titel, stichwortliste, abgeschlossen, erlaeuterung) {}

    Projektarbeit(int id, QString titel, bool abgeschlossen, QString erlaeuterung) :
        SonstigesProjekt(id, titel, abgeschlossen, erlaeuterung) {}

    Projektarbeit(int id, QString titel, QStringList stichwortliste, bool abgeschlossen) :
        SonstigesProjekt(id, titel, stichwortliste, abgeschlossen) {}

    Projektarbeit(QString titel, QStringList stichwortliste, bool abgeschlossen) :
        SonstigesProjekt(titel, stichwortliste, abgeschlossen) {}

    unsigned semester() const;
    void setSemester(const unsigned &semester);

    Projektarbeit(const SonstigesProjekt &rhs) {
        this->setId(rhs.id());
        this->setTitel(rhs.titel());
        this->setProfessor(rhs.professor());
        this->setBearbeiter(rhs.bearbeiter());
        this->setStudiengang(rhs.studiengang());
        this->setErlaeuterung(rhs.erlaeuterung());
        this->setAbgeschlossen(rhs.abgeschlossen());
        this->setStichwortliste(rhs.stichwortliste());
    }

    static vector<Projektarbeit> query_all();
    static vector<Projektarbeit> query(QString condition);
private:
    unsigned _semester;
};


#endif // PROJEKTARBEIT_H

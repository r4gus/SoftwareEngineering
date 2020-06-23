#include "arbeit.h"

QString Arbeit::titel() const
{
    return _titel;
}

void Arbeit::setTitel(const QString &titel)
{
    _titel = titel;
}

QVector<QString> Arbeit::stichwortliste() const
{
    return _stichwortliste;
}

void Arbeit::setStichwortliste(const QVector<QString> &stichwortliste)
{
    _stichwortliste = stichwortliste;
}

bool Arbeit::abgeschlossen() const
{
    return _abgeschlossen;
}

void Arbeit::setAbgeschlossen(bool abgeschlossen)
{
    _abgeschlossen = abgeschlossen;
}

QString Arbeit::erlaeuterung() const
{
    return _erlaeuterung;
}

void Arbeit::setErlaeuterung(const QString &erlaeuterung)
{
    _erlaeuterung = erlaeuterung;
}

std::shared_ptr<Studiengang> Arbeit::studiengang() const
{
    return _studiengang;
}

void Arbeit::setStudiengang(const std::shared_ptr<Studiengang> &studiengang)
{
    _studiengang = studiengang;
}

int Arbeit::id() const
{
    return _id;
}

void Arbeit::setId(int id)
{
    _id = id;
}

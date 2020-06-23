#include "mainwindow.h"
#include "studiengang.h"
#include "sonstigesprojekt.h"
#include "DB.h"

#include <QApplication>
#include <QScrollArea>
#include <QVector>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QVector<QString> v;
    v.push_back("Ein Stichwort");
    Studiengang s = Studiengang("IN-SE", "Bachelor");
    SonstigesProjekt sons = SonstigesProjekt("Einen Titel", v, false, "ja das ist halt so");
    sons.setStudiengang(std::shared_ptr<Studiengang>(&s));
    try {
        DB::session().add(s);
    } catch(DatabaseTransactionError &e) {
        std::cout << "Couldn't add s to database" << std::endl;
    }

    try {
        DB::session().add(sons);
    } catch(DatabaseTransactionError &e) {
        std::cout << "Couldn't add sons to database" << std::endl;
    }

    vector<Studiengang> vec = Studiengang::query_all();
    for(auto &e: vec) {
        std::cout << e << endl;
    }

    w.show();
    return a.exec();
}

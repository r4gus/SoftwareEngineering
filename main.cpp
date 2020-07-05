#include "mainwindow.h"
#include "studiengang.h"
#include "sonstigesprojekt.h"
#include "DB.h"

#include <QApplication>
#include <QScrollArea>
#include <QVector>
#include <iostream>
#include <QTranslator>


int main(int argc, char *argv[])
{
    // Translator
    QTranslator translator;
    translator.load("strings_en");

    QApplication a(argc, argv);
    a.installTranslator(&translator);
    MainWindow w;

    QVector<QString> v;
    v.push_back("Ein Stichwort");
    Studiengang s = Studiengang("IN-IS", "Bachelor");
    try {
        DB::session().add(s);
    } catch(DatabaseTransactionError &e) {
        std::cout << "Couldn't add s to database" << std::endl;
    }

    vector<Studiengang> vec = Studiengang::query_all();
    for(auto &e: vec) {
        std::cout << e << endl;
    }

    w.show();
    return a.exec();

}

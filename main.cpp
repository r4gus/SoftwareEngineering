#include "mainwindow.h"
#include "studiengang.h"
#include "DB.h"

#include <QApplication>
#include <QScrollArea>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Studiengang s = Studiengang("IN-SE", "Bachelor");
    DB::session().add(s);

    vector<Studiengang> vec = Studiengang::query_all();
    for(auto &e: vec) {
        std::cout << e << endl;
    }

    w.show();
    return a.exec();
}

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

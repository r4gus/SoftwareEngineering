#include "mainwindow.h"
#include "studiengang.h"
#include "sonstigesprojekt.h"
#include "DB.h"
#include "searchview.h"

#include <QVector>
#include <iostream>
#include <QTranslator>
#include <QtWidgets/QApplication>
#include <QtCore/QFile>


int main(int argc, char *argv[])
{
    // Translator
    QTranslator translator;
    translator.load("strings_en");

    QApplication a(argc, argv);
    a.installTranslator(&translator);

    QStringList v;
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
    MainWindow::get().setMinimumSize(1000, 600);
    // Style
    QFile styleFile(":/css/general.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QTextStream textStream(&styleFile);
        QString stylesheet = textStream.readAll();
        styleFile.close();
        a.setStyleSheet(stylesheet);
    }

    MainWindow::get().show();
    MainWindow::get().showView(new SearchView);

    return a.exec();

}

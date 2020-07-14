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
    DB::session();
    QSqlDatabase db = QSqlDatabase::database();

    if(argc > 1) {
        if(strcmp(argv[1], "clean") == 0) {
            DB::clean(db);      // get rid of everything
            DB::initialize(db); // initialize db
        } else if(strcmp(argv[1], "test") == 0) {
            DB::clean(db);
            DB::initialize(db);
            DB::test(db);   // fill db with test data
        }
    }
    DB::initialize(db); // initialize db

    addStudiengang();


    // Translator
    QTranslator translator;
    translator.load("strings_en");

    QApplication a(argc, argv);
    a.installTranslator(&translator);

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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchview.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(new SearchView);
}

MainWindow::~MainWindow()
{
    delete ui;
}



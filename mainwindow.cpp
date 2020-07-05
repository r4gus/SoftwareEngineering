#include "mainwindow.h"
#include "searchview.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(new SearchView());
}

MainWindow::~MainWindow()
{
    delete ui;
}


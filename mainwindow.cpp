#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchview.h"
#include "gui_utils.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , user(Nutzer::guest())
{
    user.setRole(Nutzer::administrator);
    setProperty("style", "background");
    ui->setupUi(this);
    root = new QVBoxLayout;
    ui->centralwidget->setLayout(root);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showView(QLayout * view) {
    clearLayout(root);
    root->addLayout(view);
}



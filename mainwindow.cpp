#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchview.h"
#include "gui_utils.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , user(Nutzer::guest())
{
    // TODO: Remove only debug
    user = Nutzer("2", "2", "2", Nutzer::administrator);
    user.setId(18);

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



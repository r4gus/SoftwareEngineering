#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include "nutzer.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    static MainWindow& get() {
        static MainWindow instance;
        return instance;
    }

    void showView(QLayout*);
    Nutzer user;

private:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;
    QVBoxLayout* root;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
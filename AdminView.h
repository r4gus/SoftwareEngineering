//
// Created by Julian on 10.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_ADMINVIEW_H
#define SOFTWAREENGINEERINGPROJECT_ADMINVIEW_H


#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

class AdminView : public QVBoxLayout {
    Q_OBJECT

public:
    AdminView();

private:
    QPushButton *btnAdd;
    QLineEdit *tfSearch;
    QPushButton *btnSearch;
    QVBoxLayout *cLecturersList;
    QPushButton *btnToSearch;

private Q_SLOTS:

    void openAddNewLecturer();

    void search();

    void toSearchView();

    void addNewLecturer(int);
};


#endif //SOFTWAREENGINEERINGPROJECT_ADMINVIEW_H

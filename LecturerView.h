//
// Created by Julian on 10.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_LECTURERVIEW_H
#define SOFTWAREENGINEERINGPROJECT_LECTURERVIEW_H


#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include "nutzer.h"

class LecturerView : public QVBoxLayout {
Q_OBJECT

public:
    LecturerView(const Nutzer &, QVBoxLayout *);

private:
    QPushButton *btnRemove;
    QPushButton *btnEdit;
    QVBoxLayout *parent;
    int lecturerID;

private Q_SLOTS:

    void remove();

    void openEditWindow();

    void edited(int);
};


#endif //SOFTWAREENGINEERINGPROJECT_LECTURERVIEW_H

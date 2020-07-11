//
// Created by Julian on 10.07.2020.
//

#include <QtWidgets/QLabel>
#include "LecturerView.h"

LecturerView::LecturerView(const Nutzer &lecturer, QVBoxLayout *parent)
    : parent(parent) {
    auto root = new QHBoxLayout;
    addLayout(root);
    {
        auto cData = new QVBoxLayout;
        root->addLayout(cData);
        {
            auto lblEmail = new QLabel(lecturer.email());
            cData->addWidget(lblEmail);
            auto cName = new QHBoxLayout;
            cData->addLayout(cName);
            {
                auto lblFirstName = new QLabel(lecturer.vname());
                cName->addWidget(lblFirstName);
                auto lblLastName = new QLabel(lecturer.nname());
                cName->addWidget(lblLastName);
            }
        }

        auto cControl = new QVBoxLayout;
        root->addLayout(cControl);
        {
            btnEdit = new QPushButton(tr("Bearbeiten"));
            cControl->addWidget(btnEdit);
            btnRemove = new QPushButton(tr("Löschen"));
            cControl->addWidget(btnRemove);
        }
    }
}

void LecturerView::remove() {

}

void LecturerView::openEditWindow() {

}

void LecturerView::edited(int) {

}

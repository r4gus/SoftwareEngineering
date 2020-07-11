//
// Created by Julian on 10.07.2020.
//

#include <QtWidgets/QLabel>
#include <QDebug>
#include "LecturerView.h"
#include "gui_utils.h"
#include "lecturereditview.h"
#include "DB.h"

LecturerView::LecturerView(const Nutzer &lecturer, QVBoxLayout *parent)
    : parent(parent), lecturerID(lecturer.id()) {
    cRoot = new QHBoxLayout;
    setLayout(cRoot);
    build(lecturer);
}

void LecturerView::build(const Nutzer &lecturer) {
    {
        auto cData = new QVBoxLayout;
        cRoot->addLayout(cData);
        {
            auto cName = new QHBoxLayout;
            cData->addLayout(cName);
            {
                auto lblFirstName = new QLabel(lecturer.vname());
                cName->addWidget(lblFirstName);
                auto lblLastName = new QLabel(lecturer.nname());
                cName->addWidget(lblLastName);
            }
            auto lblEmail = new QLabel(lecturer.email());
            cData->addWidget(lblEmail);
        }

        auto cControl = new QVBoxLayout;
        cRoot->addLayout(cControl);
        {
            btnEdit = new QPushButton(tr("Bearbeiten"));
            cControl->addWidget(btnEdit);
            btnRemove = new QPushButton(tr("Löschen"));
            cControl->addWidget(btnRemove);
        }
    }

    // SIGNALS + SLOTS
    connect(btnEdit, &QPushButton::clicked, [this]{ openEditWindow(); });
    connect(btnRemove, &QPushButton::clicked, [this]{ remove(); });

    // STYLE
    setFrameShape(QFrame::Shape::StyledPanel);
    setLineWidth(3);

    setMaximumHeight(100);
    setMinimumHeight(100);
}


void LecturerView::update(const Nutzer &lecturer) {
    clearLayout(cRoot);
    build(lecturer);
}


void LecturerView::remove() {
    auto tmpUser = Nutzer();
    tmpUser.setId(lecturerID);
    DB::session().remove(tmpUser);
    parent->removeWidget(this);
    deleteLater();
}



void LecturerView::openEditWindow() {
    auto editView = new LecturerEditView(lecturerID);
    auto popup = openPopup(editView);
    connect(editView, &LecturerEditView::requestClose, [popup]{ popup->close(); });
    connect(editView, &LecturerEditView::saved, this, &LecturerView::edited);
}

void LecturerView::edited(int) {
    auto users = Nutzer::query("nutzerID=" + str(lecturerID));
    if (users.size() == 1) {
        auto user = users[0];
        update(user);
    } else {
        qDebug() << "Error: Can't find user with ID in DB: " + str(lecturerID);
    }
}

//
// Created by Julian on 10.07.2020.
//
#include <QDebug>
#include <QtWidgets/QScrollArea>

#include "AdminView.h"
#include "mainwindow.h"
#include "searchview.h"
#include "gui_utils.h"
#include "lecturereditview.h"
#include "LecturerView.h"

AdminView::AdminView() {
    auto cRoot = new QVBoxLayout;
    addLayout(cRoot);
    {
        auto cControl = new QHBoxLayout;
        cRoot->addLayout(cControl);
        {
            btnAdd = new QPushButton(tr("Neuen Dozent hinzufügen"));
            cControl->addWidget(btnAdd);
        }
        btnToSearch = new QPushButton(tr("Zur Suche"));
        cRoot->addWidget(btnToSearch);
        {
            cLecturersList = buildScrollContainer(cRoot);
        }
    }

    // SIGNALS and SLOTS
    connect(btnToSearch, &QPushButton::clicked, [this] { toSearchView(); });
    connect(btnAdd, &QPushButton::clicked, [this] { openAddNewLecturer(); });

    // load all
    auto users = Nutzer::query("role=" + str(Nutzer::Role::dozent));
    for (const auto &user : users) {
        auto lecturerView = new LecturerView(user, cLecturersList);
        cLecturersList->addWidget(lecturerView);
    }
}

void AdminView::openAddNewLecturer() {
    auto parent = new QVBoxLayout;
    auto content = new LecturerEditView;
    parent->addWidget(content);
    auto popup = openPopup(parent);
    connect(content, &LecturerEditView::requestClose, [popup]{ popup->close(); });
    connect(content, &LecturerEditView::saved, this, &AdminView::addNewLecturer);
}

void AdminView::toSearchView() {
    MainWindow::get().showView(new SearchView);
}

void AdminView::addNewLecturer(int id) {
    auto user = queryOne<Nutzer>(Nutzer::query, "nutzerID=" + str(id));
    auto lecturerView = new LecturerView(user, cLecturersList);
    cLecturersList->addWidget(lecturerView);
}

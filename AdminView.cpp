//
// Created by Julian on 10.07.2020.
//
#include <QDebug>

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
            btnAdd = new QPushButton(tr("Hinzufügen"));
            cControl->addWidget(btnAdd);
            tfSearch = new QLineEdit;
            cControl->addWidget(tfSearch);
            btnSearch = new QPushButton(tr("Suchen"));
            cControl->addWidget(btnSearch);
        }
        btnToSearch = new QPushButton(tr("Zur Suche"));
        cRoot->addWidget(btnToSearch);
        cLecturersList = new QVBoxLayout;
        cRoot->addLayout(cLecturersList);
    }

    // SIGNALS and SLOTS
    connect(btnToSearch, &QPushButton::clicked, [this] { toSearchView(); });
    connect(btnSearch, &QPushButton::clicked, [this] { search(); });
    connect(btnAdd, &QPushButton::clicked, [this] { openAddNewLecturer(); });

    search();
}

void AdminView::openAddNewLecturer() {
    auto parent = new QVBoxLayout;
    auto content = new LecturerEditView;
    parent->addWidget(content);
    auto popup = openPopup(parent);
    connect(content, &LecturerEditView::requestClose, [popup]{ popup->close(); });
}

void AdminView::search() {
    auto users = Nutzer::query("role=" + str(Nutzer::Role::dozent));
    for (const auto user : users) {
        auto lecturerView = new LecturerView(user, cLecturersList);
        cLecturersList->addLayout(lecturerView);
    }
}

void AdminView::toSearchView() {
    MainWindow::get().showView(new SearchView);
}

void AdminView::addNewLecturer(int) {

}

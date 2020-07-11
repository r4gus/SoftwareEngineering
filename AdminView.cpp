//
// Created by Julian on 10.07.2020.
//
#include <QDebug>

#include "AdminView.h"
#include "mainwindow.h"
#include "searchview.h"
#include "gui_utils.h"
#include "lecturereditview.h"

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
        {
            // TODO: get lecturers and show
        }
    }

    // SIGNALS and SLOTS
    connect(btnToSearch, &QPushButton::clicked, [this] { toSearchView(); });
    connect(btnSearch, &QPushButton::clicked, [this] { search(); });
    connect(btnAdd, &QPushButton::clicked, [this] { openAddNewLecturer(); });
}

void AdminView::openAddNewLecturer() {
    auto parent = new QVBoxLayout;
    auto content = new LecturerEditView;
    parent->addWidget(content);
    openPopup(parent);
}

void AdminView::search() {
}

void AdminView::toSearchView() {
    MainWindow::get().showView(new SearchView);
}

void AdminView::addNewLecturer(int) {

}

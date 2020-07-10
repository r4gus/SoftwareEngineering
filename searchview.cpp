#include "searchview.h"
#include "ProjectView.h"
#include "ProjectEditView.h"
#include "gui_utils.h"
#include "LoginView.h"
#include "mainwindow.h"
#include "AdminView.h"

#include <QTranslator>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>

SearchView::SearchView()
{
    auto containerRoot = new QHBoxLayout;
    addLayout(containerRoot);
    {
        auto containerLeftSide = new QVBoxLayout();
        containerRoot->addLayout(containerLeftSide);
        {
            auto lblSearchCriteria = new QLabel(tr("Suchkriterien"));
            containerLeftSide->addWidget(lblSearchCriteria);
            auto containerSearch = new QFormLayout();
            containerLeftSide->addLayout(containerSearch);
            {
                tfSearchTitle = new QLineEdit;
                containerSearch->addRow(tr("Titel:"), tfSearchTitle);
                tfSearchAuthor = new QLineEdit;
                containerSearch->addRow(tr("Bearbeiter:"), tfSearchAuthor);
                tfSearchLecturer = new QLineEdit;
                containerSearch->addRow(tr("Betreuer:"), tfSearchLecturer);
                tfSearchTags = new QLineEdit;
                containerSearch->addRow(tr("Tags:"), tfSearchTags);
                containerSearch->addRow(tr("Zeitraum"), new QLabel);
                calendarSearchTimeFrom = new QCalendarWidget;
                containerSearch->addRow(tr("Von:"), calendarSearchTimeFrom);
                calendarSearchTimeTo = new QCalendarWidget;
                containerSearch->addRow(tr("Bis:"), calendarSearchTimeTo);
                dropdownSearchType = new QComboBox;
                containerSearch->addRow(tr("Typ:"), dropdownSearchType);
            }
            btnSearch = new QPushButton(tr("Suche"));
            containerLeftSide->addWidget(btnSearch);
        }

        auto containerRightSide = new QVBoxLayout();
        containerRoot->addLayout(containerRightSide);
        {
            auto user = MainWindow::get().user;
            if (user.role() & Nutzer::Role::create_arbeit) {
                btnAddProject = new QPushButton(tr("Arbeit hinzufügen"));
                containerRightSide->addWidget(btnAddProject);
                connect(btnAddProject, SIGNAL(clicked()), this, SLOT(openAddProject()));
            }
            btnLoginLogout = new QPushButton(user.is_student() ? tr("Anmelden") : tr("Abmelden"));
            containerRightSide->addWidget(btnLoginLogout);
            if (user.is_administrator()) {
                btnShowAdminView = new QPushButton(tr("Admin Fenster"));
                containerRightSide->addWidget(btnShowAdminView);
                connect(btnShowAdminView, SIGNAL(clicked()), this, SLOT(openAdminView()));
            }
            containerProjectsList = new QVBoxLayout;
            containerRightSide->addLayout(containerProjectsList);
        }
    }
    // SIGNALS
    connect(btnSearch, SIGNAL(clicked()), this, SLOT(search()));
    connect(btnLoginLogout, SIGNAL(clicked()), this, SLOT(loginLogout()));

}

void SearchView::search() {
    auto projects = SonstigesProjekt::query_all();
    auto user = MainWindow::get().user;
    for (const auto& project : projects) {
        bool editable = project.professor().id() == user.id() || user.is_administrator();
        auto vProject = new ProjectView(project, containerProjectsList, editable);
        containerProjectsList->addLayout(vProject);
    }
}

void SearchView::openAddProject() {
    openPopup(new ProjectEditView);
}

void SearchView::loginLogout() {
    if (MainWindow::get().user.is_student()) {
        // login
        openPopup(new LoginView);
    } else {
        // logout
        MainWindow::get().user = Nutzer::guest();
        MainWindow::get().showView(new SearchView);
    }
}

void SearchView::openAdminView() {
    MainWindow::get().showView(new AdminView);
}

void SearchView::addNewProject(int) {

}

void SearchView::toggleSort() {

}

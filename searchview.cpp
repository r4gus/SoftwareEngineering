#include "searchview.h"
#include "ProjectView.h"
#include "ProjectEditView.h"
#include "gui_utils.h"
#include "LoginView.h"

#include <QFormLayout>
#include <QTranslator>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>

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
            btnAddProject = new QPushButton(tr("Arbeit hinzufügen"));
            containerRightSide->addWidget(btnAddProject);
            btnLoginLogout = new QPushButton(tr("Anmelden")); // TODO: get state
            containerRightSide->addWidget(btnLoginLogout);
            if (true) { // TODO: only when admin
                btnShowAdminView = new QPushButton(tr("Admin Fenster"));
                containerRightSide->addWidget(btnShowAdminView);
            }
            containerProjectsList = new QVBoxLayout;
            containerRightSide->addLayout(containerProjectsList);
        }
    }
    // SIGNALS
    connect(btnSearch, SIGNAL(clicked()), this, SLOT(search()));
    connect(btnAddProject, SIGNAL(clicked()), this, SLOT(openAddProject()));
    connect(btnLoginLogout, SIGNAL(clicked()), this, SLOT(loginLogout()));
    connect(btnShowAdminView, SIGNAL(clicked()), this, SLOT(openAdminView()));

}

void SearchView::search() {
    auto projects = SonstigesProjekt::query_all();
    for (const auto& project : projects) {
        auto vProject = new ProjectView(project, containerProjectsList, true);
        containerProjectsList->addLayout(vProject);
    }
}

void SearchView::openAddProject() {
    openPopup(new ProjectEditView);
}

void SearchView::loginLogout() {
    // TODO: Check if logged in
    if (true) {
        openPopup(new LoginView);
    }
}

void SearchView::openAdminView() {

}

void SearchView::addNewProject(int) {

}

void SearchView::toggleSort() {

}

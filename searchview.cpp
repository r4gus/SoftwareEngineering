#include "searchview.h"
#include "ProjectView.h"
#include "ProjectEditView.h"
#include "gui_utils.h"
#include "LoginView.h"
#include "mainwindow.h"
#include "AdminView.h"
#include "lecturereditview.h"

#include <QTranslator>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
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
            containerProjectsList = buildScrollContainer(containerRightSide);
        }
    }
    // SIGNALS
    connect(btnSearch, SIGNAL(clicked()), this, SLOT(search()));
    connect(btnLoginLogout, SIGNAL(clicked()), this, SLOT(loginLogout()));

    // load default projects
    //    QtConcurrent::run([this]{ search(); }); use this when DB can run on this thread
    search();
}

bool hasPermission(const Nutzer &user) {
    auto currentUser = MainWindow::get().user;
    return (currentUser.id() == user.id() && user.id() != -1) || currentUser.is_administrator();
}


void SearchView::search() {
    clearLayout(containerProjectsList);

    auto projects = SonstigesProjekt::query_all();
    for (const auto& project : projects) {
        auto vProject = new ProjectView(project, containerProjectsList, hasPermission(project.professor()));
        containerProjectsList->addWidget(vProject);
    }
    auto projectsThesis = Abschlussarbeit::query_all();
    for (const auto& project : projectsThesis) {
        auto vProject = new ProjectView(project, containerProjectsList, hasPermission(project.professor()));
        containerProjectsList->addWidget(vProject);
    }
    auto projectsProject = Projektarbeit::query_all();
    for (const auto& project : projectsProject) {
        auto vProject = new ProjectView(project, containerProjectsList, hasPermission(project.professor()));
        containerProjectsList->addWidget(vProject);
    }
}

void SearchView::openAddProject() {
    auto projectView = new ProjectEditView;
    auto popup = openPopup(projectView);
    connect(projectView, &ProjectEditView::requestClose, [popup]{ popup->close(); });
    connect(projectView, &ProjectEditView::saved, this, &SearchView::addNewProject);
}

void SearchView::loginLogout() {
    if (MainWindow::get().user.is_student()) {
        // login
        auto loginView = new LoginView;
        auto popup = openPopup(loginView);
        connect(loginView, &LoginView::requestClose, [popup]{ popup->close(); });
    } else {
        // logout
        MainWindow::get().user = Nutzer::guest();
        MainWindow::get().showView(new SearchView);
    }
}

void SearchView::openAdminView() {
    MainWindow::get().showView(new AdminView);
}

void SearchView::addNewProject(int id, ProjectType projectType) {
    clearLayout(containerProjectsList);
    auto query = "arbeitID='" + str(id) + "'";
    if (projectType == PROJECT) {
        auto project = queryOne<Projektarbeit>(Projektarbeit::query, query);
        containerProjectsList->addWidget(new ProjectView(project, containerProjectsList, hasPermission(project.professor())));
    }
    if (projectType == OTHER) {
        auto project = queryOne<SonstigesProjekt>(SonstigesProjekt::query, query);
        containerProjectsList->addWidget(new ProjectView(project, containerProjectsList, hasPermission(project.professor())));
    }
    if (projectType == THESIS) {
        auto project = queryOne<Abschlussarbeit>(Abschlussarbeit::query, query);
        containerProjectsList->addWidget(new ProjectView(project, containerProjectsList, hasPermission(project.professor())));
    }
}

void SearchView::toggleSort() {

}

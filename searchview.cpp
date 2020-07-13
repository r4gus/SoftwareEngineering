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

SearchView::SearchView() {
    auto containerRoot = new QHBoxLayout;
    addLayout(containerRoot);
    {
        auto containerLeftSide = new QVBoxLayout();
        containerRoot->addLayout(containerLeftSide);
        {
            auto lblSearchCriteria = new QLabel(tr("Suchkriterien"));
            lblSearchCriteria->setProperty("style", "h1");
            containerLeftSide->addWidget(lblSearchCriteria);
            auto containerSearch = new QFormLayout();
            containerLeftSide->addLayout(containerSearch);
            {
                tfSearchKeyword = new QLineEdit;
                containerSearch->addRow(tr("Stichwort:"), tfSearchKeyword);
                tfSearchAuthor = new QLineEdit;
                containerSearch->addRow(tr("Bearbeiter:"), tfSearchAuthor);
                tfSearchLecturer = new QLineEdit;
                containerSearch->addRow(tr("Betreuer:"), tfSearchLecturer);
                containerSearch->addRow(tr("Zeitraum"), new QLabel);
                calendarSearchTimeFrom = new QCalendarWidget;
                calendarSearchTimeFrom->setSelectedDate(QDate{2000, 1, 1});
                containerSearch->addRow(tr("Von:"), calendarSearchTimeFrom);
                calendarSearchTimeTo = new QCalendarWidget;
                containerSearch->addRow(tr("Bis:"), calendarSearchTimeTo);
                dropdownSearchType = new QComboBox;
                // TODO: "Alle" to constant field
                dropdownSearchType->insertItems(0, {
                        tr("Alle"),
                        projectType2Name.find(OTHER)->second,
                        projectType2Name.find(THESIS)->second,
                        projectType2Name.find(PROJECT)->second,
                });
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
    auto allTypes = dropdownSearchType->currentText() == "Alle";  // TODO: replace with field
    ProjectType projectType;
    if ((name2ProjectType.find(dropdownSearchType->currentText())) != name2ProjectType.end()) {
        projectType = name2ProjectType.find(dropdownSearchType->currentText())->second;
    }
    Condition conditionBuilder("");
    conditionBuilder = (Condition{"titel", tfSearchKeyword->text()} ||
                        Condition{"stichwort", tfSearchKeyword->text()})
                       && (Condition{"student.VName", tfSearchAuthor->text()} ||
                           Condition{"student.NName", tfSearchAuthor->text()})
                       && (Condition{"prof.VName", tfSearchLecturer->text()} ||
                           Condition{"prof.NName", tfSearchLecturer->text()});
    // TODO: add begin and end
    auto query = conditionBuilder.condition;
    if (allTypes || projectType == OTHER) {
        auto projects = SonstigesProjekt::query(query);
        for (const auto &project : projects) {
            auto vProject = new ProjectView(project, containerProjectsList, hasPermission(project.professor()));
            containerProjectsList->addWidget(vProject);
        }
    }
    if (allTypes || projectType == THESIS) {
        conditionBuilder = conditionBuilder
                           && Condition{"beginn >='" + calendarSearchTimeFrom->selectedDate().toString("yyyy-MM-dd") + "'"}
                           && Condition{"ende <='" + calendarSearchTimeTo->selectedDate().toString("yyyy-MM-dd") + "'"}
                ;
        auto projectsThesis = Abschlussarbeit::query(conditionBuilder.condition);
        for (const auto &project : projectsThesis) {
            auto vProject = new ProjectView(project, containerProjectsList, hasPermission(project.professor()));
            containerProjectsList->addWidget(vProject);
        }
    }
    if (allTypes || projectType == PROJECT) {
        auto projectsProject = Projektarbeit::query(query);
        for (const auto &project : projectsProject) {
            auto vProject = new ProjectView(project, containerProjectsList, hasPermission(project.professor()));
            containerProjectsList->addWidget(vProject);
        }
    }
}

void SearchView::openAddProject() {
    auto projectView = new ProjectEditView;
    auto popup = openPopup(projectView);
    connect(projectView, &ProjectEditView::requestClose, [popup] { popup->close(); });
    connect(projectView, &ProjectEditView::saved, this, &SearchView::addNewProject);
}

void SearchView::loginLogout() {
    if (MainWindow::get().user.is_student()) {
        // login
        auto loginView = new LoginView;
        auto popup = openPopup(loginView);
        connect(loginView, &LoginView::requestClose, [popup] { popup->close(); });
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
    auto query = "arbeit.arbeitID='" + str(id) + "'";
    if (projectType == PROJECT) {
        auto project = queryOne<Projektarbeit>(Projektarbeit::query, query);
        containerProjectsList->addWidget(
                new ProjectView(project, containerProjectsList, hasPermission(project.professor())));
    }
    if (projectType == OTHER) {
        auto project = queryOne<SonstigesProjekt>(SonstigesProjekt::query, query);
        containerProjectsList->addWidget(
                new ProjectView(project, containerProjectsList, hasPermission(project.professor())));
    }
    if (projectType == THESIS) {
        auto project = queryOne<Abschlussarbeit>(Abschlussarbeit::query, query);
        containerProjectsList->addWidget(
                new ProjectView(project, containerProjectsList, hasPermission(project.professor())));
    }
}

void SearchView::toggleSort() {

}

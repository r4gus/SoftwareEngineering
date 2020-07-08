#include "searchview.h"
#include "ProjectView.h"

#include <QFormLayout>
#include <QTranslator>
#include <QtWidgets/QLabel>

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

        auto containerRightSide = new QHBoxLayout();
        containerRoot->addLayout(containerRightSide);
        {
            containerProjectsList = new QVBoxLayout;
            containerRightSide->addLayout(containerProjectsList);
        }
    }
    // SIGNALS
    connect(btnSearch, SIGNAL(clicked()), this, SLOT(search()));

}

void SearchView::search() {
    auto projects = SonstigesProjekt::query_all();
    for (const auto& project : projects) {
        auto vProject = new ProjectView(project, containerProjectsList, true);
        containerProjectsList->addLayout(vProject);
    }
}

void SearchView::openAddProject() {

}

void SearchView::loginLogout() {

}

void SearchView::openAdminView() {

}

void SearchView::addNewProject(int) {

}

void SearchView::toggleSort() {

}

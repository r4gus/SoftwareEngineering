#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H


#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include "ProjectView.h"

class SearchView : public QVBoxLayout
{
    Q_OBJECT
public:
    SearchView();


private:
    // GUI
    QLineEdit* tfSearchTitle;
    QLineEdit* tfSearchAuthor;
    QLineEdit* tfSearchLecturer;
    QLineEdit* tfSearchTags;
    QCalendarWidget* calendarSearchTimeFrom;
    QCalendarWidget* calendarSearchTimeTo;
    QComboBox* dropdownSearchType;
    QPushButton* btnSearch;
    QPushButton* btnAddProject;
    QVBoxLayout* containerProjectsList;
    QPushButton* btnLoginLogout;
    QPushButton* btnShowAdminView;
    // TODO add all


private Q_SLOTS:
    void search();
    void openAddProject();
    void loginLogout();
    void openAdminView();
    void addNewProject(int, ProjectType projectType);
    void toggleSort();
};

#endif // SEARCHVIEW_H


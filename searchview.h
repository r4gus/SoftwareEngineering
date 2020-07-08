#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QCalendarWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>



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
    void addNewProject(int);
    void toggleSort();
};

#endif // SEARCHVIEW_H


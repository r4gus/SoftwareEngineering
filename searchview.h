#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QLineEdit>
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


private Q_SLOTS:

};

#endif // SEARCHVIEW_H


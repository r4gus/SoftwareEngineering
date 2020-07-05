#include "searchview.h"

#include <QFormLayout>
#include <QTranslator>

SearchView::SearchView()
{
    QHBoxLayout* containerLeftSide = new QHBoxLayout();
    addLayout(containerLeftSide);
    {
        QFormLayout* containerSearch = new QFormLayout();
        containerLeftSide->addLayout(containerSearch);
        {
            tfSearchTitle = new QLineEdit;
            containerSearch->addRow(tr("Titel:"), tfSearchTitle);
            tfSearchAuthor = new QLineEdit;
            containerSearch->addRow(tr("Author:"), tfSearchAuthor);
            tfSearchLecturer = new QLineEdit;
            containerSearch->addRow(tr("Betreuer:"), tfSearchLecturer);
            tfSearchTags = new QLineEdit;
            containerSearch->addRow(tr("Tags:"), tfSearchTags);
        }
    }

    QHBoxLayout* containerRightSide = new QHBoxLayout();
    addLayout(containerRightSide);
    {

    }
}

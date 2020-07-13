//
// Created by Julian on 07.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_PROJECTVIEW_H
#define SOFTWAREENGINEERINGPROJECT_PROJECTVIEW_H


#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFrame>
#include "sonstigesprojekt.h"
#include "abschlussarbeit.h"
#include "projektarbeit.h"

enum ProjectType {
    OTHER, PROJECT, THESIS
};

// TODO: tr()   Translate
const std::map<ProjectType, QString> projectType2Name {
        {OTHER, "Sonstigesproject"},
        {PROJECT, "Projektarbeit"},
        {THESIS, "Abschlussarbeit"}
};

const std::map<QString, ProjectType> name2ProjectType {
        {projectType2Name.find(OTHER)->second, OTHER},
        {projectType2Name.find(PROJECT)->second, PROJECT},
        {projectType2Name.find(THESIS)->second, THESIS},
};

const QString TAGS_SEPARATOR = ";";

class ProjectView : public QFrame {
    Q_OBJECT

public:
    ProjectView(const SonstigesProjekt &project, QVBoxLayout *parent, bool editable, ProjectType projectType);
    ProjectView(const SonstigesProjekt& project, QVBoxLayout* parent, bool editable);
    ProjectView(const Abschlussarbeit& project, QVBoxLayout* parent, bool editable);
    ProjectView(const Projektarbeit& project, QVBoxLayout* parent, bool editable);

private:

    QPushButton* btnRemove;
    QPushButton* btnEdit;
    int projectId;
    QVBoxLayout* parent;
    bool editable;  // Changed from class diagram
    ProjectType projectType;
    QHBoxLayout *cRoot;

    void build(const SonstigesProjekt *p);
    void update(const SonstigesProjekt *project);

private Q_SLOTS:
    void remove();
    void openEditWindow();
    void edited(int, ProjectType);

};


#endif //SOFTWAREENGINEERINGPROJECT_PROJECTVIEW_H

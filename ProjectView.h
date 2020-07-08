//
// Created by Julian on 07.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_PROJECTVIEW_H
#define SOFTWAREENGINEERINGPROJECT_PROJECTVIEW_H


#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include "arbeit.h"

class ProjectView : public QVBoxLayout {
    Q_OBJECT

public:
    ProjectView(const Arbeit& project, QVBoxLayout* parent, bool editable);

private:
    QPushButton* btnRemove;
    QPushButton* btnEdit;
    int projectId;
    QVBoxLayout* parent;
    bool editable;  // Changed from class diagram

    void build(Arbeit project);
    void update(Arbeit project);

private Q_SLOTS:
    void remove();
    void openEditWindow();
    void edited(int);

};


#endif //SOFTWAREENGINEERINGPROJECT_PROJECTVIEW_H

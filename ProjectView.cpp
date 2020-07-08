//
// Created by Julian on 07.07.2020.
//

#include "ProjectView.h"
#include "gui_utils.h"

#include <utility>
#include <QtWidgets/QLabel>

ProjectView::ProjectView(const SonstigesProjekt& project, QVBoxLayout *parent, bool editable)
: projectId(project.id()), parent(parent), editable(editable)
{
    build(project);
}

void ProjectView::remove() {
    clearLayout(this);
    parent->removeItem(this);
}

void ProjectView::update(const SonstigesProjekt& project) {
    clearLayout(this);
    build(project);
}

void ProjectView::build(const SonstigesProjekt& project) {
    auto cRoot = new QHBoxLayout;
    addLayout(cRoot);
    {
        auto cLeft = new QVBoxLayout;
        cRoot->addLayout(cLeft);
        {
            auto cTop = new QHBoxLayout;
            cLeft->addLayout(cTop);
            {
                auto lblTitle = new QLabel(project.titel());
                cTop->addWidget(lblTitle);
                QString tags = project.stichwortliste().join(" - ");
                auto lblTags = new QLabel(tags);
                cTop->addWidget(lblTags);
            }
            auto cBot = new QHBoxLayout;
            cLeft->addLayout(cBot);
            {
                auto lblDescription = new QLabel(project.erlaeuterung());
                cBot->addWidget(lblDescription);
            }
        }
        auto cRight = new QVBoxLayout;
        cRoot->addLayout(cRight);
        {
            if (editable) {
                btnEdit = new QPushButton(tr("Edit"));
                cRight->addWidget(btnEdit);
                btnRemove = new QPushButton(tr("Remove"));
                cRight->addWidget(btnRemove);
            }
        }
    }

    // Signals and slots
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(openEditWindow()));
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
}

void ProjectView::openEditWindow() {

}

void ProjectView::edited(int) {

}

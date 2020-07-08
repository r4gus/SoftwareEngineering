//
// Created by Julian on 07.07.2020.
//

#include "ProjectView.h"

#include <utility>
#include <QtWidgets/QLabel>

ProjectView::ProjectView(const Arbeit& project, QVBoxLayout *parent, bool editable)
: projectId(project.id()), parent(parent), editable(editable)
{
    build(project);
}

void ProjectView::remove() {
    parent->removeItem(this);
}

void ProjectView::update(Arbeit project) {
    // TODO clear widget
    build(std::move(project));
}

void ProjectView::build(Arbeit project) {
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
                cLeft->addWidget(lblTitle);
                QString tags;
                for (auto tag : project.stichwortliste()) {
                    tags += tag;
                }

                auto lblTags = new QLabel()
            }
        }
        auto cRight = new QVBoxLayout;
        cRoot->addLayout(cRight);
        {

        }
    }
}

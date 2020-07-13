//
// Created by Julian on 07.07.2020.
//

#include "ProjectView.h"
#include "gui_utils.h"
#include "ProjectEditView.h"

#include <QDebug>
#include <utility>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFormLayout>

ProjectView::ProjectView(const SonstigesProjekt &project, QVBoxLayout *parent, bool editable, ProjectType projectType)
        : projectId(project.id()), parent(parent), editable(editable), projectType(projectType) {
    setProperty("style", "elevation-20");
    cRoot = new QHBoxLayout;

    setLayout(cRoot);
    // STYLE
    setFrameShape(QFrame::Shape::StyledPanel);
    setLineWidth(3);
}

ProjectView::ProjectView(const SonstigesProjekt &project, QVBoxLayout *parent, bool editable)
        : ProjectView(project, parent, editable, ProjectType::OTHER) {
    build(&project);
}

ProjectView::ProjectView(const Abschlussarbeit &project, QVBoxLayout *parent, bool editable)
        : ProjectView(project, parent, editable, ProjectType::THESIS) {
    build(&project);
}

ProjectView::ProjectView(const Projektarbeit &project, QVBoxLayout *parent, bool editable)
        : ProjectView(project, parent, editable, ProjectType::PROJECT) {
    build(&project);
}

void ProjectView::remove() {
    deleteLater();
    parent->removeWidget(this);
}

void ProjectView::update(const SonstigesProjekt *project) {
    clearLayout(cRoot);
    build(project);
}

void ProjectView::build(const SonstigesProjekt *p) {
    auto project = *p;
    auto cLeft = new QFormLayout;
    cRoot->addLayout(cLeft);
    {
        cLeft->addRow(tr("Titel:"), new QLabel(project.titel()));
        cLeft->addRow(tr("Bearbeiter:"), new QLabel(project.bearbeiter().fullName()));
        cLeft->addRow(tr("Stichworte:"), new QLabel(project.stichwortliste().join(" - ")));
        cLeft->addRow(tr("Betreuer:"), new QLabel(project.professor().fullName()));
        cLeft->addRow(tr("Beschreibung:"), new QLabel(project.erlaeuterung()));
        cLeft->addRow(tr("Status:"),
                      new QLabel(project.abgeschlossen() ? tr("Abgeschlossen") : tr("In Arbeit")));
        cLeft->addRow(tr("Studiengang:"),
                      new QLabel(project.studiengang().toString()));
        if (projectType == ProjectType::PROJECT) {
            auto projectSpecial = *dynamic_cast<const Projektarbeit*>(p);
            auto x = str(projectSpecial.semester());
            cLeft->addRow(tr("Semester:"), new QLabel(str(projectSpecial.semester())));
        }
        if (projectType == ProjectType::THESIS) {
            auto projectSpecial = *dynamic_cast<const Abschlussarbeit*>(p);
            cLeft->addRow(tr("Firma:"), new QLabel(projectSpecial.firma()));
            cLeft->addRow(tr("Zeitraum"),
                          new QLabel(
                                  projectSpecial.begin().toString("d.M.yy") + " - " +
                                  projectSpecial.end().toString("d.M.yy")
                          )
            );
        }
    }
    auto cRight = new QVBoxLayout;
    cRoot->addLayout(cRight);
    {
        if (editable) {
            int width = 80;
            int height = 20;
            btnEdit = new QPushButton(tr("Bearbeiten"));
            btnEdit->setFixedSize(width, height);
            cRight->addWidget(btnEdit);
            btnRemove = new QPushButton(tr("Löschen"));
            btnRemove->setFixedSize(width, height);
            cRight->addWidget(btnRemove);
            connect(btnEdit, SIGNAL(clicked()), this, SLOT(openEditWindow()));
            connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
        }
    }
}

void ProjectView::openEditWindow() {
    auto view = new ProjectEditView(projectId, projectType);
    auto popup = openPopup(view);
    connect(view, &ProjectEditView::requestClose, [popup]{ popup->close(); });
    connect(view, &ProjectEditView::saved, this, &ProjectView::edited);
}

void ProjectView::edited(int newID, ProjectType newType) {
    projectType = newType;
    projectId = newID;
    auto query = "arbeit.arbeitID='" + str(projectId) + "'";
    if (projectType == OTHER) {
        auto project = queryOne<SonstigesProjekt>(&SonstigesProjekt::query, query);
        update(&project);
    }
    if (projectType == THESIS) {
        auto project = queryOne<Abschlussarbeit>(&Abschlussarbeit::query, query);
        update(&project);
    }
    if (projectType == PROJECT) {
        auto project = queryOne<Projektarbeit>(&Projektarbeit::query, query);
        update(&project);
    }
}

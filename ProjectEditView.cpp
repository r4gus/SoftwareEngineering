//
// Created by Julian on 08.07.2020.
//

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QButtonGroup>
#include <QDebug>
#include "ProjectEditView.h"
#include "gui_utils.h"

ProjectEditView::ProjectEditView() {

    auto root = new QVBoxLayout;
    addLayout(root);
    {
        auto lblTitle = new QLabel(tr("Projekt hinzufügen")); // TODO: Change when edit
        root->addWidget(lblTitle);
        auto cFields = new QFormLayout;
        root->addLayout(cFields);
        {
            auto cType = new QHBoxLayout;
            cFields->addRow(tr("Art der Arbeit"), cType);
            {
                rbTypeThesis = new QRadioButton(tr("Abschlussarbeit"));
                cType->addWidget(rbTypeThesis);
                rbTypeProject = new QRadioButton(tr("Projekt"));
                cType->addWidget(rbTypeProject);
                rbTypeOther = new QRadioButton(tr("Sonstiges"));
                cType->addWidget(rbTypeOther);
                rbTypeOther->setChecked(true);
            }
            tfTitle = new QLineEdit;
            cFields->addRow(tr("Titel:"), tfTitle);
            tfAuthorFirstName = new QLineEdit;
            cFields->addRow(tr("Bearbeiter Vorname:"), tfAuthorFirstName);
            tfAuthorLastName = new QLineEdit;
            cFields->addRow(tr("Bearbeiter Nachname"), tfAuthorLastName);
            tfTags = new QLineEdit;
            cFields->addRow(tr("Stichwortliste"), tfTags);
            tfDescription = new QLineEdit;
            cFields->addRow(tr("Beschreibung:"), tfDescription);
            cbFinished = new QCheckBox;
            cFields->addRow(tr("Abgeschlossen:"), cbFinished);
            tfFieldOfStudy = new QLineEdit;
            cFields->addRow(tr("Schwerpunkt:"), tfFieldOfStudy);
            tfDegree = new QLineEdit;
            cFields->addRow(tr("Abschluss:"), tfDegree);
        }
        // Thesis fields
        auto cThesisFields = new QFormLayout;
        wThesisFields = new QWidget;
        wThesisFields->setLayout(cThesisFields);
        wThesisFields->setVisible(rbTypeThesis->isChecked() || !HIDE_FIELDS);
        wThesisFields->setEnabled(rbTypeThesis->isChecked());
        root->addWidget(wThesisFields);
        {
            calendarStart = new QCalendarWidget;
            cThesisFields->addRow(tr("Beginn:"), calendarStart);
            calendarFinish = new QCalendarWidget;
            cThesisFields->addRow(tr("Ende:"), calendarFinish);
            tfCompany = new QLineEdit;
            cThesisFields->addRow(tr("Firma:"), tfCompany);
        }
        auto cProjectFields = new QFormLayout;
        wProjectFields = new QWidget;
        wProjectFields->setLayout(cProjectFields);
        wProjectFields->setVisible(rbTypeProject->isChecked() || !HIDE_FIELDS);
        wProjectFields->setEnabled(rbTypeProject->isChecked());
        root->addWidget(wProjectFields);
        root->addLayout(cProjectFields);
        {
            tfSemester = new QLineEdit;
            cProjectFields->addRow(tr("Semester"), tfSemester);
        }

        auto cControlButtons = new QHBoxLayout;
        root->addLayout(cControlButtons);
        {
            btnSave = new QPushButton(tr("Hinzufügen"));
            cControlButtons->addWidget(btnSave);
            btnCancel = new QPushButton(tr("Abbrechen"));
            cControlButtons->addWidget(btnCancel);
        }
    }

    // SIGNALS and SLOTS
    connect(btnSave, SIGNAL(clicked()), this, SLOT(save()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    if (HIDE_FIELDS) {
        connect(rbTypeThesis, &QRadioButton::toggled, wThesisFields, &QWidget::setVisible);
        connect(rbTypeProject, &QRadioButton::toggled, wProjectFields, &QWidget::setVisible);
    }
    connect(rbTypeThesis, &QRadioButton::toggled, wThesisFields, &QWidget::setEnabled);
    connect(rbTypeProject, &QRadioButton::toggled, wProjectFields, &QWidget::setEnabled);
}

ProjectEditView::ProjectEditView(int projectId, ProjectType projectType) : ProjectEditView(){
    SonstigesProjekt projectCommon;
    auto query =  "arbeitID = '" + str(projectId) + "'";
    if (projectType == OTHER) {
        auto project = queryOne<SonstigesProjekt>(SonstigesProjekt::query, query);
        projectCommon = static_cast<SonstigesProjekt>(project);
        rbTypeOther->setChecked(true);
    }
    if (projectType == THESIS) {
        auto project = queryOne<Abschlussarbeit>(Abschlussarbeit::query, query);
        projectCommon = static_cast<SonstigesProjekt>(project);
        calendarStart->setSelectedDate(project.begin());
        calendarFinish->setSelectedDate(project.end());
        tfCompany->setText(project.firma());
        rbTypeThesis->setChecked(true);
    }
    if (projectType == PROJECT) {
        auto project = queryOne<Projektarbeit>(Projektarbeit::query, query);
        projectCommon = static_cast<SonstigesProjekt>(project);
        tfSemester->setText(str(project.semester()));
        rbTypeProject->setChecked(true);
    }

    // Common
    tfTitle->setText(projectCommon.titel());
    tfAuthorFirstName->setText(projectCommon.bearbeiter().vname());
    tfAuthorLastName->setText(projectCommon.bearbeiter().nname());
    tfTags->setText(projectCommon.stichwortliste().join("; "));
    tfDescription->setText(projectCommon.erlaeuterung());
    cbFinished->setChecked(projectCommon.abgeschlossen());
    tfFieldOfStudy->setText(projectCommon.studiengang().schwerpunkt());
    tfDegree->setText(projectCommon.studiengang().abschluss());

    btnSave->setText(tr("Speichern"));
}

void ProjectEditView::save() {
    // TODO
}

void ProjectEditView::cancel() {
    emit requestClose();
}





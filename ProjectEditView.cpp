//
// Created by Julian on 08.07.2020.
//

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QButtonGroup>
#include "ProjectEditView.h"

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
            }
            tfTitle = new QLineEdit;
            cFields->addRow(tr("Titel:"), tfTitle);
            tfAuthorFirstName = new QLineEdit;
            cFields->addRow(tr("Bearbeiter Vorname:"), tfAuthorFirstName);
            tfAuthorLastName = new QLineEdit;
            cFields->addRow(tr("Bearbeiter Nachname"), tfAuthorLastName);
            tfTags = new QLineEdit;
            cFields->addRow(tr("Stichwortliste"), tfTags);
            // TODO: special fields
        }
        auto cControlButtons = new QHBoxLayout;
        root->addLayout(cControlButtons);
        {
            btnSave = new QPushButton(tr("Hinzufügen")); // TODO: change when edit
            cControlButtons->addWidget(btnSave);
            btnCancel = new QPushButton(tr("Abbrechen"));
            cControlButtons->addWidget(btnCancel);
        }
    }

    // SIGNALS and SLOTS
    connect(btnSave, SIGNAL(clicked()), this, SLOT(save()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
}

ProjectEditView::ProjectEditView(int projectId) : ProjectEditView(){

}

void ProjectEditView::save() {
}

void ProjectEditView::cancel() {

}





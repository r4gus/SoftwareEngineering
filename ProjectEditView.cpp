//
// Created by Julian on 08.07.2020.
//

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QButtonGroup>
#include <QDebug>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QTextEdit>
#include "ProjectEditView.h"
#include "gui_utils.h"
#include "mainwindow.h"

ProjectEditView::ProjectEditView()
    : isEdit(false), projectID(-1)
{

    auto root = new QVBoxLayout;
    addLayout(root);
    {
        lblViewTitle = new QLabel(tr("Projekt hinzufügen"));
        root->addWidget(lblViewTitle);
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
            tfTags->setPlaceholderText("tag1;tag2");
            cFields->addRow(tr("Stichwortliste"), tfTags);
            tfDescription = new QTextEdit;
            cFields->addRow(tr("Beschreibung:"), tfDescription);
            cbFinished = new QCheckBox;
            cFields->addRow(tr("Abgeschlossen:"), cbFinished);
            cbStudy = new QComboBox;
            auto studies = Studiengang::query_all();
            for (auto &study : studies) {
                cbStudy->insertItem(0, study.toString());
            }
            cFields->addRow(tr("Studiengang"), cbStudy);
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
            tfSemester->setValidator(new QRegExpValidator(QRegExp("\\d{1,2}"))); // Only number inputs
            cProjectFields->addRow(tr("Semester"), tfSemester);
        }

        lblErrorMessage = new QLabel;
        root->addWidget(lblErrorMessage);

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
    this->projectID = projectId;
    this->isEdit = true;
    originalProjectType = projectType;

    lblViewTitle->setText(tr("Projekt bearbeiten"));

    SonstigesProjekt projectCommon;
    auto query =  "arbeit.arbeitID = '" + str(projectId) + "'";
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
    cbStudy->setCurrentIndex(cbStudy->findText(projectCommon.studiengang().toString()));
    btnSave->setText(tr("Speichern"));
}

void ProjectEditView::save() {
    // TODO: proper update
    // TODO: FIX: nutzer are not updated/changed -> When the name of the user is updated it's not updated in the DB and GUI
    // Reset state
    lblErrorMessage->setText("");

    if (isEdit) {
        auto originalTmpProject = SonstigesProjekt();
        originalTmpProject.setId(projectID);
        DB::session().remove(originalTmpProject);
    }

    // Common
    auto title = tfTitle->text();
    auto authorFirstName = tfAuthorFirstName->text();
    auto authorLastName = tfAuthorLastName->text();
    auto tags = tfTags->text().split(TAGS_SEPARATOR);
    auto description = tfDescription->toMarkdown();
    auto finished = cbFinished->isChecked();
    auto study = Studiengang::fromString(cbStudy->currentText());
    // TODO: validate input?
    auto professor = MainWindow::get().user;
    auto student = Nutzer(authorFirstName, authorLastName, authorFirstName + authorLastName, Nutzer::Role::student); // TODO: email?
    int locProjectID;
    ProjectType projectType;

    SonstigesProjekt* projectCommon;
    if (rbTypeProject->isChecked()) {
        projectType = PROJECT;
        auto project = Projektarbeit(title, tags, finished, description);
        projectCommon = &project;
        project.setSemester(tfSemester->text().toInt());
    }
    if (rbTypeThesis->isChecked()) {
        projectType = THESIS;
        auto project = Abschlussarbeit(title, tags, finished, description);
        projectCommon = &project;
        project.setBegin(calendarStart->selectedDate());
        project.setEnd(calendarFinish->selectedDate());
        project.setFirma(tfCompany->text());
    }
    if (rbTypeOther->isChecked()) {
        projectType = OTHER;
        auto project = SonstigesProjekt(title, tags, finished, description);
        projectCommon = &project;
    }
    projectCommon->setBearbeiter(student);
    projectCommon->setProfessor(professor);
    projectCommon->setStudiengang(study);
    try {
        auto o1 = rbTypeOther->isChecked();
        auto o2 = rbTypeProject->isChecked();
        auto o3 = rbTypeThesis->isChecked();
        if (rbTypeOther->isChecked()) {
            locProjectID = DB::session().add(*projectCommon);
        }
        else if (rbTypeProject->isChecked()) {
            locProjectID = DB::session().add((Projektarbeit&)*projectCommon);
        }
        if (rbTypeThesis->isChecked()) {
            locProjectID = DB::session().add((Abschlussarbeit&)*projectCommon);
        }
    } catch (exception &e) {
        lblErrorMessage->setText(tr("Fehler beim speichern: ") + e.what());
    }

    if (lblErrorMessage->text().size() == 0) {
        emit saved(locProjectID, projectType);
        emit requestClose();
    }
}

void ProjectEditView::cancel() {
    emit requestClose();
}





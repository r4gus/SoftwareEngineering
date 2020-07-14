//
// Created by Julian on 08.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_PROJECTEDITVIEW_H
#define SOFTWAREENGINEERINGPROJECT_PROJECTEDITVIEW_H


#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QTextEdit>
#include "sonstigesprojekt.h"
#include "ProjectView.h"

class ProjectEditView : public QVBoxLayout{
    Q_OBJECT

private:
    static const bool HIDE_FIELDS = false; // Non editable fields are hidden instead of only disabled
    QLabel* lblViewTitle;
    QLineEdit* tfTitle;
    QLineEdit* tfAuthorFirstName;
    QLineEdit* tfAuthorLastName;
    QLineEdit* tfTags;
    QTextEdit* tfDescription;
    QCheckBox* cbFinished;
    QRadioButton* rbTypeThesis;
    QRadioButton* rbTypeProject;
    QRadioButton* rbTypeOther;
    QCalendarWidget* calendarStart;
    QCalendarWidget* calendarFinish;
    QLineEdit* tfCompany;
    QLineEdit* tfSemester;
    QComboBox* cbStudy;
    QWidget* wThesisFields;
    QWidget* wProjectFields;

    QPushButton* btnSave;
    QPushButton* btnCancel;

    QLabel* lblErrorMessage;

    bool isEdit;
    int projectID;
    int studentID;
    ProjectType originalProjectType;

    void validateInputField(const QString &content, const QString &name, bool *error);

public:
    ProjectEditView();
    ProjectEditView(int projectId, ProjectType projectType);

private Q_SLOTS:
    void save();
    void cancel();

Q_SIGNALS:

    void saved(int, ProjectType);

    void requestClose();
};


#endif //SOFTWAREENGINEERINGPROJECT_PROJECTEDITVIEW_H

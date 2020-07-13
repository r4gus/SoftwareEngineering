#include "lecturereditview.h"
#include "ui_lecturereditview.h"
#include "nutzer.h"
#include "DB.h"
#include <QtWidgets/QPushButton>

LecturerEditView::LecturerEditView(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LecturerEditView),
        isEdit(false), lecturerID(-1) {
    ui->setupUi(this);
    connect(ui->btnSave, &QPushButton::clicked, [this] { save(); });
    connect(ui->btnCancel, &QPushButton::clicked, [this] { cancel(); });
}

LecturerEditView::LecturerEditView(int userID, QWidget *parent) :
        LecturerEditView(parent) {
    isEdit = true;
    lecturerID = userID;
    auto user = queryOne<Nutzer>(Nutzer::query, "nutzerID=" + str(userID));
    ui->tfFirstName->setText(user.vname());
    ui->tfLastName->setText(user.nname());
    ui->tfEmail->setText(user.email());
    ui->tfPassword->setText("");
    ui->btnSave->setText(tr("Speichern"));
    passwordHash = user.password_hash();
    password_salt = user.password_salt();
    personalWorkFactor = user.personal_work_factor();
}

LecturerEditView::~LecturerEditView() {
    delete ui;
}

void LecturerEditView::save() {
    ui->lblErrorMessage->setText(tr(""));

    auto firstName = ui->tfFirstName->text();
    auto lastName = ui->tfLastName->text();
    auto email = ui->tfEmail->text();
    auto password = ui->tfPassword->text();
    if (firstName.size() == 0 || lastName.size() == 0 || email.size() == 0 || (!isEdit && password.size() == 0)) {
        ui->lblErrorMessage->setText(tr("Nicht alle erforderlichen Felder ausgefüllt"));
        return;
    }
    auto n = Nutzer(firstName, lastName, email, Nutzer::Role::dozent);
    if (isEdit) {
        n.setId(lecturerID);
        if (password.size() != 0) {
            n.set_password(password);
        } else {
            n.setPassword_salt(password_salt);
            n.set_password_hash(passwordHash);
            n.setPersonal_work_factor(personalWorkFactor);
        }
        // TODO: handle failure
        try {
            DB::session().update(n);
        } catch (DBException e) {
            qDebug() << e.err();
            qDebug() << e.what();
            ui->lblErrorMessage->setText(tr("Fehler beim erstellen"));
        }
    } else {
        n.set_password(password);
        try {
            lecturerID = DB::session().add(n);
        } catch (DBException e) {
            qDebug() << e.err();
            qDebug() << e.what();
            ui->lblErrorMessage->setText(tr("Fehler beim erstellen"));
        }
    }
    emit saved(lecturerID);
    emit requestClose();
}

void LecturerEditView::cancel() {
    emit requestClose();
}

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
    auto users = Nutzer::query("nutzerID=" + str(userID));
    if (users.size() == 1) {
        auto user = users[0];
        ui->tfFirstName->setText(user.vname());
        ui->tfLastName->setText(user.nname());
        ui->tfEmail->setText(user.email());
        ui->tfPassword->setText("");
        ui->btnSave->setText(tr("Speichern"));
    } else {
        qDebug() << "Error: Can't find user with ID in DB: " + str(userID);
    }
}

LecturerEditView::~LecturerEditView() {
    delete ui;
}

void LecturerEditView::save() {
    auto firstName = ui->tfFirstName->text();
    auto lastName = ui->tfLastName->text();
    auto email = ui->tfEmail->text();
    auto password = ui->tfPassword->text();
    // TODO: Input validation
    auto n = Nutzer(firstName, lastName, email, Nutzer::Role::dozent);
    if (isEdit) {
        n.setId(lecturerID);
        if (password.size() != 0) {
            n.set_password(password);
        } else {
            n.set_password("TODO"); // TODO: Update without changing the password
        }
        // TODO: handle failure
        DB::session().update(n);
    } else {
        n.set_password(password);
        // TODO: handle failure
        lecturerID = DB::session().add(n);
    }
    emit saved(lecturerID);
    emit requestClose();
}

void LecturerEditView::cancel() {
    emit requestClose();
}

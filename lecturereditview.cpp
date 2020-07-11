#include "lecturereditview.h"
#include "ui_lecturereditview.h"
#include "nutzer.h"
#include "DB.h"
#include <QtWidgets/QPushButton>

LecturerEditView::LecturerEditView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LecturerEditView)
{
    ui->setupUi(this);
    connect(ui->btnSave, &QPushButton::clicked, [this] { save(); });
    connect(ui->btnCancel, &QPushButton::clicked, [this] { cancel(); });
}

LecturerEditView::~LecturerEditView()
{
    delete ui;
}

void LecturerEditView::save() {
    auto firstName = ui->tfFirstName->text();
    auto lastName = ui->tfLastName->text();
    auto email = ui->tfEmail->text();
    auto password = ui->tfPassword->text();
    auto n = Nutzer(firstName, lastName, email, Nutzer::Role::dozent);
    n.set_password(password);
    DB::session().add(n);

    emit saved();
    emit requestClose();
}

void LecturerEditView::cancel() {
    emit requestClose();
}

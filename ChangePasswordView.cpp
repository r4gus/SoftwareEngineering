//
// Created by Julian on 09.07.2020.
//

#include <QtWidgets/QFormLayout>
#include "ChangePasswordView.h"
#include "mainwindow.h"
#include "searchview.h"


ChangePasswordView::ChangePasswordView() {
    auto root = new QVBoxLayout;
    addLayout(root);
    {
        auto lblTitle = new QLabel(tr("Passwort ändern"));
        root->addWidget(lblTitle);
        auto cFields = new QFormLayout;
        root->addLayout(cFields);
        {
            tfPassword1 = new QLineEdit;
            cFields->addRow(tr("Neues Passwort:"), tfPassword1);
            tfPassword2 = new QLineEdit;
            cFields->addRow(tr("Neues Passwort wiederholen:"), tfPassword2);
        }
        lblErrorMessage = new QLabel;
        root->addWidget(lblErrorMessage);
        auto cControlButtons = new QHBoxLayout;
        root->addLayout(cControlButtons);
        {
            btnChange = new QPushButton(tr("Ändern"));
            cControlButtons->addWidget(btnChange);
            btnCancel = new QPushButton(tr("Abbrechen"));
            cControlButtons->addWidget(btnCancel);
        }
    }

    // SIGNALS and SLOTS
    connect(btnChange, SIGNAL(clicked()), this, SLOT(change()));
    connect(btnCancel, &QPushButton::clicked, [this]{ emit requestClose(); });
}

void ChangePasswordView::change() {

}

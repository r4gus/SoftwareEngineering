//
// Created by Julian on 09.07.2020.
//

#include <QtWidgets/QFormLayout>
#include <QtWidgets/qaction.h>
#include "LoginView.h"
#include "gui_utils.h"
#include "ChangePasswordView.h"


LoginView::LoginView() {
    auto root = new QVBoxLayout;
    addLayout(root);
    {
        auto lblTitle = new QLabel(tr("Anmelden"));
        root->addWidget(lblTitle);
        auto cFields = new QFormLayout;
        root->addLayout(cFields);
        {
            tfUsername = new QLineEdit;
            cFields->addRow(tr("Benutzername"), tfUsername);
            tfPassword = new QLineEdit;
            cFields->addRow(tr("Passwort"), tfPassword);
        }
        lblErrorMessage = new QLabel;
        root->addWidget(lblErrorMessage);
        auto cControlButtons = new QHBoxLayout;
        root->addLayout(cControlButtons);
        {
            btnLogin = new QPushButton(tr("Anmelden"));
            cControlButtons->addWidget(btnLogin);
            btnLoginChangePassword = new QPushButton(tr("Anmelden + Passwort ändern"));
            cControlButtons->addWidget(btnLoginChangePassword);
            btnCancel = new QPushButton(tr("Abbrechen"));
            cControlButtons->addWidget(btnCancel);
        }
    }

    // SIGNALS and SLOTS
    connect(btnLogin, SIGNAL(clicked()), this, SLOT(test()));
    connect(btnLoginChangePassword, &QPushButton::clicked, [this]{login(true); });
    // TODO: btnCancel
}


void LoginView::login(bool changePassword) {
    // TODO: if first login -> changePassword = true
    if (changePassword) {
        // close
        openPopup(new ChangePasswordView);
    } else {
        // close
    }
}

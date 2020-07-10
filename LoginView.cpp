//
// Created by Julian on 09.07.2020.
//

#include <QtWidgets/QFormLayout>
#include <QtWidgets/qaction.h>
#include "LoginView.h"
#include "gui_utils.h"
#include "ChangePasswordView.h"
#include "nutzer.h"
#include "DB.h"


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
    connect(btnLoginChangePassword, &QPushButton::clicked, [this]{login(true); });
    connect(btnLogin, &QPushButton::clicked, [this]{login(false); });
    // TODO: btnCancel
}


void LoginView::login(bool changePassword) {
    // TODO: if first login -> changePassword = true
    // TODO: remove debug
    Nutzer n1("Roland", "Dietrich", "1", Nutzer::Role::dozent);
    n1.set_password("1");
    DB::session().add(n1);

    bool successfulLogin = false;
    auto userName = tfUsername->text();
    auto password = tfPassword->text();
    auto users = Nutzer::query("email='" + userName + "'");
    if (users.size() == 1) {
        auto user = users[0];
        if (user.check_password(password)) {
            successfulLogin = true;
        }
    }
    if (successfulLogin) {
        if (changePassword) {
            // Crappy workaround to "replace" login view by new view
            replaceLayout(this, new ChangePasswordView);
        } else {
            auto parentDialog = (QDialog*) parentWidget();
            parentDialog->close();
        }
    } else {
        lblErrorMessage->setText(tr("Falsches Passwort oder Benutzername"));
    }
}

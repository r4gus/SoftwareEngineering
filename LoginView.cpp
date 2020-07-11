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
#include "mainwindow.h"
#include "searchview.h"
#include "AdminView.h"


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
    connect(btnCancel, &QPushButton::clicked, [this]{ emit requestClose(); });
}


void LoginView::login(bool changePassword) {
    // TODO: if first login -> changePassword = true
    // TODO: remove debug
    Nutzer n1("Roland", "Dietrich", "1", Nutzer::Role::dozent);
    n1.set_password("1");
    DB::session().add(n1);
    Nutzer n2("Roland", "Dietrich", "2", Nutzer::Role::administrator);
    n2.set_password("2");
    DB::session().add(n2);

    bool successfulLogin = false;
    auto userName = tfUsername->text();
    auto password = tfPassword->text();
    try {
        auto user = queryOne<Nutzer>(Nutzer::query, "email='" + userName + "'");
        if (user.check_password(password)) {
            successfulLogin = true;
            MainWindow::get().user = user;
        }
    } catch (DBException &e) {
        successfulLogin = false;
    }
    if (successfulLogin) {
        if (MainWindow::get().user.is_administrator()) {
            MainWindow::get().showView(new AdminView);
        } else {
            MainWindow::get().showView(new SearchView);
        }

        if (changePassword) {
            // Crappy workaround to "replace" login view by new view
            auto newView = new ChangePasswordView;
            replaceLayout(this, newView);
            connect(newView, &ChangePasswordView::requestClose, [this]{ emit requestClose(); });
        } else {
            emit requestClose();
        }
    } else {
        lblErrorMessage->setText(tr("Falsches Passwort oder Benutzername"));
    }
}

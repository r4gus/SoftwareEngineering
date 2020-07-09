//
// Created by Julian on 09.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_LOGINVIEW_H
#define SOFTWAREENGINEERINGPROJECT_LOGINVIEW_H


#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

class LoginView : public QVBoxLayout {
    Q_OBJECT

private:
    QLineEdit* tfUsername;
    QLineEdit* tfPassword;
    QPushButton* btnLogin;
    QPushButton* btnLoginChangePassword;
    QPushButton *btnCancel;
    QLabel* lblErrorMessage;

public:
    LoginView();

public Q_SLOTS:
    void login(bool changePassword);
};


#endif //SOFTWAREENGINEERINGPROJECT_LOGINVIEW_H

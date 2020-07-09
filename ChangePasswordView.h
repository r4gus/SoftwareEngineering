//
// Created by Julian on 09.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_CHANGEPASSWORDVIEW_H
#define SOFTWAREENGINEERINGPROJECT_CHANGEPASSWORDVIEW_H


#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

class ChangePasswordView : public QVBoxLayout{
    Q_OBJECT

private:
    QLineEdit* tfPassword1;
    QLineEdit* tfPassword2;
    QPushButton* btnChange;
    QPushButton* btnCancel;
    QLabel* lblErrorMessage;

public:
    ChangePasswordView();

private Q_SLOTS:
    void change();
};


#endif //SOFTWAREENGINEERINGPROJECT_CHANGEPASSWORDVIEW_H

#ifndef NUTZER_H
#define NUTZER_H

#include <QString>
#include <string>
#include <cstdlib>  // srand, rand
#include <ctime>    // time
#include <cmath>
#include "sha512.h"

/*!
 * \brief The Nutzer class represents a user on the system.
 *
 * There are active and passive users, determined by the _active attribute (true = active).
 * Active users can log into their account using their email and password. Passive users act as plain
 * data class.
 */
class Nutzer
{
public:
    Nutzer();

    void set_password(QString input);
    bool check_password(QString input);

private:
    int _id = -1;
    QString _vname;
    QString _nname;
    QString _email;
    QString _password_hash;
    QString _password_salt;
    unsigned int _personal_work_factor;
    int _role;
    bool _active;
    const static unsigned int WORK_FACTOR = 12;
};

QString generate_random_salt();

#endif // NUTZER_H

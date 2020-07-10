#ifndef NUTZER_H
#define NUTZER_H

#include <QString>
#include <string>
#include <cstdlib>  // srand, rand
#include <ctime>    // time
#include <cmath>
#include <vector>
#include "sha512.h"

/*!
 * \brief The Nutzer class represents a user on the system.
 *
 * There are active and passive users, determined by the _active attribute (true = active).
 * Active users can log into their account using their email and password. Passive users act as plain
 * data class.
 *
 * A user can have one of three roles: student, dozent, administrator.
 * A student user is always inactive and used as data class. dozent and student are
 * active by default.
 *
 * A new password should be set immediately after instantiating this class. Every student get has a DEFAULT_PASSWORD.
 */
class Nutzer
{
public:
    enum Role : unsigned int {
        create_student = 1,
        edit_student = 2,
        delete_student = 4,
        create_dozent = 8,
        edit_dozent = 16,
        delete_dozent = 32,
        create_arbeit = 64,
        edit_arbeit = 128,
        delete_arbeit = 256,
        student = 0,
        dozent = create_student + edit_student + create_arbeit + edit_arbeit + delete_arbeit,
        administrator = create_student + edit_student + delete_student + create_arbeit + edit_arbeit + delete_arbeit +
                        create_dozent + edit_dozent + delete_dozent,
    };


    Nutzer() {}

    /*!
     * \brief Nutzer Instantiate the Nutzer class
     * \param vname First name
     * \param nname Second name
     * \param email E-Mail
     * \param role The Role of the user
     *
     * If a user is of type Role::student, the account is automatically set inactive
     * and and the default password is set to "password".
     */
    Nutzer(QString vname, QString nname, QString email, int role) :
        _vname(vname), _nname(nname), _email(email), _role(role) {
        if(role == Role::student) {
            _active = false;
            set_password("password");
        }
    };

    void set_password(QString input);
    bool check_password(QString input);

    bool is_student() { return _role == Role::student; }
    bool is_dozent() { return _role == Role::dozent; }
    bool is_administrator() { return _role == Role::administrator; }

    int id() const;
    void setId(int id);

    QString vname() const;
    void setVname(const QString &vname);

    QString nname() const;
    void setNname(const QString &nname);

    QString email() const;
    void setEmail(const QString &email);

    int role() const;
    void setRole(int role);

    bool active() const;
    void setActive(bool active);

    static std::vector<Nutzer> query_all();
    static std::vector<Nutzer> query(const QString &input);

    QString password_hash() const;
    void set_password_hash(QString s) {_password_hash = s;}

    QString password_salt() const;
    void setPassword_salt(const QString &password_salt);

    unsigned int personal_work_factor() const;
    void setPersonal_work_factor(unsigned int personal_work_factor);

private:
    int _id = -1;
    QString _vname;
    QString _nname;
    QString _email;
    QString _password_hash;
    QString _password_salt;
    unsigned int _personal_work_factor;
    int _role;
    bool _active = true;

    const static unsigned int WORK_FACTOR = 12;
};

QString generate_random_salt();

#endif // NUTZER_H

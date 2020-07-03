#include "nutzer.h"
#include "DB.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

/* #################### PASSWORD ####################################################*/

/*!
 * \brief generate_random_salt returns a pseudo random salt string
 * \return salt of type QString
 */
QString
generate_random_salt()
{
    srand(time(NULL));          // initialize random number generator with seed time
    static const char chars[] =
            "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "!§$%&/()=?'#*+~-_<>|@";
    const unsigned int MAX_LENGTH = 128;
    const unsigned int MIN_LENGTH = 32;
    unsigned int iterations = rand() % MAX_LENGTH;
    iterations = iterations < MIN_LENGTH ? iterations + MIN_LENGTH : iterations;
    QString salt = "";

    for(unsigned int i = 0; i < iterations; i++) {
        salt += chars[rand() % (sizeof(chars) - 1)];
    }

    return salt;
}

/*!
 * \brief Nutzer::set_password sets the given input as password
 * \param input The password string
 *
 * The input parameter is prepended with a random salt and then
 * hashed 2^WORK_FACTOR times with the SHA512 hash algorithm.
 */
void
Nutzer::set_password(QString input)
{
    _password_salt = generate_random_salt();    // set salt
    _personal_work_factor = WORK_FACTOR;        // set work factor
    unsigned int rounds = (unsigned int) pow(2.0, _personal_work_factor);
    QString hash = _password_salt + input;      // prepend salt to password

    for(unsigned int i = 0; i < rounds; i++) {  // hash the shit out of the string
        hash = sha512(hash);
    }

    _password_hash = hash;                      // set hash as password
}

/*!
 * \brief Nutzer::check_password checks if the give password is the same as the stored one.
 * \param input The password
 * \return true if the passwords match, false otherwise
 *
 * The function prepends the stored hash to the input string, hashes the result and then
 * compares it to the stored _password_hash.
 */
bool
Nutzer::check_password(QString input)
{
    QString hash = _password_salt + input;
    unsigned int rounds = (unsigned int) pow(2.0, _personal_work_factor);

    for(unsigned int i = 0; i < rounds; i++) {
        hash = sha512(hash);
    }

    return _password_hash == hash;
}


/* #################### GETTER/ SETTER ####################################################*/

int Nutzer::id() const
{
    return _id;
}

void Nutzer::setId(int id)
{
    _id = id;
}

QString Nutzer::vname() const
{
    return _vname;
}

void Nutzer::setVname(const QString &vname)
{
    _vname = vname;
}

QString Nutzer::nname() const
{
    return _nname;
}

void Nutzer::setNname(const QString &nname)
{
    _nname = nname;
}

QString Nutzer::email() const
{
    return _email;
}

void Nutzer::setEmail(const QString &email)
{
    _email = email;
}

int Nutzer::role() const
{
    return _role;
}

void Nutzer::setRole(int role)
{
    _role = role;
}

bool Nutzer::active() const
{
    return _active;
}

void Nutzer::setActive(bool active)
{
    _active = active;
}

QString Nutzer::password_hash() const
{
    return _password_hash;
}

QString Nutzer::password_salt() const
{
    return _password_salt;
}

void Nutzer::setPassword_salt(const QString &password_salt)
{
    _password_salt = password_salt;
}

unsigned int Nutzer::personal_work_factor() const
{
    return _personal_work_factor;
}

void Nutzer::setPersonal_work_factor(unsigned int personal_work_factor)
{
    _personal_work_factor = personal_work_factor;
}


/* #################### QUERY ####################################################*/

/*!
 * \brief query_all returns a vector with all objects from the database.
 * \return A vector of Nutzer objects
 */
std::vector<Nutzer>
Nutzer::query_all()
{
    QString query_string = "";
    return Nutzer::query(query_string);
}

/*!
 * \brief query Get a subset of the database entries as objects
 * \param s The condition string
 * \return A vector of Studiengang objects
 *
 * The method expects a string of type: "key1 >= 'val1' and key2 = 'val2' or ..."
 *
 * The method acts like query_all() if an empty string is provided.
 */
std::vector<Nutzer>
Nutzer::query(QString &s)
{
    std::vector<Nutzer> vec;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString qs;

    if(!db.isValid()) throw InvalidDatabaseError();

    qs += "SELECT nutzerID, VName, NName, email, password, salt, workFactor, role, active FROM nutzer ";
    if(!s.isEmpty()) qs += " WHERE " + s;
    qs += ";";

    log("Query", qs);
    if( !query.exec(qs) ) {
        throw DatabaseTransactionError(query.lastError().text());
    }

    while( query.next() ) {
        int id = query.value(0).toInt();
        QString vname = query.value(1).toString();
        QString nname = query.value(2).toString();
        QString email = query.value(3).toString();
        QString password = query.value(4).toString();
        QString salt = query.value(5).toString();
        int work_factor = query.value(6).toInt();
        int role = query.value(7).toInt();
        bool active = query.value(8).toBool();

        Nutzer nutzer(vname, nname, email, role);
        nutzer.setId(id);
        nutzer.setActive(active);
        nutzer.set_password_hash(password);
        nutzer.setPassword_salt(salt);
        nutzer.setPersonal_work_factor(work_factor);
        vec.push_back(nutzer);
    }

    return vec;
}




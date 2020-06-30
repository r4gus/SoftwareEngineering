#include "nutzer.h"

Nutzer::Nutzer()
{

}

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

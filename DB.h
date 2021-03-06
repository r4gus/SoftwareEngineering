#ifndef DB_H
#define DB_H

#include <string>
#include <QException>
#include <QDebug>
#include <QDateTime>
#include <QSqlDatabase>
#include "sonstigesprojekt.h"
#include "projektarbeit.h"
#include "abschlussarbeit.h"
#include "studiengang.h"
#include "nutzer.h"

using namespace std;

/*! \brief singelton representing a data base

    This class is used as a wrapper around QSql. The intention is to
    make it easy to add, remove, and select database entries without
    the need to write SQL queries.
 */
class DB {
public:
    /*!
     * \brief session Obtain the db singelton object.
     * \return A reference to the db object
     */
    static DB& session() {
        static DB instance;
        return instance;
    }

    /*!
     * \brief Insert a Studiengang object into the database.
     * \param s A reference to the Studiengang object
     * \return true on success, false otherwise
     * \throws InvalidDatabaseError, DatabaseTransactionError
     *
     * The internal functionality depends on the value of _id. A value of -1 indicates
     * that the object doesn't correspond to an entry inside the database, e.g. it has
     * just been create. The method tries to add the object to the database.
     * If the object does already exist, an exception is thrown. If the value is >= 0
     * the method tries to update the corresponding database entry.
     */
    int add(Studiengang &s);   //!< insert a Studiengang object into the database - throws exception
    int add(Nutzer &s); //!< throws exception
    int add(SonstigesProjekt &s); //!< throws excetion
    int add(Projektarbeit &s); //!< throws exception
    int add(Abschlussarbeit &s);

    int update(Nutzer &s); //!< throws exception
    int update(SonstigesProjekt &s); //!< throws excetion
    int update(Projektarbeit &s); //!< throws exception
    int update(Abschlussarbeit &s);

    bool remove(Nutzer &s);
    bool remove(SonstigesProjekt &s);
    bool remove(Projektarbeit &s);
    bool remove(Abschlussarbeit &s);

    /*!
     * \brief Initializes the database.
     *
     * Creates the required tables if they don't exist.
     *
     * \return true on success, false otherwise.
     */
    static bool initialize(QSqlDatabase &db);

    /*!
     * \brief Populate the given database with test data.
     * \param db The database to populate
     * \return ture on success, false otherwise
     */
    static bool test(QSqlDatabase &db);

    /*!
     * \brief clean drops all tables and initializes the database again.
     * \param db The database to clean
     * \return true on success, false otherwise
     */
    static bool clean(QSqlDatabase &db);

private:
    DB();
    ~DB();
    DB(DB const&);              // copy constructor is private
    DB& operator=(DB const&);   // assignment operator is private
    QString _db_path;
    QString _conf_path;

};

/* ############################### EXCETPION ################################### */

/*!
 * \brief The DBException class defines custom exceptions related to the DB class.
 */
class DBException : public QException {
    QString msg;
public:
    void raise() const override { throw *this; }
    DBException *clone() const override { return new DBException(*this); }

    /*!
     * \brief DBException is the constructor.
     * \param m A description of the current error
     *
     * All exceptions are logged automatically by using qDebug.
     */
    DBException(const QString& m="DBException") : msg(m) {
        qDebug() << "[" << QDateTime::currentDateTime().toString() << "][Error]: " << msg;
    }
    const QString& err() { return msg; }
};

/*!
 * \brief The InvalidDatabaseError class is thrown if an database error occures.
 */
class InvalidDatabaseError : public DBException {
public:
    InvalidDatabaseError(const QString& m="InvaldiDatabaseError") : DBException(m) {}
};

/*!
 * \brief The DatabaseTransactionError class is thrown if an transaction couldn't be executed.
 */
class DatabaseTransactionError : public DBException {
public:
    DatabaseTransactionError(const QString& m="DatabaseTransactionError") : DBException(m) {}
};

/*!
 * \brief log Logs the specified message by using qDebug().
 * \param type Message type
 * \param msg Message string
 */
void log(const QString& type, const QString& msg);

void addStudiengang();
#endif // DB_H

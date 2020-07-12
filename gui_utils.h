//
// Created by Julian on 08.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_GUI_UTILS_H
#define SOFTWAREENGINEERINGPROJECT_GUI_UTILS_H

#include <QtWidgets/QLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QScrollArea>
#include "functional"

#include "sonstigesprojekt.h"
#include "DB.h"


void clearLayout(QLayout* layout);

QDialog* openPopup(QLayout* content);

QDialog* openPopup(QWidget* content);

void replaceLayout(QBoxLayout* parent, QLayout* child);

QString str(int i);

QVBoxLayout* buildScrollContainer(QLayout* parent);


/**
 * Make a DB query qith a unique key. Should only return one object.
 * If not exactly one is returned an exception is thrown.
 *
 * (Implementation is in header file, because templates can't be easily implemented in a cpp file.)
 *
 * @tparam T Type that should be returned and is queried.
 * @param func query function
 * @param query query string
 * @return queried Object
 */

template <class T>
T queryOne(std::function<std::vector<T>(QString)> func, const QString &query) {
    auto results = func(query);
    if (results.size() == 1) {
        return results[0];
    } else {
        qDebug() << "Error: Can't find object in DB with query: " + query;
        throw DBException("Error: Can't find object in DB with query: " + query);
    }
}

class QueryBuilder {

public:
    void add(const QString &key, const QString &value);
    void add(const QString &key, int value);
    QString build();
private:
    QString query = "";
    void addAnd(const QString &condition);
};

#endif //SOFTWAREENGINEERINGPROJECT_GUI_UTILS_H

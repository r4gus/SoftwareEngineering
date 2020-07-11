//
// Created by Julian on 08.07.2020.
//

#ifndef SOFTWAREENGINEERINGPROJECT_GUI_UTILS_H
#define SOFTWAREENGINEERINGPROJECT_GUI_UTILS_H

#include <QtWidgets/QLayout>
#include <QtWidgets/QDialog>

void clearLayout(QLayout* layout);

QDialog* openPopup(QLayout* content);

QDialog* openPopup(QWidget* content);

void replaceLayout(QBoxLayout* parent, QLayout* child);

QString str(int i);

#endif //SOFTWAREENGINEERINGPROJECT_GUI_UTILS_H

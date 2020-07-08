//
// Created by Julian on 08.07.2020.
//
#include "gui_utils.h"

#include <QWidget>

void clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout);
        delete item;
    }
}
//
// Created by Julian on 08.07.2020.
//
#include "gui_utils.h"

#include <QtWidgets/QPushButton>

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

QDialog* openPopup(QLayout* content){
    auto pDialog = new QDialog;
    pDialog->setModal(false);
    {
        pDialog->setLayout(content);
    }
    pDialog->show();
    return pDialog;
}

QDialog* openPopup(QWidget* content){
    auto layout = new QVBoxLayout;
    layout->addWidget(content);
    return openPopup(layout);
}


void replaceLayout(QBoxLayout* parent, QLayout* child) {
    clearLayout(parent);
    parent->addLayout(child);
}
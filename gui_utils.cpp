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

QString str(int i) {
    return QString::fromStdString(std::to_string(i));
}

QVBoxLayout* buildScrollContainer(QLayout* parent) {
    auto wScrollAreaContent = new QWidget;
    auto cContent = new QVBoxLayout(wScrollAreaContent);
    wScrollAreaContent->setLayout(cContent);
    auto scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(wScrollAreaContent);
    scrollArea->setWidgetResizable(true);
    parent->addWidget(scrollArea);
    return cContent;
}


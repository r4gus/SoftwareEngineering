//
// Created by Julian on 08.07.2020.
//
#include "gui_utils.h"
#include "DB.h"

#include <QtWidgets/QPushButton>
#include <QDebug>
#include <utility>

void clearLayout(QLayout *layout) {
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        if (QLayout *childLayout = item->layout())
            clearLayout(childLayout);
        delete item;
    }
}

QDialog *openPopup(QLayout *content) {
    auto pDialog = new QDialog;
    pDialog->setProperty("style", "background");
    pDialog->setModal(false);
    {
        pDialog->setLayout(content);
    }
    pDialog->show();
    return pDialog;
}

QDialog *openPopup(QWidget *content) {
    auto layout = new QVBoxLayout;
    layout->addWidget(content);
    return openPopup(layout);
}


void replaceLayout(QBoxLayout *parent, QLayout *child) {
    clearLayout(parent);
    parent->addLayout(child);
}

QString str(int i) {
    return QString::number(i);
}

QString str(unsigned int i) {
    return QString::number(i);
}

QVBoxLayout *buildScrollContainer(QLayout *parent) {
    auto wScrollAreaContent = new QWidget;
    wScrollAreaContent->setProperty("style", "elevation-10");
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



Condition::Condition(const QString &key, const QString &value) {
    if (value.size() > 0) {
        condition = "UPPER(" + key + ") LIKE UPPER('%" + value + "%')";
    } else {
        condition = "";
    }
}

Condition::Condition(QString condition)
        : condition(std::move(condition)) {

}

Condition Condition::operator&&(const Condition &other) const {
    return concat(other, "AND");
}

Condition Condition::operator||(const Condition &other) const {
    return concat(other, "OR");
}

Condition Condition::concat(const Condition &other, const QString &op) const {
    if (condition.size() > 0) {
        if (other.condition.size() > 0) {
            return Condition{"(" + condition + " " + op + " " + other.condition + ")" };
        }
        return Condition{condition};
    } else {
        return Condition{other.condition};
    }
}

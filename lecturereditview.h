#ifndef LECTUREREDITVIEW_H
#define LECTUREREDITVIEW_H

#include <QWidget>
#include "gui_utils.h"

namespace Ui {
    class LecturerEditView;
}

class LecturerEditView : public QWidget {
Q_OBJECT

public:
    explicit LecturerEditView(QWidget *parent = nullptr);
    LecturerEditView(int userID, QWidget *parent = nullptr);

    ~LecturerEditView() override;

private:
    Ui::LecturerEditView *ui;
    bool isEdit;
    int lecturerID;
    QString passwordHash;
    QString password_salt;
    unsigned int personalWorkFactor;

private slots:

    void save();

    void cancel();

signals:

    void requestClose();

    void saved(int);
};

#endif // LECTUREREDITVIEW_H

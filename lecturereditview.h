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

    ~LecturerEditView() override;

private:
    Ui::LecturerEditView *ui;

private slots:

    void save();

    void cancel();

signals:

    void requestClose();

    void saved();
};

#endif // LECTUREREDITVIEW_H

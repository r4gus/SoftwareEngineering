#ifndef LECTUREREDITVIEW_H
#define LECTUREREDITVIEW_H

#include <QWidget>

namespace Ui {
class LecturerEditView;
}

class LecturerEditView : public QWidget
{
    Q_OBJECT

public:
    explicit LecturerEditView(QWidget *parent = nullptr);
    ~LecturerEditView();

private:
    Ui::LecturerEditView *ui;
};

#endif // LECTUREREDITVIEW_H

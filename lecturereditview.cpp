#include "lecturereditview.h"
#include "ui_lecturereditview.h"

LecturerEditView::LecturerEditView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LecturerEditView)
{
    ui->setupUi(this);
}

LecturerEditView::~LecturerEditView()
{
    delete ui;
}

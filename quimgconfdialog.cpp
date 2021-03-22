#include "quimgconfdialog.h"
#include "ui_quimgconfdialog.h"
#include <math.h>

QuImgConfDialog::QuImgConfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfDialog)
{
    ui->setupUi(this);
}

QuImgConfDialog::~QuImgConfDialog()
{
    delete ui;
}

QString QuImgConfDialog::getColorTableName() const
{
    foreach(QRadioButton *rb, ui->palette_groupBox->findChildren<QRadioButton *>())
        if(rb->isChecked())
            return rb->property("paletteName").toString();
    return "c";
}

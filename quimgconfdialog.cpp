#include "quimgconfdialog.h"
#include "ui_quimgconfdialog.h"
#include <QPushButton>
#include <QtDebug>

QuImgConfDialog::QuImgConfDialog(QuImgConfDialogListener* li, QWidget *parent) :
    QDialog(parent)
{
    qDebug() << __PRETTY_FUNCTION__ << "before setup ui";
    ui = new Ui::QuImgConfDialog;
    ui->setupUi(this);
    l = li;
    qDebug() << __PRETTY_FUNCTION__ << "bubox" << ui->buttonBox;
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(applyClicked()));
    ui->cbScaleWithZoom->setChecked(parent->property("scaleWithZoom").toBool());
    setWindowTitle("Image configuration");
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

void QuImgConfDialog::applyClicked() {
    QMap<QString, QVariant> conf;
    conf["color_table"] = getColorTableName();
    conf["scale_with_zoom"] = ui->cbScaleWithZoom->isChecked();
    conf["fit_to_widget"] = ui->cbFitToWidget->isChecked();
    l->onApply(conf);
}

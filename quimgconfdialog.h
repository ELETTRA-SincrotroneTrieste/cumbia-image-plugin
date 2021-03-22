#ifndef QUIMAGECONFDIALOG_H
#define QUIMAGECONFDIALOG_H

#include <QDialog>
#include <QVector>
#include <QRgb>

namespace Ui {
class ConfDialog;
}

class QuImgConfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuImgConfDialog(QWidget *parent = 0);
    ~QuImgConfDialog();

    QString getColorTableName() const;

private:
    Ui::ConfDialog *ui;

};

#endif // QUIMAGECONFDIALOG_H

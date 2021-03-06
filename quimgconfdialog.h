#ifndef QUIMAGECONFDIALOG_H
#define QUIMAGECONFDIALOG_H

#include <QDialog>
#include <QVector>
#include <QRgb>
#include <QMap>

namespace Ui {
class QuImgConfDialog;
}

class QuImgConfDialogListener {
public:
    virtual void onApply(const QMap<QString, QVariant> &conf) = 0;
};

class QuImgConfDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QuImgConfDialog(QuImgConfDialogListener* li, QWidget *parent);
    ~QuImgConfDialog();
    QString getColorTableName() const;

public slots:
    void applyClicked();

private:
    Ui::QuImgConfDialog *ui;
    QuImgConfDialogListener *l;
};


#endif // QUIMAGECONFDIALOG_H

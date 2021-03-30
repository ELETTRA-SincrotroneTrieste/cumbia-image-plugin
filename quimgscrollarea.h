#ifndef QUIMGSCROLLAREA_H
#define QUIMGSCROLLAREA_H

#include <QScrollArea>
#include <quimageplugininterface.h>

class QuImgScrollAreaPrivate;

class QuImgScrollArea : public QScrollArea, public QuImgScrollAreaI
{
    Q_OBJECT
public:
    QuImgScrollArea(QWidget *parent);
    ~QuImgScrollArea();

    bool scrollEnabled() const;

    // QuImgScrollAreaI interface
public:
    QScrollArea *scrollArea();
    void setImage(QuImageBaseI *ii);

public slots:
    void onZoomRectChanged(const QRect &from, const QRect &to);
    void setScaleContents(bool scale);

private:
    QuImgScrollAreaPrivate *d;
};

#endif // QUIMGSCROLLAREA_H

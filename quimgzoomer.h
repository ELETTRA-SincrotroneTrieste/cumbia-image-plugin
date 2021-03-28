#ifndef QUIMGZOOMER_H
#define QUIMGZOOMER_H

#include <QRect>
#include <QList>

class QuImageBasePrivate;
class QPoint;
class QRect;

class QuImgZoomerPrivate;

class QuImgZoomer
{
public:
    QuImgZoomer(bool enable, QuImageBasePrivate *img_d);
    virtual ~QuImgZoomer();
    void setLevel(float n);
    float level() const;
    void setEnabled(bool en);
    bool enabled() const;

    QRect unzoom();
    QRect zoom(const QRect& r);

    QList<QRect> zoomStack() const;
private:
    QuImgZoomerPrivate *d;
};

#endif // QUIMGZOOMER_H

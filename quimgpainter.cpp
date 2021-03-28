#include "quimgpainter.h"
#include "quimagebase.h"
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <math.h>
#include "quimagebase.h"
#include <QPen>
#include <QtDebug>

QuImgPainter::QuImgPainter()
{

}

QuImgPainter::~QuImgPainter()
{

}

void QuImgPainter::paint(QPaintEvent *e, QuImageBasePrivate *imgb_d)
{
    if(imgb_d->error)
        imgb_d->image = imgb_d->errorImage;
    else if(imgb_d->image.isNull())
        imgb_d->image = (imgb_d->noise);

    QPainter p(imgb_d->widget);
    QRect imgRect = imgb_d->image.rect();
    imgRect.setSize(imgb_d->image.size() * (imgb_d->zoomer->level()/100.0));
    if(imgb_d->fit_to_w) imgRect.setSize(e->rect().size());
    else imgRect.setSize(imgb_d->image.size() * (imgb_d->zoomer->level()/100.0));
    //    qDebug() << __FUNCTION__ << "zoom: " << imgb->zoom << "imgRect" << imgRect << " paint device geom " << paintDevice->geometry() <<
    //                " paint rect " << e->rect();
    p.drawImage(imgRect, imgb_d->image, imgb_d->image.rect());


    //
    // p.drawImage(0, 0, imgb->image);
    if(!imgb_d->mP1.isNull() && !imgb_d->mP2.isNull())
    {
        QPen pen = p.pen();
        pen.setWidthF(2.0);
        QRect r(QPoint(0,0), imgb_d->widget->geometry().size());
        if(!r.contains(imgb_d->mP2))
            pen.setColor(Qt::red);
        else
            pen.setColor(Qt::green);

        p.setPen(pen);

        int p1x = imgb_d->mP1.x() * 100 / imgb_d->zoomer->level();
        int p1y = imgb_d->mP1.y() * 100 / imgb_d->zoomer->level();
        int p2x = imgb_d->mP2.x() * 100 / imgb_d->zoomer->level();
        int p2y = imgb_d->mP2.y() * 100 / imgb_d->zoomer->level();

        QFont f = p.font();
        f.setBold(true);
        p.setFont(f);
        QFontMetrics fm(p.font());

        QColor bg = Qt::white;
        bg.setAlpha(184);

        if(!imgb_d->leftButton)
        {
            float fl = sqrt(pow(p2x - p1x, 2) + pow(p2y - p1y, 2));
            QString val = QString("%1").arg(fl, 0, 'f', 2);
            p.fillRect(QRectF(imgb_d->mP1.x(), imgb_d->mP1.y(), fm.width(val), fm.height()), bg);
            /* first draw line */
            p.drawLine(imgb_d->mP1, imgb_d->mP2);
            /* then text */
            pen.setColor(Qt::black);
            p.setPen(pen);
            p.drawText(QPointF(imgb_d->mP1.x(), imgb_d->mP1.y() + fm.height()), val);
        }
        else
        {
            p.drawRect(QRect(imgb_d->mP1, imgb_d->mP2));
            p.setPen(bg);
            p.fillRect(QRect(imgb_d->mP1, imgb_d->mP2), bg);
            /* then text */
            QString val = QString("%1,%2 %3x%4").arg(p1x).arg(p1y).arg(fabs(p2x - p1x)).
                    arg(fabs(p2y - p1y));
            p.fillRect(QRectF(imgb_d->mP1.x(), imgb_d->mP1.y(), fm.width(val), fm.height()), bg);
            pen.setColor(Qt::black);
            p.setPen(pen);
            p.drawText(QPointF(imgb_d->mP1.x(), imgb_d->mP1.y() + fm.height()), val);
        }
    }
    if(imgb_d->error)
    {
        QPen errPen(Qt::red);
        errPen.setWidth(5);
        p.setPen(errPen);
        /* draw a cross on the figure */
        if(imgb_d->errorImage.isNull())
        {
            p.drawLine(e->rect().topLeft(), e->rect().bottomRight());
            p.drawLine(e->rect().bottomLeft(), e->rect().topRight());
        }
        else /* yeah! draw ok guy! */
        {
            if(imgb_d->errorImage.size() != e->rect().size())
                imgb_d->errorImage = imgb_d->errorImage.scaled(e->rect().size(), Qt::KeepAspectRatio);
            p.drawImage(e->rect(), imgb_d->errorImage);
        }

        QRect txtRect = e->rect();
        QColor bgColor = Qt::white;
        bgColor.setAlpha(180);
        p.fillRect(txtRect, bgColor);
        /* draw the error message */
        QFont f = p.font();
        f.setPointSize(10);
        f.setBold(true);
        p.setFont(f);
        qDebug() << __FUNCTION__ << "drawing error message";
        p.drawText(txtRect, imgb_d->errorMessage);
    }
}

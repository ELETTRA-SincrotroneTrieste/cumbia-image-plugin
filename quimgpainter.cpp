#include "quimgpainter.h"
#include "quimagebase.h"
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <math.h>
#include "quimagebase.h"
#include <QPen>
#include <QtDebug>

QuImgPainter::QuImgPainter() : dirty(true) {

}

QuImgPainter::~QuImgPainter() {

}

void QuImgPainter::paint(QPaintEvent *e, QuImageBasePrivate *imgb_d) {
    if(imgb_d->error)
        imgb_d->image = imgb_d->errorImage;
    if(imgb_d->image.isNull()) // no image and no errorImage: use noise
        imgb_d->image = (imgb_d->noise);

    QPainter p(imgb_d->widget);
    QRect imgRect = imgb_d->image.rect();
    imgRect.setSize(imgb_d->image.size() * (imgb_d->zoomer->level()/100.0));
    if(imgb_d->scale_contents) imgRect.setSize(e->rect().size());
    else imgRect.setSize(imgb_d->image.size() * (imgb_d->zoomer->level()/100.0));
    const float &irw = static_cast<float>(imgRect.width());
    const float &irh = static_cast<float>(imgRect.height());
    if(dirty &&  fabs(irh / imgb_d->image.height() - irw / imgb_d->image.width()) > 0.01) {
        imgb_d->cached_img = imgb_d->image.scaled(imgRect.width(), imgRect.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        p.drawImage(imgb_d->cached_img.rect(), imgb_d->cached_img, imgb_d->cached_img.rect());
    } else if(dirty) {
        imgb_d->cached_img = imgb_d->image;
    }
    if(dirty)
        dirty = false;
    const QRect pRect(imgb_d->scale_contents ? imgb_d->cached_img.rect() : imgRect);
    p.drawImage(pRect, imgb_d->cached_img, imgb_d->cached_img.rect());

    // draw selection rect
    if(!imgb_d->mouse_t->mP1.isNull() && !imgb_d->mouse_t->mP2.isNull() && imgb_d->zoomer->mouseEnabled())  {
        QPen pen = p.pen();
        pen.setWidthF(2.0);
        QRect r(QPoint(0,0), imgb_d->widget->geometry().size());
        !r.contains(imgb_d->mouse_t->mP2) ? pen.setColor(Qt::red) : pen.setColor(Qt::green);
        p.setPen(pen);

        int p1x = imgb_d->mouse_t->mP1.x() * 100 / imgb_d->zoomer->level();
        int p1y = imgb_d->mouse_t->mP1.y() * 100 / imgb_d->zoomer->level();
        int p2x = imgb_d->mouse_t->mP2.x() * 100 / imgb_d->zoomer->level();
        int p2y = imgb_d->mouse_t->mP2.y() * 100 / imgb_d->zoomer->level();

        QFont f = p.font();
        f.setBold(true);
        p.setFont(f);
        QFontMetrics fm(p.font());

        QColor bg = Qt::white;
        bg.setAlpha(184);

        if(!imgb_d->mouse_t->left_button) {
            float fl = sqrt(pow(p2x - p1x, 2) + pow(p2y - p1y, 2));
            QString val = QString("%1").arg(fl, 0, 'f', 2);
            p.fillRect(QRectF(imgb_d->mouse_t->mP1.x(), imgb_d->mouse_t->mP1.y(), fm.horizontalAdvance(val), fm.height()), bg);
            /* first draw line */
            p.drawLine(imgb_d->mouse_t->mP1, imgb_d->mouse_t->mP2);
            /* then text */
            pen.setColor(Qt::black);
            p.setPen(pen);
            p.drawText(QPointF(imgb_d->mouse_t->mP1.x(), imgb_d->mouse_t->mP1.y() + fm.height()), val);
        } else  {
            p.drawRect(QRect(imgb_d->mouse_t->mP1, imgb_d->mouse_t->mP2));
            p.setPen(bg);
            p.fillRect(QRect(imgb_d->mouse_t->mP1, imgb_d->mouse_t->mP2), bg);
            /* then text */
            QString val = QString("%1,%2 %3x%4").arg(p1x).arg(p1y).arg(fabs(p2x - p1x)).
                    arg(fabs(p2y - p1y));
            p.fillRect(QRectF(imgb_d->mouse_t->mP1.x(), imgb_d->mouse_t->mP1.y(), fm.horizontalAdvance(val), fm.height()), bg);
            pen.setColor(Qt::black);
            p.setPen(pen);
            p.drawText(QPointF(imgb_d->mouse_t->mP1.x(), imgb_d->mouse_t->mP1.y() + fm.height()), val);
        }
    }
}

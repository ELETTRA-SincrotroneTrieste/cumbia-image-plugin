#include "quimgmousetracker.h"
#include "quimagemouseeventif.h"
#include "quimagebase.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>

QuImgMouseTracker::QuImgMouseTracker(QuImageBasePrivate* _img_d, QuImgZoomer *zoome)
    : img_d(_img_d), mou_if(nullptr), zoomer(zoome) {

}

void QuImgMouseTracker::setMouseEventInterface(QuImageMouseEventIf *i) {
    mou_if = i;
}

void QuImgMouseTracker::mouseMove(QMouseEvent *ev) {
    if(!mP1.isNull())
        mP2= ev->pos();
    if(mou_if)
        mou_if->imageMouseMoveEvent(ev->pos(), ev->button());
    img_d->widget->update();
}

void QuImgMouseTracker::mousePress(QMouseEvent *ev)
{
    mP1 = ev->pos();
    left_button = (ev->button() == Qt::LeftButton);
    if(mou_if)
        mou_if->imageMousePressEvent(ev->pos(), ev->button());
    img_d->widget->update();
}

void QuImgMouseTracker::mouseRelease(QMouseEvent *ev) {
    /* call the release method only if the button was pressed inside the label
     */
    QRect r(QPoint(0,0), img_d->widget->geometry().size());
    if(mou_if && !mP1.isNull() && r.contains(ev->pos()))
        mou_if->imageMouseReleaseEvent(ev->pos(), ev->button());
    else if(zoomer->mouseEnabled() && !mP1.isNull() && ev->button() == Qt::LeftButton &&
            (ev->pos() - mP1).manhattanLength() > 3) {  // use rect to zoom
        QRect sr(mP1, mP2);
        if(!sr.isValid())  sr = QRect(mP2, mP1);
        QRect zr = zoomer->zoom(img_d->mapToImg(sr));
        QList<QRect> zs = zoomer->zoomStack();
        if(img_d->listener) img_d->listener->onZoom(zs.size() > 1 ? zs[zs.size() - 2] : QRect(), zr);
    }
    else if(zoomer->mouseEnabled() && !mou_if) {
        QList<QRect> zs = zoomer->zoomStack();
        // get current zoom rect before unzooming - the last in the zoom stack
        QRect from = zs.size() > 0 ? zs[zs.size() - 1] : QRect();
        QRect zr = zoomer->unzoom();
        if(img_d->listener) img_d->listener->onZoom(from, zr);
    }
    mP1 = QPoint();
    mP2 = QPoint();
    img_d->widget->update();
}

void QuImgMouseTracker::wheelEvent(QWheelEvent *event) {
    if(mou_if)
        mou_if->imageMouseWheelEvent(event);
}


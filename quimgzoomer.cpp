#include "quimgzoomer.h"
#include "quimagebase.h"

class QuImgZoomerPrivate {
public:
    float level;
    bool enabled;
    QList<QRect> zoom_stack;
    QuImageBasePrivate *img_d;
};

QuImgZoomer::QuImgZoomer(bool enable, QuImageBasePrivate* img_d)
{
    d = new QuImgZoomerPrivate;
    d->img_d = img_d;
    d->level = 100;
    d->enabled = enable;
}

QuImgZoomer::~QuImgZoomer() {
    delete d;
}

void QuImgZoomer::setLevel(float n) { d->img_d->level = n; }

void QuImgZoomer::setEnabled(bool en) {
    d->enabled = en;
}

bool QuImgZoomer::enabled() const {
    return d->enabled;
}

float QuImgZoomer::level() const {
    return d->level;
}

void QuImgZoomer::unzoom(const QPoint &pos) {
    if(!d->enabled)
        return;
    qDebug() << __PRETTY_FUNCTION__ << "unzoom";
    foreach(const QRect& r, d->zoom_stack)
        qDebug() << "-" << r;
    if(d->zoom_stack.size() > 1) {
        d->zoom_stack.takeLast(); // remove current from stack
        m_zoom(d->zoom_stack.last()); // restore previous zoom rect
        if(d->listener)
            d->listener->onZoom(d->zoom_stack.last());
    }
//    if(d->zoom_stack.size() == 1)
//        d->zoom_stack.clear();
}

void QuImgZoomer::zoom(const QRect &r)
{
    if(!d->enabled)
        return;
    if(d->img_d->zoom_stack.isEmpty())
        d->img_d->zoom_stack << d->img_d->widget->geometry();
    const QRect& g0 =  d->img_d->zoom_stack[0];
    const QRect geom = d->img_d->widget->geometry();
    QRect sel = r.intersected(d->img_d->image.rect());
//    QRect visibleR = d->img_d->widget->visibleRegion().boundingRect();
    const float &rw = g0.width()/(float) sel.width();
    const float &rh = g0.height()/(float) sel.height();
    float factor = qMax(rw, rh); // max ratio
    d->img_d->zoom = 100 * factor;
    QRect nr = QRect(QPoint(g0.x(), g0.y()), g0.size() * factor);
    d->img_d->widget->setGeometry(nr);
    d->img_d->widget->updateGeometry();
    qDebug() << __PRETTY_FUNCTION__ << "zoom rect in img coords" << r << "img rect" << d->img_d->image.rect() << "selection " << sel
             <<  "geom before " << geom << "after " << d->img_d->widget->geometry() << "zoom" << d->img_d->zoom << "factor " << factor;
}

void QuImgZoomer::m_zoom(const QRect &selectionRect)
{

}

QList<QRect> QuImgZoomer::zoomStack() const {
    return d->img_d->zoom_stack;
}

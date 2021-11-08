#include "quimgzoomer.h"
#include "quimagebase.h"
#include <QtDebug>

class QuImgZoomerPrivate {
public:
    float level;
    bool mouse_enab;
    QList<QRect> zoom_stack;
    QuImageBasePrivate *img_d;
};

QuImgZoomer::QuImgZoomer(bool enable, QuImageBasePrivate* img_d)
{
    d = new QuImgZoomerPrivate;
    d->img_d = img_d;
    d->level = 100;
    d->mouse_enab = enable;
}

QuImgZoomer::~QuImgZoomer() {
    delete d;
}

void QuImgZoomer::setLevel(float n) {
    if(n != 100) {
        d->mouse_enab = false;
        d->zoom_stack.clear();
     }
    d->level = n;
}

void QuImgZoomer::setMouseEnabled(bool en) {
    if(en) d->level = 100;
    else d->zoom_stack.clear();
    d->mouse_enab = en;
}

bool QuImgZoomer::mouseEnabled() const {
    return d->mouse_enab;
}

float QuImgZoomer::level() const {
    return d->level;
}

QRect QuImgZoomer::unzoom() {
    if(!d->mouse_enab)
        return QRect();
    foreach(const QRect& r, d->zoom_stack)
        qDebug() << "-" << r;
    if(d->zoom_stack.size() > 1) {
        d->zoom_stack.takeLast(); // remove current from stack
        m_zoom(d->zoom_stack.last()); // restore previous zoom rect
    }
    return d->zoom_stack.size() > 0 ? d->zoom_stack.last() : QRect();
}

/*!
 * \brief zoom the rect r, which is in image coordinates!
 * \param r the zoom rectangle in image coordinates
 */
QRect QuImgZoomer::zoom(const QRect &r)
{
    if(!d->mouse_enab)
        return QRect();
    m_zoom(r);
    d->zoom_stack << r;
    return r;
}

QList<QRect> QuImgZoomer::zoomStack() const {
    return d->zoom_stack;
}
void QuImgZoomer::m_zoom(const QRect &r) {
    if(!r.isValid())
        return;
    if(d->zoom_stack.isEmpty())
        d->zoom_stack << d->img_d->image.rect();
    const QRect& g0 =  d->zoom_stack[0];
    const QRect geom = d->img_d->widget->geometry();
    QRect sel = r.intersected(d->img_d->image.rect());
    const float &rw = g0.width()/(float) sel.width();
    const float &rh = g0.height()/(float) sel.height();
    float factor = qMin(rw, rh); // min ratio
    d->level = 100 * factor;
    QRect nr = QRect(QPoint(g0.x(), g0.y()), g0.size() * factor);
    d->img_d->widget->setGeometry(nr);
    d->img_d->widget->updateGeometry();
}

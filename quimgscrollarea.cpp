#include "quimgscrollarea.h"
#include <QtDebug>

class QuImgScrollAreaPrivate {
public:
    QuImageBaseI *ii;
};

QuImgScrollArea::QuImgScrollArea(QWidget *parent) : QScrollArea(parent)
{
    d = new QuImgScrollAreaPrivate;
    d->ii = nullptr;
}

QuImgScrollArea::~QuImgScrollArea() {
    delete d;
}

bool QuImgScrollArea::scrollEnabled() const {
    return widgetResizable();
}

void QuImgScrollArea::setScaleContents(bool scale) {
    setWidgetResizable(scale);
    d->ii->setScaleContents(scale);
}

QScrollArea *QuImgScrollArea::scrollArea() {
    return this;
}

void QuImgScrollArea::setImage(QuImageBaseI *ii) {
    d->ii = ii;
    if(ii->asWidget() != this->widget()) {
        setWidget(ii->asWidget());
        connect(ii->asWidget(), SIGNAL(zoomRectChanged(QRect,QRect)), this, SLOT(onZoomRectChanged(QRect,QRect)));
    }
}

// r in image coordinates
void QuImgScrollArea::onZoomRectChanged(const QRect &from, const QRect &to) {
    qDebug() << __PRETTY_FUNCTION__ << from << to << "from contains to ? "  << from.contains(to);
    QRect zr = d->ii->mapFromImg(to);
    qDebug() << __PRETTY_FUNCTION__ << "zoom r in widget coords" << zr << "widget geom" << widget()->geometry()
             << "ensuring visible bottom " << zr.bottomRight();
    ensureVisible(zr.center().x(), zr.center().y(), zr.width()/1.9, zr.height()/1.9);
}

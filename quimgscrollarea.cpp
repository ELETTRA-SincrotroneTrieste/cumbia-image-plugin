#include "quimgscrollarea.h"
#include "quimagewidget.h"
#include <QtDebug>

class QuImgScrollAreaPrivate {
public:
    QuImageBaseI *ii;
};

QuImgScrollArea::QuImgScrollArea(QWidget *parent, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo) : QScrollArea(parent)
{
    d = new QuImgScrollAreaPrivate;
    d->ii = new QuImageWidget(parent, cu_p, fpoo);
    d->ii->asWidget()->setObjectName("image_w");
    d->ii->setScaleContents(false);
    setWidget(d->ii->asWidget());
    connect(d->ii->asWidget(), SIGNAL(zoomRectChanged(QRect,QRect)), this, SLOT(onZoomRectChanged(QRect,QRect)));
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

void QuImgScrollArea::setAutoScale(bool as) {
    d->ii->setAutoScale(as);
}

void QuImgScrollArea::setLowerBound(double lb) {
    d->ii->setLowerBound(lb);
}

void QuImgScrollArea::setUpperBound(double ub) {
    d->ii->setUpperBound(ub);
}

QScrollArea *QuImgScrollArea::scrollArea() {
    return this;
}

void QuImgScrollArea::setImage(QuImageBaseI *ii) {
    if(d->ii)
        delete d->ii;
    d->ii = ii;
    if(ii->asWidget() != this->widget()) {
        setWidget(ii->asWidget());
        connect(ii->asWidget(), SIGNAL(zoomRectChanged(QRect,QRect)), this, SLOT(onZoomRectChanged(QRect,QRect)));
    }
}

bool QuImgScrollArea::scaleContents() const {
    return d->ii->scaleContents();
}

bool QuImgScrollArea::autoScale() const {
    return d->ii->autoScale();
}

double QuImgScrollArea::upperBound() const {
    return d->ii->upperBound();
}

double QuImgScrollArea::lowerBound() const {
    return d->ii->lowerBound();
}

// r in image coordinates
void QuImgScrollArea::onZoomRectChanged(const QRect &from, const QRect &to) {
    qDebug() << __PRETTY_FUNCTION__ << from << to << "from contains to ? "  << from.contains(to);
    QRect zr = d->ii->mapFromImg(to);
    qDebug() << __PRETTY_FUNCTION__ << "zoom r in widget coords" << zr << "widget geom" << widget()->geometry()
             << "ensuring visible bottom " << zr.bottomRight();
    ensureVisible(zr.center().x(), zr.center().y(), zr.width()/1.9, zr.height()/1.9);
}

bool QuImgScrollArea::mouseZoomEnabled() const {
    return d->ii->mouseZoomEnabled();
}

QImage QuImgScrollArea::image() const {
    return d->ii->image();
}

void QuImgScrollArea::setMouseZoomEnabled(bool e)
{
    d->ii->setMouseZoomEnabled(e);
}

QImage QuImgScrollArea::errorImage() const {
    return d->ii->errorImage();
}

void QuImgScrollArea::setErrorImage(const QImage &eimg) {
    d->ii->setErrorImage(eimg);
}

QString QuImgScrollArea::source() const {
    return d->ii->source();
}

void QuImgScrollArea::setSource(const QString &s) {
    d->ii->setSource(s);
}

QWidget *QuImgScrollArea::imageWidget() const {
    return d->ii->asWidget();
}

QuImageBaseI *QuImgScrollArea::imageBase() const {
    return d->ii;
}

void QuImgScrollArea::setImage(const QImage &img) {
    d->ii->setImage(img);
}

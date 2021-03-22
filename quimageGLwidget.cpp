#include "quimageGLwidget.h"
#include <QtDebug>

class QuImageGLWidgetPrivate {
public:
    QuImageBase *imgb;
};

QuImageGLWidget::QuImageGLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {
    d = new QuImageGLWidgetPrivate();
    d->imgb = new QuImageBase(this, true);
}

QuImageGLWidget::~QuImageGLWidget()
{
    delete d->imgb;
    delete d;
}

QWidget *QuImageGLWidget::asWidget() const {
    return d->imgb->asWidget();
}

void QuImageGLWidget::setImage(const QImage& img)
{
    d->imgb->setImage(img);
    /* get a reference to the image */
  //  QImage& imgRef = image();
  //  setGeometry(x(), y(), imgRef.width(), imgRef.height());
    update();
}

void QuImageGLWidget::mousePressEvent(QMouseEvent *ev)
{
    d->imgb->mousePress(ev);
}

void QuImageGLWidget::mouseMoveEvent(QMouseEvent *ev)
{
    d->imgb->mouseMove(ev);
}

void QuImageGLWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    d->imgb->mouseRelease(ev);
}

QImage &QuImageGLWidget::image() const
{
    return d->imgb->image();
}

void QuImageGLWidget::setErrorImage(const QImage &i) {
    d->imgb->setErrorImage(i);
}

QImage QuImageGLWidget::errorImage() const {
    return d->imgb->errorImage();
}

void QuImageGLWidget::setError(bool error)
{
    d->imgb->setError(error);
}

void QuImageGLWidget::setErrorMessage(const QString &msg) {
    d->imgb->setErrorMessage(msg);
}

void QuImageGLWidget::setColorTable(const QVector<QRgb> &rgb) {
    d->imgb->setColorTable(rgb);
}

QVector<QRgb> &QuImageGLWidget::colorTable() const {
    return d->imgb->colorTable();
}

void QuImageGLWidget::setImageMouseEventInterface(ImageMouseEventInterface *ifa)
{
    d->imgb->setImageMouseEventInterface(ifa);
}

bool QuImageGLWidget::isOpenGL() const {
    return true;
}

bool QuImageGLWidget::zoomEnabled() const {
    return d->imgb->zoomEnabled();
}

void QuImageGLWidget::setZoom(int n) {
    d->imgb->setZoom(n);
}

void QuImageGLWidget::setZoomEnabled(bool en) {
    d->imgb->setZoomEnabled(en);
}

float QuImageGLWidget::zoom() const {
    return d->imgb->zoom();
}

void QuImageGLWidget::paintEvent(QPaintEvent *e)
{
    d->imgb->paint(e, this);
}

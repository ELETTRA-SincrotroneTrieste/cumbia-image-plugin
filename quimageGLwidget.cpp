#include "quimageGLwidget.h"
#include <QtDebug>

class QuImageGLWidgetPrivate {
public:
    QuImageGLWidgetPrivate(QuImageBase *img) : imgb(img) { }
    QuImageBase *imgb;
};

QuImageGLWidget::QuImageGLWidget(QWidget *parent, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo) :
    QOpenGLWidget(parent) {
    d = new QuImageGLWidgetPrivate(new QuImageBase(this, false, cu_p, fpoo));
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

void QuImageGLWidget::setImage(const CuMatrix<double> &image) {
    d->imgb->setImage(image);
}

void QuImageGLWidget::setImage(const CuMatrix<unsigned short> &image) {
    d->imgb->setImage(image);
}

void QuImageGLWidget::setImage(const CuMatrix<unsigned char> &image) {
    d->imgb->setImage(image);
}

void QuImageGLWidget::setSource(const QString &src) {
    d->imgb->setSource(src);
}

void QuImageGLWidget::unsetSource() {
    d->imgb->unsetSource();
}

QImage &QuImageGLWidget::image() const {
    return d->imgb->image();
}

void QuImageGLWidget::setErrorImage(const QImage &i) {
    d->imgb->setErrorImage(i);
}

QImage QuImageGLWidget::errorImage() const {
    return d->imgb->errorImage();
}

bool QuImageGLWidget::error() const {
    return d->imgb->error();
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

void QuImageGLWidget::setZoomLevel(float n) {
    d->imgb->setZoomLevel(n);
}

void QuImageGLWidget::setZoomEnabled(bool en) {
    d->imgb->setZoomEnabled(en);
}

float QuImageGLWidget::zoomLevel() const {
    return d->imgb->zoomLevel();
}

void QuImageGLWidget::paintEvent(QPaintEvent *e)
{
    d->imgb->paint(e, this);
}

QPoint QuImageGLWidget::mapToImg(const QPoint& p) const {
   return d->imgb->mapToImg(p);
}

QRect QuImageGLWidget::mapToImg(const QRect& r) const{
    return d->imgb->mapToImg(r);
}

QPoint QuImageGLWidget::mapFromImg(const QPoint& p) const{
    return d->imgb->mapFromImg(p);
}

QRect QuImageGLWidget::mapFromImg(const QRect& r) const{
    return d->imgb->mapFromImg(r);
}

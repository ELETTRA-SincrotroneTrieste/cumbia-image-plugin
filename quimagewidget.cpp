#include "quimagewidget.h"
#include "colortablemap.h"
#include <QtDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QPainter>

/*
 * Class with properties specific to EImageWidget, as a QWidget.
 * Other properties are stored by EImageWidgetBasePrivate
 */
class EImageWidgetPrivate
{
public:
    QuImageBase *imgb;
    bool scaleWithZoom;
};

QuImageWidget::QuImageWidget(QWidget *parent) :
    QLabel(parent) {
    d = new EImageWidgetPrivate;
    d->imgb = new QuImageBase(this, false);
    d->scaleWithZoom = true;
}

QuImageWidget::~QuImageWidget()
{
    delete d->imgb;
    delete d;
}

bool QuImageWidget::scaleWithZoom() const
{
    return d->scaleWithZoom;
}

QImage QuImageWidget::errorImage() const
{
    return d->imgb->errorImage();
}

void QuImageWidget::setErrorMessage(const QString &msg)
{
    d->imgb->setErrorMessage(msg);
}

void QuImageWidget::setError(bool error)
{
    d->imgb->setError(error);
}

void QuImageWidget::setOk(bool ok)
{
    setError(!ok);
}

void QuImageWidget::setImage(const QImage& img)
{
    d->imgb->setImage(img);
    if(d->scaleWithZoom)
    {
        QImage& imgRef = d->imgb->image();
        setGeometry(x(), y(), imgRef.width() * (d->imgb->zoom() / 100.0), imgRef.height() * (d->imgb->zoom() / 100.0));
    }
    update();
}

void QuImageWidget::setErrorImage(const QImage &img)
{
    d->imgb->setErrorImage(img);
}

void QuImageWidget::mousePressEvent(QMouseEvent *ev)
{
    d->imgb->mousePress(ev);
}

void QuImageWidget::mouseMoveEvent(QMouseEvent *ev)
{
    d->imgb->mouseMove(ev);
}

void QuImageWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    d->imgb->mouseRelease(ev);
}

void QuImageWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    d->imgb->paint(e, this);
}

void QuImageWidget::wheelEvent(QWheelEvent *we)
{
    d->imgb->wheelEvent(we);
    we->accept();
    setImage(image());
}

QMenu *QuImageWidget::rightClickMenu()
{
    QMenu *menu = new QMenu(this);
    menu->addAction("Change Color Map...", this, SLOT(execConfigDialog()));
    return menu;
}

QImage &QuImageWidget::image() const {
    return d->imgb->image();
}

void QuImageWidget::setColorTable(const QVector<QRgb> &rgb){
    d->imgb->setColorTable(rgb);
}

QVector<QRgb> &QuImageWidget::colorTable() const{
    return d->imgb->colorTable();
}

void QuImageWidget::setImageMouseEventInterface(ImageMouseEventInterface *ifa){
    d->imgb->setImageMouseEventInterface(ifa);
}

bool QuImageWidget::isOpenGL() const{
    return false;
}

bool QuImageWidget::zoomEnabled() const{
    return d->imgb->zoomEnabled();
}

void QuImageWidget::setZoom(int n){
    d->imgb->setZoom(n);
}

void QuImageWidget::setZoomEnabled(bool en){
    d->imgb->setZoomEnabled(en);
}

float QuImageWidget::zoom() const {
    return d->imgb->zoom();
}

QSize QuImageWidget::minimumSizeHint() const
{
    const int min = 32;
    const QImage& i = image();
    if(!i.isNull() && i.width()/4.0 * i.height()/4.0 > min * min)
        return i.size();
    return QSize(min, min);
}

QSize QuImageWidget::sizeHint() const
{
    if(image().isNull())
        return QSize(256, 256);
    return image().size();
}

void QuImageWidget::execConfigDialog()
{
    d->imgb->execConfigDialog();
    update();
}

void QuImageWidget::setScaleWithZoom(bool scale)
{
    d->scaleWithZoom = scale;
    update();
}

QWidget *QuImageWidget::asWidget() const {
    return d->imgb->asWidget();
}



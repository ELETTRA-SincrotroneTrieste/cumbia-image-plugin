#include "quimagewidget.h"
#include "colortablemap.h"
#include <QtDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QPainter>
#include <QScrollArea>

#include <cudata.h>

/*
 * Class with properties specific to EImageWidget, as a QWidget.
 * Other properties are stored by EImageWidgetBasePrivate
 */
class QuImageWidgetPrivate
{
public:
    QuImageWidgetPrivate(QuImageBase *img) : imgb(img) { }

    bool scaleWithZoom;
    QuImageBase *imgb;
};

QuImageWidget::QuImageWidget(QWidget *parent, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo) :
    QWidget(parent)  {
    d = new QuImageWidgetPrivate(new QuImageBase(this, false, cu_p, fpoo));
    d->imgb->setImageBaseListener(this);
    d->scaleWithZoom  = true;
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

bool QuImageWidget::fitToWidget() const {
    return d->imgb->fitToWidget();
}

QImage QuImageWidget::errorImage() const {
    return d->imgb->errorImage();
}

void QuImageWidget::setErrorMessage(const QString &msg) {
    d->imgb->setErrorMessage(msg);
}

void QuImageWidget::setError(bool error) {
    d->imgb->setError(error);
}

void QuImageWidget::setOk(bool ok) {
    setError(!ok);
}

void QuImageWidget::setImage(const QImage& img) {
    d->imgb->setImage(img);
    if(d->scaleWithZoom) {
        QImage& imgRef = d->imgb->image();
        setGeometry(x(), y(), imgRef.width() * (d->imgb->zoomValue() / 100.0), imgRef.height() * (d->imgb->zoomValue() / 100.0));
    }
    update();
}

void QuImageWidget::setErrorImage(const QImage &img) {
    d->imgb->setErrorImage(img);
}

void QuImageWidget::mousePressEvent(QMouseEvent *ev) {
    d->imgb->mousePress(ev);
}

void QuImageWidget::mouseMoveEvent(QMouseEvent *ev) {
    d->imgb->mouseMove(ev);
}

void QuImageWidget::mouseReleaseEvent(QMouseEvent *ev) {
    d->imgb->mouseRelease(ev);
}

void QuImageWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    d->imgb->paint(e, this);
}

void QuImageWidget::wheelEvent(QWheelEvent *we)
{
    QWidget::wheelEvent(we);
}

void QuImageWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = new QMenu(this);
    menu->addAction("Change Color Map...", this, SLOT(execConfigDialog()));
    menu->exec(QCursor::pos());
}

void QuImageWidget::setImage(const CuMatrix<double> &image) {
    d->imgb->setImage(image);
}

void QuImageWidget::setImage(const CuMatrix<unsigned short> &image) {
    d->imgb->setImage(image);
}

void QuImageWidget::setImage(const CuMatrix<unsigned char> &image) {
    d->imgb->setImage(image);
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

float QuImageWidget::zoomValue() const {
    return d->imgb->zoomValue();
}

void QuImageWidget::setSource(const QString &src) {
    d->imgb->setSource(src);
}

void QuImageWidget::unsetSource() {
    d->imgb->unsetSource();
}

void QuImageWidget::onZoom(const QRect &oldRect, const QRect &newRect) {
    emit zoomRectChanged(oldRect, newRect);
}

QSize QuImageWidget::minimumSizeHint() const
{
    const int min = 32;
    const QImage& i = image();
    if(!d->imgb->fitToWidget() && !i.isNull() && i.width()/4.0 * i.height()/4.0 > min * min)
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

void QuImageWidget::setFitToWidget(bool fit) {
    d->imgb->setFitToWidget(fit);
    updateGeometry();
    qDebug() << __PRETTY_FUNCTION__ << fit << geometry() << "BEFORE adjustSize";
    adjustSize();
    qDebug() << __PRETTY_FUNCTION__ << fit << geometry() << "AFTER adjustSize";
}

QWidget *QuImageWidget::asWidget() const {
    return d->imgb->asWidget();
}

void QuImageWidget::onNewData(const CuData &da) {
    if(da["err"].toBool()) {

    } else {
        const CuVariant &v = da["value"];
        if(v.getFormat() == CuVariant::Matrix) {
            switch(v.getType()) {
            case CuVariant::Double:
                d->imgb->setImage(v.toMatrix<double>());
                break;
            case CuVariant::UChar:
                d->imgb->setImage(v.toMatrix<unsigned char>());
                break;
            case CuVariant::UShort:
                d->imgb->setImage(v.toMatrix<unsigned short>());
                break;
            case CuVariant::Int: {
                CuMatrix<int> mi = v.toMatrix<int>();
                const std::vector<int> iv =  mi.data();
                const std::vector<unsigned char> ucv(iv.begin(), iv.end());
                d->imgb->setImage(CuMatrix<unsigned char>(ucv, mi.nrows(), mi.ncols()));
            }  break;
            default:
                perr("QuImageWidget.onNewData: data type %d [%s] is not supported", v.getType(), v.dataTypeStr(v.getType()).c_str());
                break;
            }
        }
    }
}



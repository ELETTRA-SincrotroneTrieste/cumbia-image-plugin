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
    QuImageBase *imgb;
};

QuImageWidget::QuImageWidget(QWidget *parent, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo) :
    QWidget(parent)  {
    d = new QuImageWidgetPrivate(new QuImageBase(this, false, cu_p, fpoo));
    d->imgb->setImageBaseListener(this);
}

QuImageWidget::~QuImageWidget()
{
    delete d->imgb;
    delete d;
}

bool QuImageWidget::autoScale() const {
    return d->imgb->autoScale();
}

double QuImageWidget::upperBound() const {
    return d->imgb->upperBound();
}

double QuImageWidget::lowerBound() const {
    return d->imgb->lowerBound();
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

bool QuImageWidget::error() const {
    return d->imgb->error();
}

/*!
 * \brief clear the error flag
 * \param equivalent to setError( !ok )
 */
void QuImageWidget::setOk(bool ok) {
    setError(!ok);
}

/*!
 * \brief slot version of QuImageBaseI::setImage(const QImage& img)
 * \param img a QImage
 */
void QuImageWidget::setImage(const QImage& img) {
    d->imgb->setImage(img);
    if(!d->imgb->scaleContents()) {
        QImage& imgRef = d->imgb->image();
        qDebug() << __PRETTY_FUNCTION__ << "calling setGeometry";
        setGeometry(x(), y(), imgRef.width() * (d->imgb->zoomLevel() / 100.0), imgRef.height() * (d->imgb->zoomLevel() / 100.0));
    }
    update();
}

/*!
 * \brief slot version of QuImageBaseI::setErrorImage
 * \param img a QImage to be displayed in case of error
 */
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

/*!
 * \brief execute an image configuration menu
 */
void QuImageWidget::contextMenuEvent(QContextMenuEvent *)
{
    QMenu *menu = new QMenu(this);
    menu->addAction("Change Color Map...", this, SLOT(execConfigDialog()));
    menu->exec(QCursor::pos());
}

void QuImageWidget::resizeEvent(QResizeEvent *re) {
    d->imgb->setPainterDirty(true);
    QWidget::resizeEvent(re);
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

void QuImageWidget::setImageMouseEventInterface(QuImageMouseEventIf *ifa){
    d->imgb->setImageMouseEventInterface(ifa);
}

bool QuImageWidget::isOpenGL() const{
    return false;
}

bool QuImageWidget::mouseZoomEnabled() const{
    return d->imgb->mouseZoomEnabled();
}

void QuImageWidget::setZoomLevel(float n){
    d->imgb->setZoomLevel(n);
}

void QuImageWidget::setMouseZoomEnabled(bool en){
    d->imgb->setMouseZoomEnabled(en);
}

float QuImageWidget::zoomLevel() const {
    return d->imgb->zoomLevel();
}

QString QuImageWidget::source() const {
    return d->imgb->source();
}

void QuImageWidget::setSource(const QString &src) {
    d->imgb->setSource(src);
}

void QuImageWidget::setAutoScale(bool a) {
    d->imgb->setAutoScale( a );
    update();
}

void QuImageWidget::setLowerBound(double lb) {
    d->imgb->setLowerBound(lb);
}

void QuImageWidget::setUpperBound(double ub) {
    d->imgb->setUpperBound(ub);
}

void QuImageWidget::unsetSource() {
    d->imgb->unsetSource();
}

void QuImageWidget::onZoom(const QRect &from_z, const QRect &to_z) {
    emit zoomRectChanged(from_z, to_z);
}

void QuImageWidget::onBoundsChanged(double lower, double upper) {
    emit lowerBoundChanged(lower);
    emit upperBoundChanged(upper);
}

QSize QuImageWidget::minimumSizeHint() const
{
    const int min = 32;
    const QImage& i = image();
    if(!d->imgb->scaleContents() && !i.isNull() && i.width()/4.0 * i.height()/4.0 > min * min)
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

bool QuImageWidget::scaleContents() const {
    return  d->imgb->scaleContents();
}

void QuImageWidget::setScaleContents(bool fit) {
    d->imgb->setScaleContents(fit);
    updateGeometry();
    adjustSize();
}

QWidget *QuImageWidget::asWidget() const {
    return d->imgb->asWidget();
}

/*!
 * \brief slot executed to update the image with the new data
 * \param da CuData
 *
 * The supported types are those declared in the setImage methods of QuImageBaseI
 *
 * Supported data types:
 * \li double
 * \li unsigned char
 * \li unsigned short
 * \li int (converted to unsigned char)
 */
void QuImageWidget::onNewData(const CuData &da) {
    auto t1 = std::chrono::high_resolution_clock::now();
    bool err = da["err"].toBool();
    std::string msg = da["msg"].toString();
    const CuVariant &v = da["value"];
    if(!err) {
        if(da.has("qs", "invalid")) {
            err = true;
            msg = "QuImageWidget: data quality invalid" + (msg.length() > 0 ? " : " + msg : "");
        }
        else if(v.getFormat() == CuVariant::Matrix) {
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
                const std::vector<int> &iv =  mi.data();
                const std::vector<unsigned char> ucv(iv.begin(), iv.end());
                d->imgb->setImage(CuMatrix<unsigned char>(ucv, mi.nrows(), mi.ncols()));
            }  break;
            case CuVariant::LongInt: {
                CuMatrix<long int> mi = v.toMatrix<long int>();
                const std::vector<long int> &iv =  mi.data();
                const std::vector<unsigned char> ucv(iv.begin(), iv.end());
                d->imgb->setImage(CuMatrix<unsigned char>(ucv, mi.nrows(), mi.ncols()));
            }  break;
            case CuVariant::LongUInt: {
                CuMatrix<unsigned long int> mi = v.toMatrix<unsigned long int>();
                const std::vector<unsigned long int> &iv =  mi.data();
                const std::vector<unsigned char> ucv(iv.begin(), iv.end());
                d->imgb->setImage(CuMatrix<unsigned char>(ucv, mi.nrows(), mi.ncols()));
            }  break;
            case CuVariant::UInt: {
                CuMatrix<unsigned  int> mi = v.toMatrix<unsigned  int>();
                const std::vector<unsigned  int> &iv =  mi.data();
                const std::vector<unsigned char> ucv(iv.begin(), iv.end());
                d->imgb->setImage(CuMatrix<unsigned char>(ucv, mi.nrows(), mi.ncols()));
            }  break;
            case CuVariant::Float: {
                CuMatrix<float> mf = v.toMatrix<float>();
                const std::vector<float> iv =  mf.data();
                const std::vector<unsigned char> ucv(iv.begin(), iv.end());
                d->imgb->setImage(CuMatrix<unsigned char>(ucv, mf.nrows(), mf.ncols()));
                break;
            }
            case CuVariant::Short: {
                CuMatrix<short> ms = v.toMatrix<short>();
                const std::vector<short> sv =  ms.data();
                const std::vector<unsigned char> ucv(sv.begin(), sv.end());
                d->imgb->setImage(CuMatrix<unsigned char>(ucv, ms.nrows(), ms.ncols()));
                break;
            }
            default:
                err = true;
                msg = "QuImageWidget: data type " + v.dataTypeStr(v.getType()) + " unsupported";
                break;
            }
        }
        else {
            err = true;
            msg = "QuImageWidget: data format " + v.dataFormatStr(v.getFormat()) + " unsupported";
        }
    }
    d->imgb->setError(err);
    if(err) {
        d->imgb->setErrorMessage(msg.c_str());
        d->imgb->errorImage().isNull() ?
                    d->imgb->setImage(d->imgb->image().convertToFormat(QImage::Format_Grayscale8)) :
                    d->imgb->setImage(d->imgb->errorImage());
        d->imgb->asWidget()->updateGeometry();
    }
    d->imgb->asWidget()->setToolTip(msg.c_str());

    emit newData(da);
    auto t2 = std::chrono::high_resolution_clock::now();
    printf("QuImageWidget: updating image took %ldus\n", std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count());
}

QPoint QuImageWidget::mapToImg(const QPoint& p) const {
    return d->imgb->mapToImg(p);
}

QRect QuImageWidget::mapToImg(const QRect& r) const{
    return d->imgb->mapToImg(r);
}

QPoint QuImageWidget::mapFromImg(const QPoint& p) const{
    return d->imgb->mapFromImg(p);
}

QRect QuImageWidget::mapFromImg(const QRect& r) const{
    return d->imgb->mapFromImg(r);
}



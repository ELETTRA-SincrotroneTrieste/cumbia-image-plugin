#include "quimagebase.h"
#include <QPaintEvent>
#include <QOpenGLWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>
#include <cumacros.h>
#include <elettracolors.h>
#include "quimagemouseeventif.h"
#include <math.h>
#include <QSettings>
#include <QtDebug>
#include <quwatcher.h>
#include "quimgconfdialog.h"
#include "colortablemap.h"
#include "quimgpainter.h"

/*! \brief maps point p in widget coordinates to the corresponding pixel in the image
 *  \param p a point on the widget
 *  \return p mapped to image coords
 */
QPoint QuImageBasePrivate::mapToImg(const QPoint &p)const {
    const QSize &s = image.size()/* * (d->zoom/100.0)*/;
    const QSize& ws = widget->size();
    return QPoint(s.width() * p.x() / ws.width(), s.height() * p.y() / ws.height());
}

QRect QuImageBasePrivate::mapToImg(const QRect &r) const {
    return QRect(mapToImg(r.topLeft()), mapToImg(r.bottomRight()));
}

QPoint QuImageBasePrivate::mapFromImg(const QPoint &p) const {
    const QSize &s = image.size()/* * (d->zoom/100.0)*/;
    const QSize& ws = widget->size();
    return QPoint(ws.width() * p.x() / s.width(), ws.height() * p.y() / s.height());
}

QRect QuImageBasePrivate::mapFromImg(const QRect &r)const  {
    return QRect(mapFromImg(r.topLeft()), mapFromImg(r.bottomRight()));
}

QuImageBase::QuImageBase(QWidget *widget, bool isOpenGL, CumbiaPool *cu_p, const CuControlsFactoryPool &fpoo)
{
    d = new QuImageBasePrivate(cu_p, fpoo); // creates zoomer
    d->isOpenGL = isOpenGL;
    d->widget = widget;
    d->image = QImage();
    d->colorTable = ColorTableMap()["c"];
    d->errorImage.load(":/artwork/okguy.png");
    d->noise.load(":/artwork/bwnoise.png");
    d->listener = nullptr;
}

QuImageBase::~QuImageBase() {
    delete d; // deletes zoomer
}

QWidget *QuImageBase::asWidget() const {
    return d->widget;
}

QImage &QuImageBase::image() const {
    return d->image;
}

void QuImageBase::setImage(const QImage &img) {
    d->painter.dirty = true;
    d->image = img;
}

void QuImageBase::setImage(const CuMatrix<double> &data) {
    const std::vector<double> &v = data.data();
    std::vector<unsigned char> vuc(v.begin(), v.end());
    m_set_image(CuMatrix<unsigned char>(vuc, data.nrows(), data.ncols()));
}

void QuImageBase::setImage(const CuMatrix<unsigned short> &data) {
    const std::vector<unsigned short> &v = data.data();
    std::vector<unsigned char> vuc(v.begin(), v.end());
    m_set_image(CuMatrix<unsigned char>(vuc, data.nrows(), data.ncols()));
}

void QuImageBase::setImage(const CuMatrix<unsigned char> &data) {
    m_set_image(data);
}

void QuImageBase::m_set_image(const CuMatrix<unsigned char> &data)
{
    // after auto scale or bound are changed, raw data must be saved
    // so that image updates upon further bounds change can be shown
    // immediately
    if(d->raw_data.isValid()) {
        d->raw_data = data;
    }
    QSize olds(d->image.size());
    // use d->painter.dirty as changed flag
    d->painter.dirty = d->image.size() != QSize(data.ncols(), data.nrows());
    unsigned int factor = 1, lower = 0;
    if(d->autoscale) {
        double m = 0, M = 0.0;
        for(size_t i = 0; i < data.nrows(); i++)  {
            for(size_t j = 0; j < data.ncols(); j++) {
                const unsigned int& d = data[i][j];
                if(i == 0 && j == 0) {
                    M = m = d;
                }
                else {
                    if(M < d) M = d;
                    if(m > d) m = d;
                }
            }
        }
        lower = d->lower = m;
        d->upper = M;
        if(d->lower < d->upper)
            factor = 255.0/(M-m);
    }
    if(d->painter.dirty) {
        QVector<QRgb> &colorT = colorTable();
        d->image = QImage(data.ncols(), data.nrows(), QImage::Format_Indexed8);
        d->image.setColorTable(colorT);
        for(size_t i = 0; i < data.nrows(); i++)  {
            uchar *line = d->image.scanLine(i);
            for(size_t j = 0; j < data.ncols(); j++) {
                const uchar &v = (data[i][j] - lower) * factor;
                line[j] = v;
            }
        }
        /* newImage without colorTable. setImage will save current color table from current image
         * and then restore it
         */
        printf(" image size changed from %dx%d to %dx%d setting new image \n", olds.width(), olds.height(), d->image.width(), d->image.height());
        d->widget->update();
    }
    else  {/* no resize needed */
        int chcnt = 0;
        for(size_t i = 0; i < data.nrows(); i++) {
            uchar *line = d->image.scanLine(i);
            for(size_t j = 0; j < data.ncols(); j++)  {
                const unsigned char &v = (data[i][j] - lower) * factor;
                if(line[j] != v) {
                    chcnt++;
                    d->painter.dirty = true; // QuImgPainter::paint needs redraw image
                    line[j] = v;
                }
            }
        }
        printf(" %d/%d pixels changed in image %ldx%ld factor %d lower %d changed %s\n",
               chcnt, d->image.width() * d->image.height(), data.nrows(), data.ncols(), factor, lower, d->painter.dirty ? "YES" : "NO");
        /* img is a reference to the current image. No resize needed, so image has been reused.
         * setImage will find a non empty color table and won't save/restore it
         */
        if(d->painter.dirty)
            d->widget->update();
    }
}

void QuImageBase::m_save_raw_data() {
    if(!d->image.isNull() && !d->raw_data.isValid() && d->image.colorTable().size() > 0) {
        std::vector<unsigned char> vc;
        for(int i = 0; i < d->image.width(); i++)
            for(int j = 0; j < d->image.height(); j++)
                vc.push_back(d->image.pixelIndex(i, j));
        // CuMatrix built with vc, number of rows, number of columns
        d->raw_data = CuMatrix<unsigned char>(vc, d->image.height(), d->image.width());
    }
}

void QuImageBase::setColorTable(const  QVector<QRgb> &rgb)
{
    d->colorTable = rgb;
    d->image.setColorTable(d->colorTable);
}

bool QuImageBase::mouseZoomEnabled() const {
    return d->zoomer->mouseEnabled();
}

/*!
 * \brief enable zooming with the mouse
 * \param en true: enables zoom mouse event.
 *
 * \note zoom level is reset to 100
 */
void QuImageBase::setMouseZoomEnabled(bool en) {
    qDebug() << __PRETTY_FUNCTION__ << en;
    d->zoomer->setMouseEnabled(en);
    d->painter.dirty = true;
    d->widget->update();
}

/*!
 * \brief Returns the current zoom level
 * \return the current zoom level. 100: original size
 */
float QuImageBase::zoomLevel() const {
    return d->zoomer->level();
}

bool QuImageBase::autoScale() const {
    return d->autoscale;
}

double QuImageBase::lowerBound() const {
    return d->lower;
}

double QuImageBase::upperBound() const {
    return d->upper;
}

/*!
 * \brief set manually a zoom level
 * \param f the level of zoom: 100 (percentage): original image size
 *
 * \note Disables mouse zoom
 */
void QuImageBase::setZoomLevel(float f) {
    d->zoomer->setLevel(f);
    d->painter.dirty = true;
    d->widget->update();
}

bool QuImageBase::isOpenGL() const { return d->isOpenGL; }

void QuImageBase::setErrorImage(const QImage &i) {
    d->errorImage = i;
}

QImage QuImageBase::errorImage() const {
    return d->errorImage;
}

QVector<QRgb> &QuImageBase::colorTable() const {
    return d->colorTable;
}

void QuImageBase::mouseMove(QMouseEvent *ev) {
    d->mouse_t->mouseMove(ev);
    d->widget->update();
}

void QuImageBase::mousePress(QMouseEvent *ev)
{
    d->mouse_t->mousePress(ev);
    d->widget->update();
}

void QuImageBase::mouseRelease(QMouseEvent *ev)
{
    d->mouse_t->mouseRelease(ev);
    d->widget->update();
}

void QuImageBase::wheelEvent(QWheelEvent *event) {
    d->mouse_t->wheelEvent(event);
    d->widget->update();
}

void QuImageBase::paint(QPaintEvent *e, QWidget *) {
    d->painter.paint(e, d);
}

void QuImageBase::setImageMouseEventInterface(QuImageMouseEventIf* ifa) {
    d->mouse_t->setMouseEventInterface(ifa);
}

void QuImageBase::setAutoScale(bool as) {
    m_save_raw_data();
    d->autoscale = as;
    m_set_image(d->raw_data);
    d->widget->update();
}

void QuImageBase::setLowerBound(double lb) {
    m_save_raw_data();
    d->autoscale = false;
    if(d->lower != lb) {
        d->lower = lb;
        m_set_image(d->raw_data);
        d->widget->update();
    }
}

void QuImageBase::setUpperBound(double ub) {
    m_save_raw_data();
    d->autoscale = false;
    if(d->upper != ub) {
        d->upper = ub;
        m_set_image(d->raw_data);
        d->widget->update();
    }
}

void QuImageBase::setBounds(double l, double u) {
    m_save_raw_data();
    d->autoscale = false;
    if(l != d->lower || u != d->upper) {
        d->upper = u;
        d->lower = l;
        m_set_image(d->raw_data);
        d->widget->update();
    }
}

bool QuImageBase::error() const {
    return d->error;
}

void QuImageBase::setError(bool error) {
    d->error = error ;
}

void QuImageBase::setErrorMessage(const QString &msg) { d->errorMessage = msg; }

QString QuImageBase::source() const {
    QuWatcher *w = d->widget->findChild<QuWatcher *>();
    return w != nullptr ? w->source() : QString();
}

void QuImageBase::setSource(const QString &src) {
    if(d->cu_pool) {
        QuWatcher *w = d->widget->findChild<QuWatcher *>();
        if(!w) {
            w = new QuWatcher(d->widget, d->cu_pool, d->fpool);
            QObject::connect(w, SIGNAL(newData(CuData)), d->widget, SLOT(onNewData(CuData)));
        }
        w->setSource(src);
    }
    else
        perr("QuImageBase.setSource: cannot set source (%s) if QuImagePluginInterface::init() has not been called", qstoc(src));
}

void QuImageBase::unsetSource() {
    QuWatcher *w = d->widget->findChild<QuWatcher *>();
    if(w) {
        w->unsetSource();
        w->deleteLater();
    }
}

/*! \brief maps point p in widget coordinates to the corresponding pixel in the image
 *  \param p a point on the widget
 *  \return p mapped to image coords
 */
QPoint QuImageBase::mapToImg(const QPoint &p)const {
    return d->mapToImg(p);
}

QRect QuImageBase::mapToImg(const QRect &r) const {
    return d->mapToImg(r);
}

QPoint QuImageBase::mapFromImg(const QPoint &p) const {
    return d->mapFromImg(p);
}

QRect QuImageBase::mapFromImg(const QRect &r)const  {
    return d->mapFromImg(r);
}

void QuImageBase::setImageBaseListener(QuImageBaseListener *li) {
    d->listener = li;
}

void QuImageBase::setPainterDirty(bool di) {
    d->painter.dirty = di;
}

void QuImageBase::setScaleContents(bool fit) {
    d->scale_contents = fit;
    d->zoomer->setMouseEnabled(!fit);
    d->painter.dirty = true;
    d->widget->update();
}

bool QuImageBase::scaleContents() const {
    return d->scale_contents;
}

void QuImageBase::execConfigDialog() {
    QuImgConfDialog co(this, d->widget);
    co.exec();
}

void QuImageBase::onApply(const QMap<QString, QVariant> &conf) {
    ColorTableMap colorMap;
    d->image.setColorTable(colorMap[conf["color_table"].toString()]);
    d->widget->setProperty("fitToWidget", conf["fit_to_widget"].toBool());
}

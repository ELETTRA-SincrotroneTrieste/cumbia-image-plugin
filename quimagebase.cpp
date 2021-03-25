#include "quimagebase.h"
#include <QPaintEvent>
#include <QGLWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>
#include <cumacros.h>
#include <elettracolors.h>
#include "eimagemouseeventinterface.h"
#include <math.h>
#include <QSettings>
#include <QtDebug>
#include <quwatcher.h>
#include "quimgconfdialog.h"
#include "colortablemap.h"

QuImageBase::QuImageBase(QWidget *widget, bool isOpenGL, CumbiaPool *cu_p, const CuControlsFactoryPool &fpoo)
{
    d = new QuImageBasePrivate(cu_p, fpoo);
    d->widget = widget;
    d->isOpenGL = isOpenGL;
    d->error = false;
    d->fit_to_w = true;
    d->mouseEventIf = NULL;
    d->zoom = 100;
    d->zoomEnabled = true;
    d->image = QImage();
    d->colorTable = ColorTableMap()["c"];
    d->errorImage.load(":/artwork/okguy.png");
    d->noise.load(":/artwork/bwnoise.png");
    d->listener = nullptr;
}

QuImageBase::~QuImageBase()
{
    delete d;
}

QWidget *QuImageBase::asWidget() const {
    return d->widget;
}

QImage &QuImageBase::image() const {
    return d->image;
}

void QuImageBase::setImage(const QImage &img) {
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
    QImage& img = image();
    bool changed = img.size() != QSize(data.nrows(), data.ncols());
//    qDebug() << __PRETTY_FUNCTION__ << image() << "size " << img.size() << " data " << data.nrows() << "x" << data.ncols();
    if(changed)
    {
        QVector<QRgb> &colorT = colorTable();
        QImage newImage = QImage(data.nrows(), data.ncols(), QImage::Format_Indexed8);
        newImage.setColorTable(colorT);
        for(size_t i = 0; i < data.nrows(); i++)  {
            for(size_t j = 0; j < data.ncols(); j++)
                newImage.setPixel(i, j, data[i][j]);
        }
        /* newImage without colorTable. setImage will save current color table from current image
         * and then restore it
         */
        setImage(newImage);
        d->widget->updateGeometry();
    }
    else  {/* no resize needed */
        for(size_t i = 0; i < data.nrows(); i++) {
            for(size_t j = 0; j < data.ncols(); j++)  {
                if(img.pixel(i, j) != data[i][j]) {
                    changed = true;
                    img.setPixel(i, j, data[i][j]);
                }
            }
        }
        /* img is a reference to the current image. No resize needed, so image has been reused.
         * setImage will find a non empty color table and won't save/restore it
         */
        setImage(img);
        if(changed)
            d->widget->update();
    }
}

void QuImageBase::setColorTable(const  QVector<QRgb> &rgb)
{
    d->colorTable = rgb;
    d->image.setColorTable(d->colorTable);
}

void QuImageBase::setZoomEnabled(bool en) {
    d->zoomEnabled = en;
}

bool QuImageBase::zoomEnabled() const {
    return d->zoomEnabled;
}

float QuImageBase::zoom() const {
    return d->zoom;
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
    if(!d->mP1.isNull())
        d->mP2= ev->pos();
    if(d->mouseEventIf)
        d->mouseEventIf->imageMouseMoveEvent(ev->pos(), ev->button());
    const QPoint &p = mapToImg(ev->pos());
    printf("QuImageBase.mouseMove: \e[1;36m(%d,%d) --> (%d,%d)\e[0m\n", ev->pos().x(), ev->pos().y(), p.x(), p.y());
    d->widget->update();
}

void QuImageBase::mousePress(QMouseEvent *ev)
{
    d->mP1 = ev->pos();
    d->leftButton = (ev->button() == Qt::LeftButton);
    if(d->mouseEventIf)
        d->mouseEventIf->imageMousePressEvent(ev->pos(), ev->button());
    d->widget->update();
}

void QuImageBase::mouseRelease(QMouseEvent *ev)
{
    /* call the release method only if the button was pressed inside the label
     */
    QRect r(QPoint(0,0), d->widget->geometry().size());
    if(d->mouseEventIf && !d->mP1.isNull() && r.contains(ev->pos()))
        d->mouseEventIf->imageMouseReleaseEvent(ev->pos(), ev->button());

    d->mP1 = QPoint();
    d->mP2 = QPoint();
    d->widget->update();
}


QRect QuImageBase::m_calc_zoom_rect(const QPoint &pos, double factor) {
    const QRect& g0 = d->widget->geometry();
    double dx = (pos.x() - g0.width()/2.0)/(double) g0.width();
    double dy = (pos.y() -g0.height()/2.0) / (double) g0.height();
    dx *= factor;
    dy *= factor;
    QRect g;
    g.setSize(g0.size() * factor);
    g.moveTop(-g0.height() * dy);
    g.moveLeft(-g0.width() * dx);
    qDebug() << __PRETTY_FUNCTION__ << "moving top" << -dy << " moving left " << -dx << "rect " << g0 << "-->" << g << "pos" << pos << "zoom" << d->zoom;
    return g;
}

void QuImageBase::wheelEvent(QWheelEvent *event) {
    int delta = event->delta();
    const double z0 = d->zoom;
    delta > 0 ? d->zoom = d->zoom * delta/100.0 : d->zoom = d->zoom / (-delta/100.0);
    const QRect& r = m_calc_zoom_rect(event->pos(), d->zoom/z0);
    d->widget->updateGeometry();
    if(d->listener) d->listener->onZoom(r);
}

void QuImageBase::setImageMouseEventInterface(ImageMouseEventInterface* ifa) {
    d->mouseEventIf = ifa;
}

void QuImageBase::setError(bool error) { d->error = error ; }

void QuImageBase::setErrorMessage(const QString &msg) { d->errorMessage = msg; }

void QuImageBase::paint(QPaintEvent *e, QWidget *paintDevice)
{
    if(d->error)
        d->image = d->errorImage;
    else if(d->image.isNull())
        d->image = d->noise;

    QPainter p(paintDevice);
    QRect imgRect = d->image.rect();
    imgRect.setSize(d->image.size() * (d->zoom/100.0));
    if(d->fit_to_w) imgRect.setSize(e->rect().size());
    else imgRect.setSize(d->image.size() * (d->zoom/100.0));
//    qDebug() << __FUNCTION__ << "zoom: " << d->zoom << "imgRect" << imgRect << " paint device geom " << paintDevice->geometry() <<
//                " paint rect " << e->rect();
    p.drawImage(imgRect, d->image, d->image.rect());


    //
   // p.drawImage(0, 0, d->image);
    if(!d->mP1.isNull() && !d->mP2.isNull())
    {
        QPen pen = p.pen();
        pen.setWidthF(2.0);
        QRect r(QPoint(0,0), d->widget->geometry().size());
        if(!r.contains(d->mP2))
            pen.setColor(Qt::red);
        else
            pen.setColor(Qt::green);

        p.setPen(pen);

        int p1x = d->mP1.x() * 100 / d->zoom;
        int p1y = d->mP1.y() * 100 / d->zoom;
        int p2x = d->mP2.x() * 100 / d->zoom;
        int p2y = d->mP2.y() * 100 / d->zoom;

        QFont f = p.font();
        f.setBold(true);
        p.setFont(f);
        QFontMetrics fm(p.font());

        QColor bg = Qt::white;
        bg.setAlpha(184);

        if(!d->leftButton)
        {
            float fl = sqrt(pow(p2x - p1x, 2) + pow(p2y - p1y, 2));
            QString val = QString("%1").arg(fl, 0, 'f', 2);
            p.fillRect(QRectF(d->mP1.x(), d->mP1.y(), fm.width(val), fm.height()), bg);
            /* first draw line */
            p.drawLine(d->mP1, d->mP2);
            /* then text */
            pen.setColor(Qt::black);
            p.setPen(pen);
            p.drawText(QPointF(d->mP1.x(), d->mP1.y() + fm.height()), val);
        }
        else
        {
            p.drawRect(QRect(d->mP1, d->mP2));
            p.setPen(bg);
            p.fillRect(QRect(d->mP1, d->mP2), bg);
            /* then text */
            QString val = QString("%1,%2 %3x%4").arg(p1x).arg(p1y).arg(fabs(p2x - p1x)).
                    arg(fabs(p2y - p1y));
            p.fillRect(QRectF(d->mP1.x(), d->mP1.y(), fm.width(val), fm.height()), bg);
            pen.setColor(Qt::black);
            p.setPen(pen);
            p.drawText(QPointF(d->mP1.x(), d->mP1.y() + fm.height()), val);
        }
    }
    if(d->error)
    {
        QPen errPen(KDARKRED);
        errPen.setWidth(5);
        p.setPen(errPen);
        /* draw a cross on the figure */
        if(d->errorImage.isNull())
        {
            p.drawLine(e->rect().topLeft(), e->rect().bottomRight());
            p.drawLine(e->rect().bottomLeft(), e->rect().topRight());
        }
        else /* yeah! draw ok guy! */
        {
            if(d->errorImage.size() != e->rect().size())
                d->errorImage = d->errorImage.scaled(e->rect().size(), Qt::KeepAspectRatio);
            p.drawImage(e->rect(), d->errorImage);
        }

        QRect txtRect = e->rect();
        QColor bgColor = Qt::white;
        bgColor.setAlpha(180);
        p.fillRect(txtRect, bgColor);
        /* draw the error message */
        QFont f = p.font();
        f.setPointSize(10);
        f.setBold(true);
        p.setFont(f);
        qDebug() << __FUNCTION__ << "drawing error message";
        p.drawText(txtRect, d->errorMessage);
    }
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

void QuImageBase::setFitToWidget(bool fit) {
    d->fit_to_w = fit;
}

bool QuImageBase::fitToWidget() const {
    return d->fit_to_w;
}

/*! \brief maps point p in widget coordinates to the corresponding pixel in the image
 *  \param p a point on the widget
 *  \return p mapped to image coords
 */
QPoint QuImageBase::mapToImg(const QPoint &p) {
    const QSize &s = d->image.size()/* * (d->zoom/100.0)*/;
    const QSize& ws = d->widget->size();
    return QPoint(s.width() * p.x() / ws.width(), s.height() * p.y() / ws.height());
}

void QuImageBase::setImageBaseListener(QuImageBaseListener *li) {
    d->listener = li;
}

void QuImageBase::execConfigDialog()
{
    QuImgConfDialog co(this, d->widget);
    co.exec();
}

void QuImageBase::onApply(const QMap<QString, QVariant> &conf) {
    ColorTableMap colorMap;
    d->image.setColorTable(colorMap[conf["color_table"].toString()]);
    d->widget->setProperty("scaleWithZoom", conf["scale_with_zoom"].toBool());
    d->widget->setProperty("fitToWidget", conf["fit_to_widget"].toBool());
    qDebug() << __PRETTY_FUNCTION__ << "scale with zoom property on widget " << d->widget->property("scaleWithZoom").toBool();
}



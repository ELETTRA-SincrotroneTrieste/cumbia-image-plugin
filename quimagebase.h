#ifndef QUIMAGEBASE_H
#define QUIMAGEBASE_H

#include <QColor>
#include <QPoint>
#include <QImage>
#include <quimageplugininterface.h>
#include <quimgconfdialog.h>
#include <quimgpainter.h>
#include <quimgzoomer.h>
#include <quimgmousetracker.h>

class ImageMouseEventInterface;
class ExternalScaleWidget;
class QPaintEvent;
class QPaintDevice;
class QWidget;
class QMouseEvent;
class QWheelEvent;

class QuImageBaseListener {
public:
    virtual void onZoom(const QRect& from_zr, const QRect& to_zr) = 0;
};

class QuImageBasePrivate
{
public:

    QuImageBasePrivate(CumbiaPool *cu_p, const CuControlsFactoryPool &fpoo)
        : isOpenGL(false),
          error(false),
          scale_contents(true),
          cu_pool(cu_p),
          fpool(fpoo),
          zoomer(new QuImgZoomer(!scale_contents, this)),
          mouse_t(new QuImgMouseTracker(this, zoomer)) {

    }

    ~QuImageBasePrivate() {
        delete zoomer;
        delete mouse_t;
    }

    QImage image, cached_img, errorImage, noise;
    bool isOpenGL;
    bool error;
    bool scale_contents; // fit to widget
    QString errorMessage;
    QWidget *widget;
    QVector<QRgb> colorTable;

    CumbiaPool *cu_pool;
    const CuControlsFactoryPool &fpool;
    QuImageBaseListener *listener;
    // painter
    QuImgPainter painter;
    // zoomer
    QuImgZoomer *zoomer;
    // mouse events
    QuImgMouseTracker *mouse_t;

    QPoint mapToImg(const QPoint &p) const;
    QRect mapToImg(const QRect &r) const;
    QPoint mapFromImg(const QPoint &p) const;
    QRect mapFromImg(const QRect &r) const;
};

class QuImageBase : public QuImageBaseI, public QuImgConfDialogListener
{
public:
    QuImageBase(QWidget *widget, bool isOpenGL, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo);
    virtual ~QuImageBase();
    QWidget *asWidget() const;

    QImage& image() const;
    void setImage(const QImage& img);

    virtual void setImage(const CuMatrix<double> &data);
    virtual void setImage(const CuMatrix<unsigned short> &data);
    virtual void setImage(const CuMatrix<unsigned char> &data);

    void setImageMouseEventInterface(ImageMouseEventInterface* ifa);

    void setScaleContents(bool fit);
    bool scaleContents() const;

    bool error() const;
    void setError(bool error);
    void setErrorMessage(const QString& msg);
    void setErrorImage(const QImage& i);
    QImage errorImage() const;

    void setColorTable(const QVector<QRgb> &rgb);
    QVector<QRgb>& colorTable() const;

    void setZoomLevel(float f);
    void setMouseZoomEnabled(bool en);
    bool mouseZoomEnabled() const;
    float zoomLevel() const;

    bool isOpenGL() const;

    void setImageMouseEventInterface(QuImageMouseEventIf *ifa);
    void mouseMove(QMouseEvent *e);
    void mousePress(QMouseEvent *e);
    void mouseRelease(QMouseEvent *e);
    void wheelEvent(QWheelEvent * event);

    void paint(QPaintEvent *e, QWidget *widget);

    void execConfigDialog();

    QString source() const;
    void setSource(const QString &src);
    void unsetSource();

    QPoint mapToImg(const QPoint& p) const;
    QRect mapToImg(const QRect& r)const;
    QPoint mapFromImg(const QPoint &p)const;
    QRect mapFromImg(const QRect &p)const;

    void setImageBaseListener(QuImageBaseListener *li);

    void setPainterDirty(bool di);

    // QuImgConfDialogListener interface
public:
    void onApply(const QMap<QString, QVariant> &conf);
private:
    void m_set_image(const CuMatrix<unsigned char> &data);
    QuImageBasePrivate *d;
};

#endif // QUIMAGEBASE_H

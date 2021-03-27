#ifndef QUIMAGEBASE_H
#define QUIMAGEBASE_H

#include <QColor>
#include <QPoint>
#include <QImage>
#include <quimageplugininterface.h>
#include <quimgconfdialog.h>
#include <quimgpainter.h>
#include <quimgzoomer.h>

class ImageMouseEventInterface;
class ExternalScaleWidget;
class QPaintEvent;
class QPaintDevice;
class QWidget;
class QMouseEvent;
class QWheelEvent;

class QuImageBaseListener {
public:
    virtual void onZoom(const QRect& zoom_r) = 0;
};

class QuImageBasePrivate
{
public:

    QuImageBasePrivate(CumbiaPool *cu_p, const CuControlsFactoryPool &fpoo) : cu_pool(cu_p), fpool(fpoo) { }

    QImage image, errorImage, noise;
    int isOpenGL;
    float zoom;
    bool error;
    bool fit_to_w; // fit to widget
    QString errorMessage;
    ImageMouseEventInterface *mouseEventIf;
    bool leftButton, zoomEnabled;
    QPoint mP1, mP2;
    QWidget *widget;
    QVector<QRgb> colorTable;

    CumbiaPool *cu_pool;
    const CuControlsFactoryPool &fpool;
    QuImageBaseListener *listener;
    QList<QRect> zoom_stack;

    // painter
    QuImgPainter painter;
    // zoomer
    QuImgZoomer zoomer;
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

    bool error() const;
    void setError(bool error);
    void setErrorMessage(const QString& msg);
    void setErrorImage(const QImage& i);
    QImage errorImage() const;

    void setColorTable(const QVector<QRgb> &rgb);
    QVector<QRgb>& colorTable() const;

    void setZoom(int n) { d->zoom = n; }
    void setZoomEnabled(bool en);
    bool zoomEnabled() const;
    float zoomLevel() const;
    void unzoom(const QPoint &pos);
    void zoom(const QRect& r);

    bool isOpenGL() const;
    void mouseMove(QMouseEvent *e);
    void mousePress(QMouseEvent *e);
    void mouseRelease(QMouseEvent *e);
    void wheelEvent(QWheelEvent * event);

    void paint(QPaintEvent *e, QWidget *widget);

    void execConfigDialog();

    void setSource(const QString &src);
    void unsetSource();

    void setFitToWidget(bool fit);
    bool fitToWidget() const;

    QPoint mapToImg(const QPoint& p) const;
    QRect mapToImg(const QRect& r)const;
    QPoint mapFromImg(const QPoint &p)const;
    QRect mapFromImg(const QRect &p)const;
    QList<QRect> zoomStack() const;

    void setImageBaseListener(QuImageBaseListener *li);

    // QuImgConfDialogListener interface
public:
    void onApply(const QMap<QString, QVariant> &conf);

private:

    void m_set_image(const CuMatrix<unsigned char> &data);
    void m_zoom(const QRect& selectionRect);

    QuImageBasePrivate *d;

};

#endif // QUIMAGEBASE_H

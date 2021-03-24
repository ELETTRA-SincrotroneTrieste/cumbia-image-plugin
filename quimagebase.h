#ifndef QUIMAGEBASE_H
#define QUIMAGEBASE_H

#include <QColor>
#include <QPoint>
#include <QImage>
#include <quimageplugininterface.h>

class ImageMouseEventInterface;
class ExternalScaleWidget;
class QPaintEvent;
class QPaintDevice;
class QWidget;
class QMouseEvent;
class QWheelEvent;

class QuImageBasePrivate
{
public:

    QuImageBasePrivate(CumbiaPool *cu_p, const CuControlsFactoryPool &fpoo) : cu_pool(cu_p), fpool(fpoo) { }

    QImage image, errorImage, noise;
    int isOpenGL;
    float zoom;
    bool error;
    QString errorMessage;
    ImageMouseEventInterface *mouseEventIf;
    bool leftButton, zoomEnabled;
    QPoint mP1, mP2;
    QWidget *widget;
    QVector<QRgb> colorTable;

    CumbiaPool *cu_pool;
    const CuControlsFactoryPool &fpool;
};

class QuImageBase : public QuImageBaseI
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

    void setError(bool error);
    void setErrorMessage(const QString& msg);
    void setErrorImage(const QImage& i);
    QImage errorImage() const;

    void setColorTable(const QVector<QRgb> &rgb);
    QVector<QRgb>& colorTable() const;

    void setZoom(int n) { d->zoom = n; }
    void setZoomEnabled(bool en);
    bool zoomEnabled() const;
    float zoom() const;

    bool isOpenGL() const;
    void mouseMove(QMouseEvent *e);
    void mousePress(QMouseEvent *e);
    void mouseRelease(QMouseEvent *e);
    void wheelEvent(QWheelEvent * event);

    void paint(QPaintEvent *e, QWidget *paintDevice);

    void execConfigDialog();

    void setSource(const QString &src);
    void unsetSource();

private:

    void m_set_image(const CuMatrix<unsigned char> &data);

    QuImageBasePrivate *d;
};

#endif // QUIMAGEBASE_H

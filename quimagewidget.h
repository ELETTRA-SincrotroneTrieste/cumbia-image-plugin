#ifndef QUIMAGEWIDGET_H
#define QUIMAGEWIDGET_H

#include <QLabel>
#include <QRgb>
#include <QVector>
#include <quimagebase.h>
#include <quimageplugininterface.h>

class QImage;
class ExternalScaleWidget;
class ImageMouseEventInterface;
class QMenu;
class QuImageWidgetPrivate;
class CuData;

class QuImageWidget : public QLabel, public QuImageBaseI
{
    Q_OBJECT

    Q_PROPERTY(bool scaleWithZoom READ scaleWithZoom WRITE setScaleWithZoom DESIGNABLE true)
    Q_PROPERTY(QImage errorImage READ errorImage WRITE setErrorImage  DESIGNABLE true)

public:
    explicit QuImageWidget(QWidget *parent, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo);
    virtual ~QuImageWidget();

    bool scaleWithZoom() const;
    QImage errorImage() const;

signals:
    
public slots:

    void setImage(const QImage& image);
    void setErrorImage(const QImage& img);
    void setErrorMessage(const QString &msg);
    void setError(bool error);

    void setOk(bool ok);
    void execConfigDialog();
    void setScaleWithZoom(bool scale);

    QWidget *asWidget() const;

    void onNewData(const CuData& da);

protected:

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *we);

    virtual QMenu* rightClickMenu();

private:
    QuImageWidgetPrivate *d;

    // QuImageBaseI interface
public:
    void setImage(const CuMatrix<double> &image);
    void setImage(const CuMatrix<unsigned short> &image);
    void setImage(const CuMatrix<unsigned char> &image);
    QImage &image() const;
    void setColorTable(const QVector<QRgb> &rgb);
    QVector<QRgb> &colorTable() const;
    void setImageMouseEventInterface(ImageMouseEventInterface *ifa);
    bool isOpenGL() const;
    bool zoomEnabled() const;
    void setZoom(int n);
    void setZoomEnabled(bool en);
    float zoom() const;

    void setSource(const QString& src);
    void unsetSource();

public:
};

#endif // IMAGE_H

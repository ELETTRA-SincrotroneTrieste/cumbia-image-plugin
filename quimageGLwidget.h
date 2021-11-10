#ifndef QUIMAGEGLWIDGET_H
#define QUIMAGEGLWIDGET_H

#include "quimagebase.h"

#include <QOpenGLWidget>

class QuImageGLWidgetPrivate;

class QuImageGLWidget : public QOpenGLWidget, public QuImageBaseI {
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource DESIGNABLE true)
    Q_PROPERTY(bool scaleContents READ scaleContents WRITE setScaleContents DESIGNABLE true)
    Q_PROPERTY(bool mouseZoomEnabled READ mouseZoomEnabled WRITE setMouseZoomEnabled DESIGNABLE true)
    Q_PROPERTY(QImage errorImage READ errorImage WRITE setErrorImage  DESIGNABLE true)
    Q_PROPERTY(bool autoScale READ autoScale WRITE setAutoScale DESIGNABLE true)
    Q_PROPERTY(double  lowerBound READ lowerBound WRITE setLowerBound DESIGNABLE true)
    Q_PROPERTY(double  upperBound READ upperBound WRITE setUpperBound DESIGNABLE true)

public:
    explicit QuImageGLWidget(QWidget *parent, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo);
    virtual ~QuImageGLWidget();
    void setImage(const QImage& image);

    bool autoScale() const;
    double upperBound() const;
    double lowerBound() const;
    bool scaleContents() const;
    void setScaleContents(bool scale);

signals:

public slots:
    void setErrorImage(const QImage &i);
    void setErrorMessage(const QString &msg);
    void setZoomLevel(float n);
    void setMouseZoomEnabled(bool en);

    void setAutoScale(bool a);
    void setLowerBound(double lb);
    void setUpperBound(double ub);

    void setSource(const QString &src);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    QuImageGLWidgetPrivate *d;

    // QuImageBaseI interface
public:
    QImage &image() const;
    QImage errorImage() const;
    bool error() const;
    void setError(bool error);
    void setColorTable(const QVector<QRgb> &rgb);
    QVector<QRgb> &colorTable() const;
    void setImageMouseEventInterface(QuImageMouseEventIf *ifa);
    bool isOpenGL() const;
    bool mouseZoomEnabled() const;
    float zoomLevel() const;
    QWidget *asWidget() const;

    // QuImageBaseI interface
public:
    void setImage(const CuMatrix<double> &image);
    void setImage(const CuMatrix<unsigned short> &image);
    void setImage(const CuMatrix<unsigned char> &image);

    QString source() const;
    void unsetSource();


    // image -> widget geometry mappings
    virtual QPoint mapToImg(const QPoint& p) const;
    virtual QRect mapToImg(const QRect& r) const;
    virtual QPoint mapFromImg(const QPoint& p) const;
    virtual QRect mapFromImg(const QRect& r) const;
};

#endif // IMAGEGL_H

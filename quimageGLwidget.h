#ifndef QUIMAGEGLWIDGET_H
#define QUIMAGEGLWIDGET_H

#include "quimagebase.h"

#include <QOpenGLWidget>

class QuImageGLWidgetPrivate;

class QuImageGLWidget : public QOpenGLWidget, public QuImageBaseI {
    Q_OBJECT

public:
    explicit QuImageGLWidget(QWidget *parent = 0);
    virtual ~QuImageGLWidget();
    void setImage(const QImage& image);

signals:

public slots:

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
    void setErrorImage(const QImage &i);
    QImage errorImage() const;
    void setError(bool error);
    void setErrorMessage(const QString &msg);
    void setColorTable(const QVector<QRgb> &rgb);
    QVector<QRgb> &colorTable() const;
    void setImageMouseEventInterface(ImageMouseEventInterface *ifa);
    bool isOpenGL() const;
    bool zoomEnabled() const;
    void setZoom(int n);
    void setZoomEnabled(bool en);
    float zoom() const;
    QWidget *asWidget() const;
};

#endif // IMAGEGL_H

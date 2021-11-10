#ifndef QUIMAGEWIDGET_H
#define QUIMAGEWIDGET_H

#include <QWidget>
#include <QRgb>
#include <QVector>
#include <quimagebase.h>
#include <quimageplugininterface.h>

class QImage;
class ExternalScaleWidget;
class QuImageMouseEventIf;
class QMenu;
class QuImageWidgetPrivate;
class CuData;

/*! \brief QWidget implementing QuImageBaseI
 *
 *  This object is controlled through QuImageBaseI interface only.
 *  QuImageBaseI::asWidget returns this *as QWidget* so that properties, signals and slots can be used
 *  if more convenient.
 *
 */
class QuImageWidget : public QWidget, public QuImageBaseI, public QuImageBaseListener
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource DESIGNABLE true)
    Q_PROPERTY(bool scaleContents READ scaleContents WRITE setScaleContents DESIGNABLE true)
    Q_PROPERTY(bool mouseZoomEnabled READ mouseZoomEnabled WRITE setMouseZoomEnabled DESIGNABLE true)
    Q_PROPERTY(QImage errorImage READ errorImage WRITE setErrorImage  DESIGNABLE true)
    Q_PROPERTY(bool autoScale READ autoScale WRITE setAutoScale DESIGNABLE true)
    Q_PROPERTY(double  lowerBound READ lowerBound WRITE setLowerBound DESIGNABLE true)
    Q_PROPERTY(double  upperBound READ upperBound WRITE setUpperBound DESIGNABLE true)

public:
    explicit QuImageWidget(QWidget *parent, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo);
    virtual ~QuImageWidget();

    bool autoScale() const;
    double upperBound() const;
    double lowerBound() const;
    bool scaleContents() const;
    QImage errorImage() const;

signals:
    /*!
     * \brief zoomRectChanged is emitted when the zoom rect changes from from_z to to_z, in image coordinates
     * \param from_z previous zoom rect, in image coordinates
     * \param to_z current zoom rect, in image cooridinates
     */
    void zoomRectChanged(const QRect& from_z, const QRect& to_z);
    void newData(const CuData& data);
    void upperBoundChanged(double ub);
    void lowerBoundChanged(double lb);

public slots:

    void setSource(const QString& src);

    void setAutoScale(bool a);
    void setLowerBound(double lb);
    void setUpperBound(double ub);

    void setImage(const QImage& image);
    void setErrorImage(const QImage& img);
    void setErrorMessage(const QString &msg);
    void setError(bool error);
    bool error() const;

    void setOk(bool ok);
    void execConfigDialog();
    void setScaleContents(bool fit);

    QWidget *asWidget() const;

    void onNewData(const CuData& da);

    void setZoomLevel(float n);
    void setMouseZoomEnabled(bool en);

    void setImage(const CuMatrix<double> &image);
    void setImage(const CuMatrix<unsigned short> &image);
    void setImage(const CuMatrix<unsigned char> &image);

protected:

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *we);

    void contextMenuEvent(QContextMenuEvent *e);

    void resizeEvent(QResizeEvent *re);

private:
    QuImageWidgetPrivate *d;

    // QuImageBaseI interface
public:
    QImage &image() const;
    void setColorTable(const QVector<QRgb> &rgb);
    QVector<QRgb> &colorTable() const;
    void setImageMouseEventInterface(QuImageMouseEventIf *ifa);
    bool isOpenGL() const;
    bool mouseZoomEnabled() const;
    float zoomLevel() const;

    QString source() const;
    void unsetSource();


    // image -> widget geometry mappings
    virtual QPoint mapToImg(const QPoint& p) const;
    virtual QRect mapToImg(const QRect& r) const;
    virtual QPoint mapFromImg(const QPoint& p) const;
    virtual QRect mapFromImg(const QRect& r) const;

public:

    // QuImageBaseListener interface
public:
    void onZoom(const QRect& from_z, const QRect& to_z);
    void onBoundsChanged(double lower, double upper);
};

#endif // IMAGE_H

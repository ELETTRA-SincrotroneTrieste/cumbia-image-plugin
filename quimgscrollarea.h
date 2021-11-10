#ifndef QUIMGSCROLLAREA_H
#define QUIMGSCROLLAREA_H

#include <QScrollArea>
#include <quimageplugininterface.h>

class QuImgScrollAreaPrivate;

class QuImgScrollArea : public QScrollArea, public QuImgScrollAreaI
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource DESIGNABLE true)
    Q_PROPERTY(bool mouseZoomEnabled READ mouseZoomEnabled WRITE setMouseZoomEnabled DESIGNABLE true)
    Q_PROPERTY(bool scaleContents READ scaleContents WRITE setScaleContents DESIGNABLE true)
    Q_PROPERTY(bool autoScale READ autoScale WRITE setAutoScale DESIGNABLE true)
    Q_PROPERTY(QImage errorImage READ errorImage WRITE setErrorImage  DESIGNABLE true)
    Q_PROPERTY(QImage image READ image WRITE setImage  DESIGNABLE true)

public:
    QuImgScrollArea(QWidget *parent, CumbiaPool *cu_p, const CuControlsFactoryPool& fpoo);
    ~QuImgScrollArea();

    bool scrollEnabled() const;

    // QuImgScrollAreaI interface
public:
    QScrollArea *scrollArea();
    void setImage(QuImageBaseI *ii);

    bool scaleContents() const;
    bool autoScale() const;
    double upperBound() const;
    double lowerBound() const;
    bool mouseZoomEnabled() const;
    QImage image() const;
    QImage errorImage() const;
    QString source() const;
    void setSource(const QString &s);
    QWidget *imageWidget() const;
    QuImageBaseI *imageBase() const;

public slots:
    void setImage(const QImage& img);
    void setErrorImage(const QImage &eimg);
    void setMouseZoomEnabled(bool e);
    void onZoomRectChanged(const QRect &from, const QRect &to);
    void setScaleContents(bool scale);
    void setAutoScale(bool as);
    void setLowerBound(double lb);
    void setUpperBound(double ub);

private:
    QuImgScrollAreaPrivate *d;
    bool m_mouseZoomEnabled;
    QImage m_errorImage;
    QString m_source;
};

#endif // QUIMGSCROLLAREA_H

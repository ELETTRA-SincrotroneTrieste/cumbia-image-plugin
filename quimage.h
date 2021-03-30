#ifndef QUIMAGE_H
#define QUIMAGE_H

#include <QWidget>
#include <qulabel.h>
#include <quimageplugininterface.h>
#include <QHBoxLayout>
#include <QScrollArea>


class QuImagePrivate {
public:
    bool scaleContents;
    QString source;
    QuImageBaseI *ii;
};

class QuImage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(bool scaleContents READ scaleContents WRITE setScaleContents)

public:
    QuImage(QWidget *parent) : QWidget(parent) {
        d = new QuImagePrivate;
        d->scaleContents = false;
        d->ii = nullptr;
    }

   QuImage(QWidget *parent, CumbiaPool * cu_p, const CuControlsFactoryPool& fpoo) : QWidget (parent) {
        d = new QuImagePrivate;
        d->scaleContents = false;
        d->ii = nullptr;

        QObject *image_plo;
        QuImagePluginInterface *plugin_i = QuImagePluginInterface::get_instance(cu_p, fpoo, &image_plo);
        if(!plugin_i)
            perr("CuDisplay: failed to load plugin \"%s\"", QuImagePluginInterface::file_name().toStdString().c_str());
        else {
            plugin_i->init(cu_p, fpoo);
            QuImgScrollAreaI *sa = plugin_i->new_scroll_area(this);
            QHBoxLayout *lo = new QHBoxLayout(this);
            d->ii = plugin_i->new_image(this);
            d->ii->setImage(QImage(":/seoyeji.png"));
            sa->setImage(d->ii);
            lo->addWidget(sa->scrollArea());
        }
    }

    ~QuImage() {
        delete d;
    }


    QString source() const {
        return d->ii->source();
    }

    bool scaleContents() const {
        return d->ii->scaleContents();
    }

public slots:
    void setSource(QString source)  {
        d->ii->setSource(source);
    }

    void setScaleContents(bool scaleContents) {
        d->ii->setScaleContents(scaleContents);
    }

signals:

private:
    QuImagePrivate *d;
};


#endif // QUIMAGE_H

#include "quimageplugin.h"
#include <cucontext.h>
#include <cucontrolsreader_abs.h>
#include <cudata.h>
#include <QTimer>
#include <QMap>
#include <QHBoxLayout>
#include <QtDebug>
#include <cucontrolsreader_abs.h>
#include <qwidget.h>
#include <QMetaProperty>
#include <qustringlist.h>
#include <qustring.h>
#include <QRegularExpression>
#include <quimagewidget.h>
#include <quimageGLwidget.h>
#include "quimgscrollarea.h"

class CuImagePluginPrivate {
public:
    CuImagePluginPrivate() : cu_pool(nullptr) {}

    CumbiaPool *cu_pool;
    CuControlsFactoryPool fpoo;
};

CuImagePlugin::CuImagePlugin(QObject *parent) : QObject(parent)
{
    d = new CuImagePluginPrivate;
}

CuImagePlugin::~CuImagePlugin() {
    delete d;
}

void CuImagePlugin::init(CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool) {
    d->cu_pool = cumbia_pool;
    d->fpoo = fpool;
}

/** \brief returns a reference to this object, so that it can be used as a QObject
 *         to benefit from signal/slot connections.
 *
 */
const QObject *CuImagePlugin::get_qobject() const {
    return this;
}

QuImageBaseI *CuImagePlugin::new_image(QWidget *parent, bool opengl) const {
    if(!opengl) return new QuImageWidget(parent, d->cu_pool, d->fpoo);
    else return new QuImageGLWidget(parent, d->cu_pool, d->fpoo);
}

QuImgScrollAreaI *CuImagePlugin::new_scroll_area(QWidget *parent) const {
    return new QuImgScrollArea(parent);
}

void CuImagePlugin::layout(QuImageBaseI *ii, QWidget *container) {
    QHBoxLayout *lo = new QHBoxLayout(container);
    lo->addWidget(ii->asWidget());
}

void CuImagePlugin::layout(QuImgScrollAreaI *sa, QWidget *container){
    QHBoxLayout *lo = new QHBoxLayout(container);
    lo->addWidget(sa->scrollArea());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(cumbia-image, CuImage)
#endif // QT_VERSION < 0x050000

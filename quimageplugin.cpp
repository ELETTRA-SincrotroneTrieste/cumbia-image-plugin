#include "quimageplugin.h"
#include <cucontext.h>
#include <cucontrolsreader_abs.h>
#include <cudata.h>
#include <QTimer>
#include <QMap>
#include <QtDebug>
#include <cucontrolsreader_abs.h>
#include <qwidget.h>
#include <QMetaProperty>
#include <qustringlist.h>
#include <qustring.h>
#include <QRegularExpression>
#include <quimagewidget.h>
#include <quimageGLwidget.h>

class CuImagePluginPrivate {
public:
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
    if(!opengl) return new QuImageWidget(parent);
    else return new QuImageGLWidget(parent);
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(cumbia-image, CuImage)
#endif // QT_VERSION < 0x050000

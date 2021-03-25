#include "cudisplay.h"

// cumbia
#include <cumbiapool.h>
#include <cuserviceprovider.h>
#include <cumacros.h>
#include <quapps.h>
// cumbia

#include <quimageplugininterface.h>
#include <QtDebug>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QCheckBox>

CuDisplay::CuDisplay(CumbiaPool *cumbia_pool, QWidget *parent) :
    QWidget(parent)
{
    // cumbia
    CuModuleLoader mloader(cumbia_pool, &m_ctrl_factory_pool, &m_log_impl);
    cu_pool = cumbia_pool;

    // mloader.modules() to get the list of loaded modules
    // cumbia

    // load magic plugin
    QObject *magic_plo;
    QuImagePluginInterface *plugin_i = QuImagePluginInterface::get_instance(cumbia_pool, m_ctrl_factory_pool, &magic_plo);
    if(!plugin_i)
        perr("CuDisplay: failed to load plugin \"%s\"", QuImagePluginInterface::file_name().toStdString().c_str());
    else if(qApp->arguments().size() > 1) {
        QScrollArea *sa = new QScrollArea(this);
        plugin_i->init(cumbia_pool, m_ctrl_factory_pool);
        QGridLayout *glo = new QGridLayout(this);
        QLabel *label = new QLabel(qApp->arguments().at(1), this);
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        QuImageBaseI *iw = plugin_i->new_image(this);
        iw->asWidget()->setObjectName("image_w");
        connect(iw->asWidget(), SIGNAL(zoomRectChanged(QRect)), this, SLOT(onZoomRectChanged(QRect)));
        printf("\e[1;33mCuDisplay : setting mouse tracking for testing purposes\e[0m\n");
        iw->asWidget()->setMouseTracking(true);
        sa->setWidget(iw->asWidget());
        QCheckBox *cb = new QCheckBox("Fit to window", this);
        connect(cb, SIGNAL(toggled(bool)), this, SLOT(setFitToWindow(bool)));
        iw->setSource(qApp->arguments().at(1));
        glo->addWidget(label, 0, 0, 1, 5);
        glo->addWidget(sa, 1, 0, 5, 5);
        glo->addWidget(cb, glo->rowCount(), 0, 1 , glo->columnCount());
        cb->setChecked(true);
    }

}

CuDisplay::~CuDisplay() {

}

void CuDisplay::setFitToWindow(bool f) {
    QScrollArea *sa = findChild<QScrollArea *>();
    sa->setWidgetResizable(f);
    findChild<QWidget *>("image_w")->setProperty("fitToWidget", f);
}

void CuDisplay::onZoomRectChanged(const QRect &r) {
    qDebug() << __PRETTY_FUNCTION__ << r;
}


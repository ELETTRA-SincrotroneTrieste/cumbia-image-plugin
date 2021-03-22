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
    else {
        QGridLayout *glo = new QGridLayout(this);
        QuImageBaseI *iw = plugin_i->new_image(this);
        glo->addWidget(iw->asWidget(), 0, 0, 5, 5);
    }

}

CuDisplay::~CuDisplay()
{

}


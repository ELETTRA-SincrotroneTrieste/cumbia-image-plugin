#include "cudisplay-designer.h"
#include "ui_cudisplay-designer.h"

// cumbia
#include <cumbiapool.h>
#include <cuserviceprovider.h>
#include <cumacros.h>
#include <quapps.h>
// cumbia

#include <QGridLayout>

#include <quimageplugininterface.h>

CuDisplayDes::CuDisplayDes(CumbiaPool *cumbia_pool, QWidget *parent) :
    QWidget(parent)
{
    // cumbia
    CuModuleLoader mloader(cumbia_pool, &m_ctrl_factory_pool, &m_log_impl);
    cu_pool = cumbia_pool;
    ui = new Ui::Quimagetest;
    ui->setupUi(this, cu_pool, m_ctrl_factory_pool);

    // mloader.modules() to get the list of loaded modules
    // cumbia

    QObject *image_plo;
    QuImagePluginInterface *plugin_i = QuImagePluginInterface::get_instance(cumbia_pool, m_ctrl_factory_pool, &image_plo);
    if(!plugin_i)
         perr("Quimagetest: failed to load plugin \"%s\"", qstoc(QuImagePluginInterface::file_name()));
     else { // image here
        QuImageBaseI *ii = plugin_i->new_image(this);
        QuImgScrollAreaI *scrolla = plugin_i->new_scroll_area(this);
        ii->setImage(QImage(":/seoyeji.png"));
        ii->setScaleContents(false);
        scrolla->setImage(ii);
        // insert the image into a QHBoxLayout automatically added to *ui->img1*
        plugin_i->layout(scrolla, ui->img); // (1) layout ii into ui->img1
     }
}

CuDisplayDes::~CuDisplayDes()
{
    delete ui;
}

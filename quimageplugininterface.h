#ifndef QUIMAGEPLUGININTERFACE_H
#define QUIMAGEPLUGININTERFACE_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QImage>
#include <cupluginloader.h>
#include <cumacros.h>
#include <cumatrix.h>

class Cumbia;
class CumbiaPool;
class CuControlsReaderFactoryI;
class CuControlsFactoryPool;
class QString;
class QStringList;
class CuData;
class CuContext;
class CuVariant;
class QuImageMouseEventIf;
class QScrollArea;
class QuImgScrollAreaI;

/*!
 * \brief Interface to the cumbia widget to display images
 *
 *
 */
class QuImageBaseI {
public:
    virtual ~QuImageBaseI() {}

    // set image methods
    virtual void setImage(const CuMatrix<double> &image) = 0;
    virtual void setImage(const CuMatrix<unsigned short> &image) = 0;
    virtual void setImage(const CuMatrix<unsigned char> &image) = 0;

    virtual void setImage(const QImage& img) = 0;
    virtual QImage& image() const = 0;

    virtual void setScaleContents(bool scale) = 0;
    virtual bool scaleContents() const = 0;

    // error methods
    virtual bool error() const = 0;
    virtual void setErrorImage(const QImage& i) = 0;
    virtual QImage errorImage() const = 0;
    virtual void setError(bool error) = 0;
    virtual void setErrorMessage(const QString& msg) = 0;

    // color methods
    virtual void setColorTable(const QVector<QRgb> &rgb) = 0;
    virtual QVector<QRgb>& colorTable() const = 0;

    virtual void setImageMouseEventInterface(QuImageMouseEventIf* ifa) = 0;

    virtual  bool isOpenGL() const = 0;

    // image -> widget geometry mappings
    virtual QPoint mapToImg(const QPoint& p) const = 0;
    virtual QRect mapToImg(const QRect& p) const = 0;
    virtual QPoint mapFromImg(const QPoint& p) const = 0;
    virtual QRect mapFromImg(const QRect& p) const = 0;

    // zoom methods
    virtual bool zoomEnabled() const = 0;
    virtual void setZoomEnabled(bool en) = 0;
    virtual float zoomLevel() const = 0;
    virtual void setZoomLevel(float l) = 0;

    virtual QWidget *asWidget() const = 0;

    // cumbia engine
    virtual QString source() const = 0;
    virtual void setSource(const QString& src) = 0;
    virtual void unsetSource() = 0;

};

/*!
 * \mainpage Plugin to read images with cumbia and display them on a Qt widget
 *
 * The plugin offers an interface to display images on a Qt widget using cumbia to get data from any supported engine.
 * If the image is not connected to the cumbia engine, it scales down to a simple viewer at the cost of a single pointer to an
 * unused object.
 *
 * The image can be drawn
 * \li *scaled* to the widget dimensions
 * \li within a *scroll area*, where either the entire or only a portion of an image can be drawn according to the widget dimensions.
 *
 * \par Scaled images
 * The image size adapts to the widget geometry and its *size hint* within a *layout*. In this configuration, zooming
 * is not possible.
 *
 * \par Images in a scroll area
 * In this case, the image can be zoomed either selecting an area with the mouse or programmatically.
 * The layout will affect the scroll area size and leave the image size unaffected.
 *
 * \image html seoyeji.png "Image in a scroll area"
 *
 * \par Zooming
 * The user can zoom the image drawing the desired rectangle with the mouse only if the
 * *scaleContents* property is disabled (default) and no QuImageMouseEventIf is installed through QuImageBaseI::setImageMouseEventInterface
 *
 * \par Loading the plugin
 * The plugin is loaded with the following few lines of code:
 *
 * \code
   #include <QuImagePluginInterface.h>
   // load image plugin
   QObject *image_plo;
   QuImagePluginInterface *plugin_i = QuImagePluginInterface::get_instance(cumbia_pool, m_ctrl_factory_pool, image_plo);
   if(!plugin_i)
        perr("MyimageApp: failed to load plugin \"%s\"", qstoc(QuImagePluginInterface::file_name()));
    else { // image here
    }
 * \endcode
 *
 * Once the plugin has been successfully loaded, an interface to an image (namely QuImageBaseI) can be obtained with the new_image method.
 * QuImageBase offers a rich interface to control the behaviour of the image. The plugin implementation can either instantiate a
 * QuImageWidget or a QuImageGLWidget (incomplete at the moment). They are QWidget instances so their *signals and slots* can be exploited
 * if deemed convenient. To obtain a reference to them *as qwidgets*, use QuImageBaseI::asWidget.
 * For example:
 *
 * \code
 * QuImageBaseI *ii = plugin_i->new_image(this);
 * connect(ii->asWidget(), SIGNAL(zoomRectChanged(QRect,QRect)), this, SLOT(onZoomRectChanged(QRect,QRect)));
 * ii->asWidget()->setProperty("scaleContents", false);
 * \endcode
 *
 * If you want to enclose the image in a scroll area, a convenience implementation of QScrollArea is provided by the plugin, and a new
 * instance can be obtained with the method new_scroll_area
 * The plugin implementation of the scroll area is offered by the QuImgScrollArea class, which adheres to the QuImgScrollAreaI interface.
 *
 * Embedding a QuImageBaseI into a QuImgScrollAreaI is useful because zooming is automatically integrated with scrolling:
 *
 * \code
 MyimageApp(QWidget *parent, CumbiaPool * cu_p, const CuControlsFactoryPool& fpoo) : QWidget (parent) {
    // ...
    // initialize plugin as above
    //
    if(!plugin_i)
        perr("MyimageApp: failed to load plugin \"%s\"", qstoc(QuImagePluginInterface::file_name()));
    else {
        QHBoxLayout *lo = new QHBoxLayout(this); // layout
        QuImgScrollAreaI *sa = plugin_i->new_scroll_area(this);
        QuImageBaseI *ii = plugin_i->new_image(this);
        ii->setImage(QImage(":/seoyeji.png"));
        sa->setImage(d->ii);
        lo->addWidget(sa->scrollArea());
    }
 * \endcode
 *
 * \par Connecting the image to cumbia
 *
 * To connect a QuImageBaseI to cumbia, you have to call first the QuImagePluginInterface::init method on the plugin interface
 * and then connect the image to the source:
 *
 * \code
    MyimageApp(QWidget *parent, CumbiaPool * cu_p, const CuControlsFactoryPool& fpoo) : QWidget (parent) {
          // plugin initialization..
          QuImageBaseI *ii = plugin_i->new_image(this);
          plugin_i->init(cumbia_pool, m_ctrl_factory_pool);   // 1
          ii->setSource("$1/uchar_image");                    // 2
          // ...
    }
 * \endcode
 *
 * Here, cumbia_pool and m_ctrl_factory_pool are a pointer to a CumbiaPool instance and a const reference to CuControlsFactoryPool respectively.
 * You will identificate them easily in the application if either it has been generated with *cumbia new project* or, more generally, if
 * the cumbia engine is already supported. Refer to the *cumbia* documentation if in doubt.
 *
 * \par Designer integration
 * Since there is no Qt Designer integration (yet) offering a ready to use widget, this is the quickest way to add images to an *UI*:
 *
 * In the Qt Designer form editor:
 *
 * \li add a base *QWidget* to the Qt Designer *form* and include it properly in a *layout* as you would for any other widget
 * \li give the widget an appropriate *objectName*, for example ("img1")
 * \note
 * Do not add a layout to *img1* and do not place any child widget within
 *
 * In the *cpp implementation* file, a one liner does the job (1):
 *
 * \code
   MyimageApp(QWidget *parent, CumbiaPool * cu_p, const CuControlsFactoryPool& fpoo) : QWidget (parent) {
          // plugin initialization..
          QuImageBaseI *ii = plugin_i->new_image(this);
          // insert the image into a QHBoxLayout automatically added to *ui->img1*
          plugin_i->layout(ii, ui->img1); // (1) layout ii into ui->img1
          // ...
   }
 * \endcode
 *
 * \par Further reading
 * The QuImageWidget and its incomplete counterpart QuImageGLWidget are QWidget and QOpenGLWidget implementing QuImageBaseI.
 * The QuImageBaseI::asWidget allows signal/slot connections to them as well as access to Qt properties.
 * QuImgScrollArea implements QuImgScrollAreaI
 *
 * See also QuImgScrollAreaI, QuImageBaseI, QuImageWidget, QuImgScrollArea.
 *
 *
 */
class QuImagePluginInterface
{
public:

    virtual ~QuImagePluginInterface() { }

    /** \brief Initialise the multi reader with mixed engine mode and the read mode.
     *
     * @param cumbia a reference to the CumbiaPool engine chooser
     * @param r_fac the CuControlsFactoryPool factory chooser
     *
     */
    virtual void init(CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool) = 0;

    /** \brief To provide the necessary signals aforementioned, the implementation must derive from
     *         Qt QObject. This method returns the subclass as a QObject, so that the client can
     *         connect to the multi reader signals.
     *
     * @return The object implementing QuImageI as a QObject.
     */
    virtual const QObject* get_qobject() const = 0;

    /*!
     * \brief get_image returns a new QuImageI object with parent the given QObject

     * \return a *new* instance of an object implementing QuImageI interface
     *
     * \see new_image_base
     */
    virtual QuImageBaseI *new_image(QWidget* parent, bool opengl = false) const = 0;

    virtual QuImgScrollAreaI *new_scroll_area(QWidget *parent) const = 0;

    /*!
     * \brief convenience method to layout an image into a container (that has no layout yet)
     * \param ii an image as QuImageBaseI
     * \param container the widget that will contain the image (QHBoxLayout will be used)
     *
     * If you are embedding an image into a QuImgScrollAreaI, consider using the convenience method below
     *
     * @see layout(QuImgScrollAreaI *scroll_a, QWidget* container)
     */
    virtual void layout(QuImageBaseI *ii, QWidget* container) = 0;

    /*!
     * \brief the same method as the above *layout* pertaining to a scroll area instead of an image
     *
     * @see layout(QuImageBaseI *ii, QWidget* container)
     */
    virtual void layout(QuImgScrollAreaI *scroll_a, QWidget* container) = 0;

    // convenience method to get the plugin instance
    /*!
     * \brief QuImagePluginInterface::get_instance returns a new instance of the plugin
     * \param cu_poo pointer to a previously allocated CumbiaPool
     * \param fpoo const reference to CuControlsFactoryPool
     * \param plugin_qob a pointer to a QObject, will contain this plugin *as QObject* (for signals and slots)
     * \return an instance of the plugin or nullptr in case of failure.
     *
     * \note Repeated calls will return the same plugin instance (by Qt plugin nature)
     *       Use QuImagePluginInterface::new_image to get new instances
     */
    static QuImagePluginInterface* get_instance(CumbiaPool *cu_poo,
                                                      const CuControlsFactoryPool& fpoo,
                                                      QObject **plugin_qob){
        QuImagePluginInterface *i;
        CuPluginLoader plo;
        i = plo.get<QuImagePluginInterface>(QuImagePluginInterface::file_name(), plugin_qob);
        if(!i)
            perr("QuImagePluginInterface::get_instance: failed to load plugin \"%s\"", qstoc(QuImagePluginInterface::file_name()));
        else
            i->init(cu_poo, fpoo);
        return i;
    }

    static QString file_name() { return "libcumbia-image-plugin.so"; }
};

class QuImgScrollAreaI {
public:
    virtual ~QuImgScrollAreaI() {}

    virtual QScrollArea *scrollArea() = 0;
    virtual void setImage(QuImageBaseI *ii) = 0;
    virtual void setScaleContents(bool scale) = 0;
};

#define QuImagePluginInterface_iid "eu.elettra.qutils.QuImagePluginInterface"

Q_DECLARE_INTERFACE(QuImagePluginInterface, QuImagePluginInterface_iid)

#endif // QUIMAGEPLUGININTERFACE_H

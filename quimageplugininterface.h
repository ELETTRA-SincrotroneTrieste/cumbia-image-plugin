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
 * \brief Interface to the cumbia widget to display images.
 *
 * QuImageBaseI defines an interface for clients of the plugin to control the behaviour of the cumbia image display
 * widget implementations, namely QuImageWidget and QuImageGLWidget. These two implement QWidget and they can be
 * accessed *as QWidget*s through the *asWidget* method. This allows the clients to use Qt properties and signal/slot
 * connections. For the rest, QuImageWidget and QuImageGLWidget shall only be controlled through the QuImageBaseI interface.
 *
 * There are some setImage flavors to set an image on the widget. An image can be set manually as a QImage or using cumbia
 * <a href="https://elettra-sincrotronetrieste.github.io/cumbia-libs/html/cumbia/html/classCuMatrix.html">CuMatrix</a> image container.
 * For the basic automatic usage of this plugin described in the examples it is not necessary to deal with CuMatrix.
 *
 * The setScaleContents method controls whether the image scales or not when the widget geometry changes (for example within a layout).
 * If scale contents is *false*, then the image can be zoom and placed in a scroll area to improve its management (see QuImgScrollAreaI).
 *
 * \par Error related methods
 * A custom image can be set in case of error (setErrorImage). The error condition can be triggered manually with setError or set internally
 * by the cumbia engine, if it has been enabled through QuImagePluginInterface::init and QuImageBaseI::setSource has been called.
 * The same logic applies to setErrorMessage.
 *
 * \par Color tables
 * The setColorTable allows changing the image color table. colorTable returns the one in use.
 *
 * \par Mouse events
 * Mouse events (press, release, move, wheel) can be intercepted by implementations of QuImageMouseEventIf.
 * setImageMouseEventInterface installs the mouse event interface. When a mouse interface is installed, the
 * QuImageBaseI shall not process mouse events automatically (for example, zooming).
 *
 * \par Mapping image to widget geometry
 * mapToImg functions map a point (or a rectangle) from widget coordinates to image coordinates.
 * mapFromImg map a point (or QRect) from image coordinates to widget coordinates.
 *
 * \par Zoom
 * Zooming is enabled by default if scaleContents is false. Zoom can be disabled with setZoomEnabled
 * and the level can be changed with setZoomLevel. The level shall be given as a percentage.
 *
 * \par QuImageBaseI widget implementation
 * QuImageBase is implemented by QuImageWidget and QuImageGLWidget, that are QWidget derived objects.
 * The asWidget method, returns a pointer to QuImageBase *as QWidget*.
 * Qt properties and signal/slot connections can be exploited.
 *
 * \par Connecting to cumbia
 * The setSource method connects the image to a cumbia source. The supported engines rely on cumbia and
 * application level configurations.
 * The source method returns the source of data in use.
 * In order for connections to work, QuImagePluginInterface::init shall be called beforehand
 *
 * \par Supported engine data types
 *
 * Supported data types from the cumbia engine are:
 * \li double
 * \li unsigned char
 * \li unsigned short
 * There is a setImage method version for each of them
 *
 */
class QuImageBaseI {
public:
    virtual ~QuImageBaseI() {}

    // set image methods
    /*!
     * \brief setImage double data type version
     * \param image CuMatrix of double
     */
    virtual void setImage(const CuMatrix<double> &image) = 0;

    /*!
     * \brief setImage unsigned short data type version
     * \param image CuMatrix of unsigned short
     */
    virtual void setImage(const CuMatrix<unsigned short> &image) = 0;

    /*!
     * \brief setImage unsigned char data type version
     * \param image CuMatrix of unsigned char
     */
    virtual void setImage(const CuMatrix<unsigned char> &image) = 0;

    /*!
     * \brief setImage from QImage
     * \param img a QImage
     */
    virtual void setImage(const QImage& img) = 0;

    /*!
     * \brief image returns the currently displayed image as QImage
     * \return a *reference* to the current image in use
     */
    virtual QImage& image() const = 0;

    /*!
     * \brief setScaleContents: if true the image scales with the widget's geometry
     *
     * \param scale true: the image scales proportionally to the widget geometry. Zoom not possible.
     * \param scale false: the image will not scale when the widget geometry changes. Zoom is enabled by default.
     *        Suitable to be inserted into a scroll area
     */
    virtual void setScaleContents(bool scale) = 0;

    /*!
     * \brief scaleContents returns the value of the scaleContents property
     * \see setScaleContents
     */
    virtual bool scaleContents() const = 0;

    // error methods
    /*!
     * \brief error returns the error condition
     * \return true if either an error occurred from the connected source or setError has been called explicitly
     */
    virtual bool error() const = 0;

    /*!
     * \brief setErrorImage set the image to be used in case of error
     * \param i const reference to QImage
     */
    virtual void setErrorImage(const QImage& i) = 0;

    /*!
     * \brief errorImage returns the image displayed in case of error condition
     * \return QImage used in case of error
     */
    virtual QImage errorImage() const = 0;

    /*!
     * \brief setError manually set (or clear) an error condition
     * \param error true: error condition
     * \param error false: no error condition
     */
    virtual void setError(bool error) = 0;

    /*!
     * \brief set a specific message to display in case of error condition
     * \param msg a QString
     */
    virtual void setErrorMessage(const QString& msg) = 0;

    // color methods

    /*!
     * \brief apply the given color table to the image
     * \param rgb a QVector of QRgb values
     */
    virtual void setColorTable(const QVector<QRgb> &rgb) = 0;

    /*!
     * \brief colorTable returns the color table currently in use
     * \return QVector<QRgb>&
     */
    virtual QVector<QRgb>& colorTable() const = 0;

    /*!
     * \brief install a custom mouse event handler
     * \param ifa an implementation of QuImageMouseEventIf.
     *
     * \note
     * Mouse events are sent to the interface without local processing
     */
    virtual void setImageMouseEventInterface(QuImageMouseEventIf* ifa) = 0;

    /*!
     * \brief isOpenGL shall return true if the implementation of QuImageBaseI is a QOpenGLWidget
     * \return true if the implementation of QuImageBaseI is a QOpenGLWidget
     * \return false if the implementation of QuImageBaseI is a QWidget
     */
    virtual  bool isOpenGL() const = 0;

    // image -> widget geometry mappings

    /*!
     * \brief map the point p from widget to image coordinates
     * \param p a QPoint in widget coordinates
     * \return a QPoint in the *image coordinates*
     *
     */
    virtual QPoint mapToImg(const QPoint& p) const = 0;

    /*!
     * \brief map the QRect r from widget to image coordinates
     * \param r a QRect in widget coordinates
     * \return a QRect in the *image coordinates*
     *
     */
    virtual QRect mapToImg(const QRect& r) const = 0;

    /*!
     * \brief map the point p from image to widget coordinates
     * \param p a QPoint in image coordinates
     * \return a QPoint in the *widget coordinates*
     *
     */
    virtual QPoint mapFromImg(const QPoint& p) const = 0;

    /*!
     * \brief map the rect r from image to widget coordinates
     * \param r a QRect in image coordinates
     * \return a QRect in the *widget coordinates*
     *
     */
    virtual QRect mapFromImg(const QRect& r) const = 0;

    // zoom methods
    /*!
     * \brief returns true if the mouse zoom is enabled
     *
     * \return true the zoom is enabled
     *
     * \note this function may return true while the zoom still is unavailable because
     * scaleContents is enabled.
     *
     * \note calling setZoomLevel with a value different from 100 disables the mouse zoom
     * and clears the zoom stack (the zoom history)
     */
    virtual bool mouseZoomEnabled() const = 0;

    /*!
     * \brief enable or disable the zoom
     * \param en true zoom enabled.
     *
     * \note scale contents property must be false
     *
     * \note calling this function with a true value resets zoomLevel to 100. On the other hand, calling
     * setZoomLevel with a value different from 100 disables mouse zoom and clears the zoom stack
     * (the zoom history). To enable mouse zoom again after setZoomLevel, call setMouseZoomEnabled again.
     */
    virtual void setMouseZoomEnabled(bool en) = 0;

    /*!
     * \brief returns the current zoom level
     * \return the zoom level, from 0 to 100
     *
     * @see setZoomLevel
     */
    virtual float zoomLevel() const = 0;

    /*!
     * \brief programmatically set a zoom level
     * \param l the zoom level from 0 to 100 (percent)
     *
     * \note
     * Usually zooming is accomplished mouse-selecting a rectangular region on the image
     *
     * \note scale contents must be false in order for zoom to operate
     *
     * \note calling this function with l different from 100 disables mouse zoom.
     * To enable mouse zoom again after setZoomLevel, call setMouseZoomEnabled again.
     */
    virtual void setZoomLevel(float l) = 0;

    /*!
     * \brief return this object *as QWidget*, to exploit Qt properties and signal/slots
     * \return this object as QWidget
     *
     * \note properties can be set and signal slots from implementations can be used if convenient, even though
     * only QuImageBaseI interface is available to the clients of the plugin.
     *
     * @see QuImageWidget
     * @see QuImageGLWidget
     */
    virtual QWidget *asWidget() const = 0;

    // cumbia engine

    /*!
     * \brief returns the source of the connection
     * \return a QString
     *
     * Refer to cumbia documentation for supported engines and source syntax (which depends on the engine)
     *
     * \see setSource
     * \see unsetSource
     */
    virtual QString source() const = 0;

    /*!
     * \brief set a source for the cumbia connection
     * \param src the source name, depending on the control system in use.
     *
     * See cumbia documentation for details on modules and supported engines.
     */
    virtual void setSource(const QString& src) = 0;

    /*!
     * \brief stop reading from the engine source to update the image
     *
     * Removes the connection to a previously specified source
     */
    virtual void unsetSource() = 0;


    virtual bool autoScale() const = 0;
    virtual double lowerBound() const = 0;
    virtual double upperBound() const = 0;

    virtual void setAutoScale(bool au) = 0;
    virtual void setLowerBound(double lb) = 0;
    virtual void setUpperBound(double ub) = 0;

};

/*!
 * \mainpage Plugin to read images with cumbia and display them on a Qt widget
 *
 * The plugin offers an interface to display images on a Qt widget using cumbia to get data from any supported engine.
 * If the image is not connected to the cumbia engine, it scales down to a simple viewer at the cost of a single pointer to an
 * unused object.
 *
 * \par Data types
 * Image data types are integrated in cumbia libs *since version 1.2.5*.
 * <a href="https://elettra-sincrotronetrieste.github.io/cumbia-libs/html/cumbia/html/classCuMatrix.html">CuMatrix</a>
 * is the container for image data. *CuMatrices* are carried within
 * <a href="https://elettra-sincrotronetrieste.github.io/cumbia-libs/html/cumbia/html/classCuVariant.html">CuVariant</a>s.
 * The latter provide constructors and conversion methods for the supported types. CuMatrix is a template class.
 * You may not need to know these low level details in order to use this plugin.
 * Some flavours of setImage are provided to support a limited subset of data types.
 *
 * The image can be drawn
 * \li *scaled* to the widget dimensions
 * \li within a *scroll area*, where either the entire or only a portion of an image can be drawn according to the widget dimensions.
 *
 * \par Scaled images
 * The image size adapts to the widget geometry and its *size hint* within a *layout*. In this configuration, zooming
 * is not possible.
 *
 * \image html seoyeji-scaled.png "Scaled image"
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
 * \image html seoyeji-zoom.png "Zooming in *non scaled* mode"
 *
 * Unzoom is accomplished clicking with the mouse mid button. Each click goes back one step in the zoom stack.
 *
 * \par Loading the plugin
 * The plugin is loaded with the following few lines of code:
 *
 * \code
   #include <QuImagePluginInterface.h>
   // load image plugin
   QObject *image_plo;
   QuImagePluginInterface *plugin_i = QuImagePluginInterface::get_instance(cumbia_pool, m_ctrl_factory_pool, &image_plo);
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
 * \image html tg_image.png "Reading TangoTest double_image attribute"
 *
 * \par Designer integration
 *
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
 * The *examples* directory contains a project named *cudisplay-designer* realised with this technique.
 *
 * \par Further reading
 * The QuImageWidget and its incomplete counterpart QuImageGLWidget are QWidget and QOpenGLWidget implementing QuImageBaseI.
 * The QuImageBaseI::asWidget allows signal/slot connections to them as well as access to Qt properties.
 * QuImgScrollArea implements QuImgScrollAreaI
 *
 * See also QuImgScrollAreaI, QuImageBaseI, QuImageWidget, QuImgScrollArea.
 *
 * \par Examples
 * The directory *examples/cudisplay* provides a working application to read images with cumbia:
 *
 * > cudisplay  test/device/1/double_image
 *
 * The cudisplay utility can be installed executing qmake with the *INSTALL_DIR* option:
 *
 * > qmake INSTALL_DIR=/usr/local/cumbia-libs/bin
 *
 * Under *examples*, the project *cudisplay-designer* is the equivalent realised with the Qt designer.
 *
 */
class QuImagePluginInterface
{
public:

    virtual ~QuImagePluginInterface() { }

    /** \brief Initialise the image reader with mixed engine mode (CumbiaPool + CuControlsFactoryPool).
     *
     * This call is necessary if you want to setSource on the image later and display an image from cumbia.
     *
     * @param cumbia_pool a reference to the CumbiaPool engine chooser
     * @param fpool the CuControlsFactoryPool factory chooser
     *
     */
    virtual void init(CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool) = 0;

    /** \brief Get a pointer to QuImagePlugin, that implements QuImagePluginInterface and *is* a QObject.
     *
     *  Obtaining a reference to this plugin as QObject allows to use Qt properties and signal/slots.
     *
     * @return This plugin as QObject.
     */
    virtual const QObject* get_qobject() const = 0;

    /*!
     * \brief get_image returns a new QuImageI object with the given parent

     * \return a *new* instance of an object implementing QuImageI interface
     *
     * \par Implementations
     * QuImageBaseI is implemented by QuImageWidget, QuImageGLWidget and QuImageBase.
     * The latter is a class used by QuImageWidget and QuImageGLWidget to implement common functionality. It shall
     * not be used by the clients of this plugin.
     * QuImageWidget and QuImageGLWidget are the plugin implementations and cannot be used by the clients of the
     * library with the exception of *signal / slot* connections. Clients shall use QuImageBaseI interface to
     * control QuImageWidget and QuImageGLWidget and, where convenient, QuImageWidget and QuImageGLWidget signals and
     * slots.
     *
     * \see new_scroll_area
     * \see QuImageWidget
     * \see QuImageGLWidget
     */
    virtual QuImageBaseI *new_image(QWidget* parent, bool opengl = false) const = 0;

    /*!
     * \brief new_scroll_area returns a new instance of QuImgScrollAreaI
     * \param parent the parent QWidget
     * \return a QScrollArea that implements QuImgScrollAreaI and handles changes in the image geometry.
     *
     * \par Example
     * \code
        QGridLayout *lo = new QGridLayout(this); // layout
        QuImgScrollAreaI *sa = plugin_i->new_scroll_area(this);
        QuImageBaseI *ii = plugin_i->new_image(this);
        ii->setImage(QImage(":/seoyeji.png"));
        sa->setImage(d->ii);
        lo->addWidget(sa->scrollArea(), 0, 0, 6, 6);
     * \endcode
     *
     * @see QuImgScrollAreaI
     * @see QuImgScrollAreaI::scrollArea
     *
     */
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

    /*!
     * \brief file_name returns this plugin's file name
     * \return  the name of the plugin: *libcumbia-image-plugin.so*.
     */
    static QString file_name() { return "libcumbia-image-plugin.so"; }
};

/*!
 * \brief A QScrollArea dedicated to hosting a QuImageBaseI
 *
 * You can get a new instance of QuImgScrollAreaI through QuImagePluginInterface::new_scroll_area,
 * use setImage to set a QuImageBaseI on the scroll area and finally add the scroll area to a
 * layout, either manually or through QuImagePluginInterface::layout(QuImgScrollAreaI *scroll_a, QWidget* container).
 *
 * The scrollArea method returns this object *as* a QScrollArea.
 * The setScaleContents called with a true parameter disables scrolling and scales the image according to the changes
 * in the area geometry. Scroll bars can be used to move the image in the view otherwise. Remember that zooming is possible
 * only if scale contents is false.
 *
 * An example is provided in the introduction.
 *
 */
class QuImgScrollAreaI {
public:
    virtual ~QuImgScrollAreaI() {}

    virtual QScrollArea *scrollArea() = 0;
    virtual void setImage(QuImageBaseI *ii) = 0;
    virtual void setAutoScale(bool au) = 0;
    virtual void setLowerBound(double lb) = 0;
    virtual void setUpperBound(double ub) = 0;
    void setBounds(double l, double u);
    virtual void setScaleContents(bool scale) = 0;
    virtual void setImage(const QImage& img) = 0;
    virtual void setErrorImage(const QImage &eimg) = 0;
    virtual void setMouseZoomEnabled(bool e) = 0;

    virtual bool mouseZoomEnabled() const = 0;
    virtual bool autoScale() const = 0;
    virtual double lowerBound() const = 0;
    virtual double upperBound() const = 0;
    virtual QImage image() const = 0;
    virtual QImage errorImage() const = 0;
    virtual QString source() const = 0;
    virtual void setSource(const QString &s) = 0;
    virtual QWidget *imageWidget() const = 0;
    virtual QuImageBaseI *imageBase() const = 0;
};

#define QuImagePluginInterface_iid "eu.elettra.qutils.QuImagePluginInterface"

Q_DECLARE_INTERFACE(QuImagePluginInterface, QuImagePluginInterface_iid)

#endif // QUIMAGEPLUGININTERFACE_H

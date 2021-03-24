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
class ImageMouseEventInterface;

class QuImageBaseI {
public:
    virtual ~QuImageBaseI() {};

    // set image methods
    virtual void setImage(const CuMatrix<double> &image) = 0;
    virtual void setImage(const CuMatrix<unsigned short> &image) = 0;
    virtual void setImage(const CuMatrix<unsigned char> &image) = 0;

    virtual void setImage(const QImage& img) = 0;
    virtual QImage& image() const = 0;

    // error methods
    virtual void setErrorImage(const QImage& i) = 0;
    virtual QImage errorImage() const = 0;
    virtual void setError(bool error) = 0;
    virtual void setErrorMessage(const QString& msg) = 0;

    // color methods
    virtual void setColorTable(const QVector<QRgb> &rgb) = 0;
    virtual QVector<QRgb>& colorTable() const = 0;

    virtual void setImageMouseEventInterface(ImageMouseEventInterface* ifa) = 0;

    virtual  bool isOpenGL() const = 0;

    // zoom methods
    virtual bool zoomEnabled() const = 0;
    virtual void setZoom(int n) = 0;
    virtual void setZoomEnabled(bool en) = 0;
    virtual float zoom() const = 0;

    virtual QWidget *asWidget() const = 0;

    // cumbia engine
    virtual void setSource(const QString& src) = 0;
    virtual void unsetSource() = 0;

};

/*!
 * \mainpage Add some cumbia image to simple Qt widgets
 *
 * \section Introduction
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
 * \endcode
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

#define QuImagePluginInterface_iid "eu.elettra.qutils.QuImagePluginInterface"

Q_DECLARE_INTERFACE(QuImagePluginInterface, QuImagePluginInterface_iid)

#endif // QUIMAGEPLUGININTERFACE_H

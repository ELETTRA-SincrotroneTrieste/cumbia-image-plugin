#ifndef QUIMAGEPLUGIN_H
#define QUIMAGEPLUGIN_H

#include <QWidget>
#include <QList>
#include <quimageplugininterface.h>
#include <quimagebase.h>
#include <cudata.h>
#include <cudatalistener.h>
#include <qustring.h>
#include <qustringlist.h>


class CuImagePluginPrivate;
class Cumbia;
class CumbiaPool;
class CuControlsReaderFactoryI;
class CuControlsFactoryPool;
class CuControlsReaderA;

class QuImagePrivate
{
public:
    CuContext *context;
    QString src; // bare src passed in setSource
};


/*!
 *
 * This plugin provides
 */
class CuImagePlugin : public QObject, public QuImagePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "cumbia-image.json")

    Q_INTERFACES(QuImagePluginInterface)

public:

    CuImagePlugin(QObject *parent = nullptr);
    virtual ~CuImagePlugin();

    // CuImagePluginInterface interface
public:
    void init(CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool);
    const QObject *get_qobject() const;

private:
    CuImagePluginPrivate *d;

    // QuImagePluginInterface interface
public:
    QuImageBaseI *new_image(QWidget *parent, bool opengl) const;
//    QuImageGLWidgetA *new_gl_image(QWidget *parent) const;
};

#endif // QUIMAGEPLUGIN_H

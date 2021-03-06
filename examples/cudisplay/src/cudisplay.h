#ifndef Magicdemo_H
#define Magicdemo_H

#include <QWidget>

// cumbia
#include <qulogimpl.h>
#include <cucontrolsfactorypool.h>
#include <quplot_base.h>
class CumbiaPool;
// cumbia

class QuImageBaseI;

namespace Ui {
class Magicdemo;
}

class CuDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit CuDisplay(CumbiaPool *cu_p, QWidget *parent = 0);
    ~CuDisplay();
protected slots:
private:
    Ui::Magicdemo *ui;

    // cumbia
    CumbiaPool *cu_pool;
    QuLogImpl m_log_impl;
    CuControlsFactoryPool m_ctrl_factory_pool;
    // cumbia

    QuImageBaseI *iw;
};

#endif // Magicdemo_H

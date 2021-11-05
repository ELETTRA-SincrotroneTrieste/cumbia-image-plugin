#ifndef Quimagetest_H
#define Quimagetest_H

#include <QWidget>

// cumbia
#include <qulogimpl.h>
#include <cucontrolsfactorypool.h>
class CumbiaPool;
// cumbia

namespace Ui {
class Quimagetest;
}

class CuDisplayDes : public QWidget
{
    Q_OBJECT

public:
    explicit CuDisplayDes(CumbiaPool *cu_p, QWidget *parent = 0);
    ~CuDisplayDes();

private:
    Ui::Quimagetest *ui;

    // cumbia
    CumbiaPool *cu_pool;
    QuLogImpl m_log_impl;
    CuControlsFactoryPool m_ctrl_factory_pool;
    // cumbia
};

#endif // Quimagetest_H

#include <quapplication.h>
#include "cudisplay.h"

// cumbia
#include <cumbiapool.h>
#include <cuthreadfactoryimpl.h>
#include <qthreadseventbridgefactory.h>
// cumbia

#define VERSION "1.0"

int main(int argc, char *argv[])
{
    QuApplication qu_app( argc, argv );
    qu_app.setOrganizationName("Elettra");
    qu_app.setApplicationName("CuDisplay");
    QString version(VERSION);
    qu_app.setApplicationVersion(version);
    qu_app.setProperty("author", "Giacomo");
    qu_app.setProperty("mail", "giacomo.strangolino@elettra.eu");
    qu_app.setProperty("phone", "0403758073");
    qu_app.setProperty("office", "T2PT025");
    qu_app.setProperty("hwReferent", "$HW_REFERENT$"); /* name of the referent that provides the device server */
    
    int ret = 1;
    if(argc < 2)
        perr("CuDisplay: usage: %s source\n", argv[0]);
    else {

        CumbiaPool *cu_p = new CumbiaPool();
        CuDisplay *w = new CuDisplay(cu_p, NULL);
        w->show();
        // exec application loop
        ret = qu_app.exec();
        // delete resources and return
        delete w;

        for(std::string n : cu_p->names())
            if(cu_p->get(n))
                delete cu_p->get(n);
    }
    return ret;
}

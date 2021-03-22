#include "quimagereadevent.h"

QuImageReadEvent::QuImageReadEvent() : QEvent(QEvent::Type(QEvent::User + 1))
{
    error = false;
    save = false;
    message = "";
    dimX = dimY = 0;
}

QuImageReadEvent::~QuImageReadEvent()
{

}

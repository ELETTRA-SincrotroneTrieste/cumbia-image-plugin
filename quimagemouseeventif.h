#ifndef QUIMAGEMOUSEEVENTIF_H
#define QUIMAGEMOUSEEVENTIF_H

class QPoint;
#include <QMouseEvent>

class QuImageMouseEventIf
{
public:
    QuImageMouseEventIf();

    virtual void imageMousePressEvent(const QPoint& pos, int button) = 0;
    virtual void imageMouseReleaseEvent(const QPoint& pos, int button) = 0;
    virtual void imageMouseMoveEvent(const QPoint& pos, int button) = 0;
    virtual void imageMouseWheelEvent(QWheelEvent *event) = 0 ;
};

#endif // IMAGEMOUSEEVENTINTERFACE_H

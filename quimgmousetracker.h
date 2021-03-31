#ifndef QUIMGMOUSETRACKER_H
#define QUIMGMOUSETRACKER_H

class QMouseEvent;
class QWheelEvent;
class QuImageBasePrivate;
class QuImageMouseEventIf;
class QuImgZoomer;

#include <QPoint>


/*!
 * \internal
 */
class QuImgMouseTracker
{
public:
    QuImgMouseTracker(QuImageBasePrivate* _img_d, QuImgZoomer *z);

    void mouseMove(QMouseEvent *e);
    void mousePress(QMouseEvent *e);
    void mouseRelease(QMouseEvent *e);
    void wheelEvent(QWheelEvent * event);

    void setMouseEventInterface(QuImageMouseEventIf *i);

    QPoint mP1, mP2;
    bool left_button;

private:
    QuImageBasePrivate* img_d;
    QuImageMouseEventIf *mou_if;
    QuImgZoomer *zoomer;
};

#endif // QUIMGMOUSETRACKER_H

#ifndef QUIMGPAINTER_H
#define QUIMGPAINTER_H

class QPaintEvent;
class QuImageBasePrivate;


/*!
 * \internal
 */
class QuImgPainter
{
public:
    QuImgPainter();
    virtual ~QuImgPainter();
    void paint(QPaintEvent *e, QuImageBasePrivate *imgb);
    bool dirty;
};

#endif // QUIMGPAINTER_H

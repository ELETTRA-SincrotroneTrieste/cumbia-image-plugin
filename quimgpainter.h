#ifndef QUIMGPAINTER_H
#define QUIMGPAINTER_H

class QPaintEvent;
class QuImageBasePrivate;

class QuImgPainter
{
public:
    QuImgPainter();
    virtual ~QuImgPainter();
    void paint(QPaintEvent *e, QuImageBasePrivate *imgb);
    bool dirty;
};

#endif // QUIMGPAINTER_H

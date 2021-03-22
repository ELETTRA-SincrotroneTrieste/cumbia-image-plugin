#ifndef QUIMAGEREADEVENT_H
#define QUIMAGEREADEVENT_H

#include <QEvent>
#include <QString>

class QuImageReadEvent : public QEvent
{
public:
    QuImageReadEvent();

    virtual ~QuImageReadEvent();

    bool error;
    bool save;
    QString message;

    int imageDepth;

    int dimX, dimY;
    std::vector<unsigned char> imageArray8;
    std::vector<unsigned short> imageArray16;
};

#endif // IMAGEREADEVENT_H

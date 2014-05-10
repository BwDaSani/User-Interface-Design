#include "addimagecommand.h"

AddImageCommand:: AddImageCommand(QString text, QVector<QPixmap> inVec, int inLoc, BoxImage *inBox, QTimer *Intimer)
{
    boxImage = inBox;
    Vec = inVec;
    insertLoc = inLoc;
    setText(text);
    timer=Intimer;
    timerActive=0;
}

void AddImageCommand::undo()
{
    if (timer->isActive())
    {
        timer->stop();
        timerActive=1;
    }
    else
    {
        timerActive=0;
    }
    boxImage->deleteImages(insertLoc, insertLoc + Vec.size() - 1);
    if (timerActive)
    {
        timer->start();
    }
}

void AddImageCommand::redo()
{
    if (timer->isActive())
    {
        timer->stop();
        timerActive=1;
    }
    else
    {
        timerActive=0;
    }
    boxImage->insertMultipleImages(Vec, insertLoc);
    if (timerActive)
    {
        timer->start();
    }
}

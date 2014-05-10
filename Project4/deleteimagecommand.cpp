#include "deleteImagecommand.h"

DeleteImageCommand:: DeleteImageCommand(QString text, int instart, int inend, BoxImage *inBox, QTimer *Intimer)
{
    boxImage = inBox;
    start=instart;
    end=inend;
    setText(text);
    timer=Intimer;
    timerActive=0;
}

void DeleteImageCommand::undo()
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
    boxImage->insertMultipleImages(Vec, start);
    if (timerActive)
    {
        timer->start();
    }
}

void DeleteImageCommand::redo()
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

    boxImage->deleteImages(start, end);
    if (timerActive)
    {
        timer->start();
    }
}
void DeleteImageCommand::setPixmapList(QVector<QPixmap> inVec)
{
Vec=inVec;
}

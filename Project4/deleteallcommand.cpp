#include "deleteallcommand.h"

DeleteAllCommand:: DeleteAllCommand(QString text, int instart, int inend, BoxImage *inBox, QTimer *Intimer)
{
    boxImage = inBox;
    start=instart;
    end=inend;
    setText(text);
    timer=Intimer;
    timerActive=0;
}

void DeleteAllCommand::undo()
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

void DeleteAllCommand::redo()
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
void DeleteAllCommand::setPixmapList(QVector<QPixmap> inVec)
{
Vec=inVec;
}

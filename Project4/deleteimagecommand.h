#ifndef DELETEIMAGECOMMAND_H
#define DELETEIMAGECOMMAND_H
#include <QUndoCommand>

#include "boximage.h"

class DeleteImageCommand : public QUndoCommand
{
public:
    DeleteImageCommand(QString text, int instart, int inend, BoxImage *inBox,QTimer *Intimer);

    void undo();
    void redo();
    void setPixmapList(QVector<QPixmap> inVec);

private:
    BoxImage *boxImage;
    int start;
    int end;
    QTimer* timer;
    QVector<QPixmap> Vec;
    bool timerActive;
};
#endif // DELETEIMAGECOMMAND_H




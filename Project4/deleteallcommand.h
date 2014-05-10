#ifndef DELETEALLCOMMAND_H
#define DELETEALLCOMMAND_H
#include <QUndoCommand>

#include "boximage.h"

class DeleteAllCommand : public QUndoCommand
{
public:
    DeleteAllCommand(QString text, int instart, int inend, BoxImage *inBox,QTimer *Intimer);

    void undo();
    void redo();
    void setPixmapList(QVector<QPixmap> inVec);

private:
    BoxImage *boxImage;
    int start;
    int end;
    QTimer *timer;
    QVector<QPixmap> Vec;
    bool timerActive;
};
#endif // DELETEALLCOMMAND_H

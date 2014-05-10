#ifndef ADDIMAGECOMMAND_H
#define ADDIMAGECOMMAND_H
#include <QUndoCommand>

#include "boximage.h"

class AddImageCommand : public QUndoCommand
{
public:
    AddImageCommand(QString text, QVector<QPixmap> inVec, int inLoc, BoxImage *inBox,QTimer *Intimer);

    void undo();
    void redo();

private:
    BoxImage *boxImage;
    int insertLoc;
    QTimer *timer;
    QVector<QPixmap> Vec;
    bool timerActive;
};



#endif // ADDIMAGECOMMAND_H

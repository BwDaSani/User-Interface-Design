#ifndef ADDCOLLECTIONCOMMAND_H
#define ADDCOLLECTIONCOMMAND_H
#include <QUndoCommand>

#include "mainwindow.h"

class AddCollectionCommand : public QUndoCommand
{
public:
    AddCollectionCommand(QString text, QVector<QPixmap> inVec, int inLoc, BoxImage *inBox,QTimer *Intimer);

    void undo();
    void redo();

private:
    BoxImage *boxImage;
    int insertLoc;
    bool timerActive;
    QTimer *timer;

    QVector<QPixmap> Vec;
};

#endif // ADDCOLLECTIONCOMMAND_H

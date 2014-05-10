#ifndef CCP_H
#define CCP_H
#include "boximage.h"
class CopyCommand : public QUndoCommand
{
public:
    AddImageCommand(QString text, QVector<QPixmap> inVec, int inLoc, BoxImage *inBox);

    void undo();
    void redo();

private:
    BoxImage *boxImage;
    int insertLoc;

    QVector<QPixmap> Vec;
};

#endif // CCP_H

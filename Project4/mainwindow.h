#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "boximage.h"
#include "flickrcollector.h"
#include "imagecollector.h"
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QSplitter *splitter;
    BoxImage *boxImage;
    QUndoStack *undoStack;
    QClipboard *clipBoard;
    QToolButton *leftBut;
    FlickrCollector *flick;
    ImageCollector *collector;
    QVector<QPixmap> flickVec;
    QTimer *timer;
    QDockWidget *dock;

    QAction *quitAction;
    QAction *addCollectionAction;
    QAction *addImageAction;
    QAction *deleteAllAction;
    QAction *deleteImageAction;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *butAct;
    QAction *pasteAct;
    QAction *flkr;
    QAction *playAct;
    QAction *stopAct;

    //QAction *timerCall;
    void AddIntoBox();
    ~MainWindow();

public slots:
    void updatePaste();
    void updateBut();
    void notYetCallback(void);
    void pasteCallback();
    void copyCallback();
    void cutCallback();
    void flkrCallBack();
    void getUrl();
    void flkrAdd(QPixmap);
    void TimerStart();
    void TimerStop();
    void SlideShow();
    void Debug();
    void flkrfail();
private:
    Ui::MainWindow *ui;
    int curLoc;
    int flkrcount;
    QMessageBox MBox;
};

#endif // MAINWINDOW_H

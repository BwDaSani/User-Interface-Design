#include "mainwindow.h"
#include "boximage.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QtWidgets>
#include <QUndoStack>
#include <QBoxLayout>
#include "addimagecommand.h"
#include "deleteimagecommand.h"
#include "flickrcollector.h"
#include "imagecollector.h"
#include "addcollectioncommand.h"

using namespace std;
const int IMAGE_SIZE = 144;
const int BUTTON_SIZE = 150;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    undoStack = new QUndoStack();
    flick= new FlickrCollector(this);
    dock=new QDockWidget(tr("Image Preview"),this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    collector=new ImageCollector();
        QUndoView *undoView = new QUndoView(undoStack);
        undoView->setWindowTitle(tr("Command List"));
        undoView->show();
        undoView->setAttribute(Qt::WA_QuitOnClose, false);

        clipBoard = QApplication::clipboard();

        newAction = new QAction("&New",this);
        newAction->setShortcuts(QKeySequence::New);

        openAction = new QAction("&Open",this);
        openAction->setShortcuts(QKeySequence::Open);

        saveAction = new QAction("&Save",this);
        saveAction->setShortcuts(QKeySequence::Save);

        saveAsAction = new QAction("S&ave as",this);
        saveAsAction->setShortcuts(QKeySequence::SaveAs);

        quitAction = new QAction("&Quit",this);
        quitAction->setShortcuts(QKeySequence::Quit);

        cutAction = new QAction("&Cut",this);
        cutAction->setShortcuts(QKeySequence::Cut);

        copyAction = new QAction("&Copy",this);
        copyAction->setShortcuts(QKeySequence::Copy);

        pasteAction = new QAction("&Paste",this);
        pasteAction->setShortcuts(QKeySequence::Paste);

        addCollectionAction = new QAction("Add &Collection",this);
        addCollectionAction->setShortcut(QKeySequence::SelectAll);

        addImageAction = new QAction("Add &Image",this);
        addImageAction->setShortcut(QKeySequence::Italic);

        deleteAllAction = new QAction("&Delete All Images",this);
        deleteAllAction->setShortcut(QKeySequence("CTRL+p"));

        deleteImageAction = new QAction("&Delete Image",this);
        deleteImageAction->setShortcut(QKeySequence("CTRL+m"));

        flkr=new QAction("&Add From Flickr",this);
        flkr->setShortcut(QKeySequence("CTRL+F"));

        playAct=new QAction("&Play",this);
        playAct->setShortcut(QKeySequence("CTRL+."));

        stopAct=new QAction("&Stop",this);
        stopAct->setShortcut(QKeySequence("CTRL+/"));

        QMenu* fileMenu = this->menuBar()->addMenu("&File");
        fileMenu->addAction(newAction);
        fileMenu->addAction(openAction);
        fileMenu->addSeparator();
        fileMenu->addAction(saveAction);
        fileMenu->addAction(saveAsAction);
        fileMenu->addSeparator();
        fileMenu->addAction(quitAction);

        QMenu* editMenu = this->menuBar()->addMenu("&Edit");
        QAction *temp = undoStack->createUndoAction(editMenu);
        temp->setShortcut(QKeySequence("ctrl+Z"));
        editMenu->addAction(temp);
        temp = undoStack->createRedoAction(editMenu);
        temp->setShortcut(QKeySequence("ctrl+Y"));
        editMenu->addAction(temp);
        editMenu->addSeparator();
        editMenu->addAction(cutAction);
        editMenu->addAction(copyAction);
        editMenu->addAction(pasteAction);

        QMenu* imageMenu = this->menuBar()->addMenu("&Images");
        imageMenu->addAction(addCollectionAction);
        imageMenu->addAction(addImageAction);
        imageMenu->addAction(flkr);
        imageMenu->addSeparator();
        imageMenu->addAction(deleteAllAction);
        imageMenu->addAction(deleteImageAction);

        QMenu* showMenu = this ->menuBar()->addMenu("&Slide Show");
        showMenu->addAction(playAct);
        showMenu->addAction(stopAct);


        //splitter = new QSplitter();


        QFrame *leftWidget = new QFrame();
        QGridLayout *leftLayout = new QGridLayout();
        leftWidget->setLayout(leftLayout);
        leftWidget->setStyleSheet("QFrame { background-color: rgb(219, 226, 228); }");


        QFrame *innerWidget = new QFrame();
        QGridLayout *innerLayout = new QGridLayout();
        innerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        innerWidget->setLayout(innerLayout);

        leftBut=new QToolButton();
        leftBut->setMaximumWidth(300);
        leftBut->setMinimumWidth(300);
        leftBut->setMinimumHeight(300);
        leftBut->setText("Nothing Selected");
        //leftBut->setEnabled(false);

        innerLayout->addWidget(leftBut,0,0);
        leftLayout->addWidget(innerWidget,0,0, Qt::AlignTop);
        dock->setWidget(leftWidget);
        leftBut->show();
        dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
        addDockWidget(Qt::LeftDockWidgetArea, dock);


        /*const int N_BUTTONS = 4;
        QToolButton *button[N_BUTTONS];
        QString buttonStr[N_BUTTONS] = {"Add Collection","Add Action",
                                        "Delete All", "Delete Image"};
        for (int i=0;i < N_BUTTONS; i++) {
            button[i] = new QToolButton();
            button[i]->setMinimumSize(150,75);
            button[i]->setMaximumSize(150,75);
            button[i]->setText(buttonStr[i]);
            innerLayout->addWidget(button[i],i,0);
        }
        button[0]->setDefaultAction(addCollectionAction);
        button[1]->setDefaultAction(addImageAction);
        button[2]->setDefaultAction(deleteAllAction);
        button[3]->setDefaultAction(deleteImageAction);*/

        leftLayout->addWidget(innerWidget,0,0, Qt::AlignTop);
        //splitter->addWidget(leftWidget);


        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(1);
        boxImage = new BoxImage(undoStack,timer, this);
        boxImage->setMaximumWidth(BUTTON_SIZE*5+20);
        boxImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
        scrollArea->setWidget(boxImage);
        //splitter->addWidget(scrollArea);
        boxImage->setStyleSheet("QFrame { background-color: rgb(254,254,254); }");
        scrollArea->setStyleSheet("QScrollArea { background-color: rgb(254,254,254); }");


        setStyleSheet("QDockWidget {background-color: rgb(219, 226, 228);}");
        QList<int> sizeList = QList<int>() << 200 << BUTTON_SIZE*5;
        //splitter->setSizes(sizeList);
        this->setCentralWidget(scrollArea);
        this->resize(BUTTON_SIZE*5+200,400);


        connect (quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
        connect (addCollectionAction, SIGNAL(triggered()),
                 boxImage, SLOT(addCollectionCallback()));
        connect (addImageAction, SIGNAL(triggered()),
                 boxImage, SLOT(addImageCallback()));
        connect (deleteAllAction, SIGNAL(triggered()),
                 boxImage, SLOT(deleteAllCallback()));
        connect (deleteImageAction, SIGNAL(triggered()),
                 boxImage, SLOT(deleteImageCallback()));
        connect(cutAction, SIGNAL(triggered()),
                this, SLOT(cutCallback()));
        connect(copyAction, SIGNAL(triggered()),
                this, SLOT(copyCallback()));
        connect(pasteAction, SIGNAL(triggered()),
                this, SLOT(pasteCallback()));
        connect(playAct,SIGNAL(triggered()),this,SLOT(TimerStart()));
        connect(stopAct,SIGNAL(triggered()),this,SLOT(TimerStop()));
        connect(flick,SIGNAL(ready()),this,SLOT(flkrCallBack()));
        connect(flick,SIGNAL(failed()),this,SLOT(flkrfail()));
        connect(collector,SIGNAL(pixmapAvailable(QPixmap)),this,SLOT(flkrAdd(QPixmap)));

        updatePaste();

        pasteAct=new QAction("&check paste",this);

        connect(newAction,SIGNAL(triggered()),this,SLOT(notYetCallback()));
        connect(openAction,SIGNAL(triggered()),this,SLOT(notYetCallback()));
        connect(saveAction,SIGNAL(triggered()),this,SLOT(notYetCallback()));
        connect(saveAsAction,SIGNAL(triggered()),this,SLOT(notYetCallback()));
        connect(clipBoard,SIGNAL(dataChanged()),this,SLOT(updatePaste()));
        connect(flkr,SIGNAL(triggered()),this,SLOT(getUrl()));
        connect(timer,SIGNAL(timeout()),this,SLOT(SlideShow()));

        boxImage->buttonGroup->connect(boxImage->buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
                                   this, SLOT(updateBut()));


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::notYetCallback(void)
{
    QMessageBox message;
    message.setText("Sorry, this functionality is not yet implemented.");
    message.exec();
}

void MainWindow::updateBut()
{
    QPixmap pixmap;
    QPixmap pixmap1;
    QLayoutItem *item;
    QIcon icon;

    QToolButton *button = (QToolButton *) boxImage->buttonGroup->checkedButton();
    int itemNum = boxImage->imageGrid->indexOf(button);
    if (itemNum < 0)
    {
      leftBut->setText("Nothing Selected");
      return;
    }
    item = boxImage->imageGrid->itemAt(itemNum);
    icon = (dynamic_cast<QToolButton *>(item->widget()))->icon();
    pixmap=icon.pixmap(150,150);
    pixmap1=pixmap.scaled(300,300,Qt::KeepAspectRatio);
    leftBut->setIcon(QIcon(pixmap1));
    leftBut->setIconSize(QSize(300,300));
    qDebug()<<"here";

}

void MainWindow::updatePaste()
{
    const QMimeData *data = clipBoard->mimeData();

    if (data->hasImage())
    {
    pasteAction->setEnabled(true);
    }
    else
    {
    pasteAction->setEnabled(false);
    }
}
void MainWindow::pasteCallback()
{

    const QMimeData *data = clipBoard->mimeData();

    if (data->hasImage()) {
        bool k=0;
        if(timer->isActive())
        {
        TimerStop();
        k=1;
        }
        qDebug() << "Paste image";
        QVector<QPixmap> CopyVec;
        QPixmap curPix = clipBoard->pixmap();
        CopyVec.append(curPix);

        int insertLoc;
        QToolButton *button = (QToolButton *) boxImage->buttonGroup->checkedButton();
        if (button == 0)
            insertLoc = boxImage->imageGrid->count();

        int selectNum = boxImage->imageGrid->indexOf(button);
        if (selectNum >= 0)
            insertLoc = selectNum;

        AddImageCommand *command = new AddImageCommand("paste image", CopyVec, insertLoc, boxImage,timer);
        undoStack->push(command);
        if(k)
        {
            TimerStart();
        }
    }
    else {
        qDebug() << "Something is wrong";
    }

}
void MainWindow::copyCallback()
{
QVector<QPixmap> Vec;
QPixmap pixmap;
QLayoutItem *item;
QIcon icon;

QToolButton *button = (QToolButton *) boxImage->buttonGroup->checkedButton();
int itemNum = boxImage->imageGrid->indexOf(button);
if (itemNum < 0)
{
  return;
}
item = boxImage->imageGrid->itemAt(itemNum);
icon = (dynamic_cast<QToolButton *>(item->widget()))->icon();
pixmap=icon.pixmap(150,150);
Vec.append(pixmap);
clipBoard->setPixmap(pixmap);

}

void MainWindow::cutCallback()
{
    bool k=0;
    if(timer->isActive())
    {
    TimerStop();
    k=1;
    }

    QVector<QPixmap> Vec;
    QPixmap pixmap;
    QLayoutItem *item;
    QIcon icon;

    QToolButton *button = (QToolButton *) boxImage->buttonGroup->checkedButton();
    int itemNum = boxImage->imageGrid->indexOf(button);
    if (itemNum < 0)
    {
      return;
    }
    item = boxImage->imageGrid->itemAt(itemNum);
    icon = (dynamic_cast<QToolButton *>(item->widget()))->icon();
    pixmap=icon.pixmap(150,150);
    Vec.append(pixmap);
    clipBoard->setPixmap(pixmap);
    DeleteImageCommand *command= new DeleteImageCommand("cut",itemNum,itemNum,boxImage,timer);
    command->setPixmapList(Vec);
    undoStack->push(command);
    if(k)
    {
        TimerStart();
    }
}

void MainWindow::getUrl()
{
    addCollectionAction->setEnabled(false);
    addImageAction->setEnabled(false);
    flickVec.clear();
    flick->execute();
    qDebug()<<"Getting URL";
}

void MainWindow::flkrCallBack()
{
//flick->printList();
qDebug()<<"list ready";
QStringList list;
list=flick->list();
flkrcount=list.size();
for (int j=0;j<flkrcount;j++)
{
    collector->loadImage(list.at(j));
    //qDebug() << list.at(j);
}

}

void MainWindow::flkrAdd(QPixmap pix)
{
if (!pix.isNull())
{
qDebug()<<"Running append";
flickVec.append(pix);
flkrcount--;
}

if(flkrcount==0)
{
    qDebug()<<flickVec.size()<<"HERE";
    int i=boxImage->imageGrid->count();
    //Debug();
    AddCollectionCommand *command=new AddCollectionCommand("Add Flickr",flickVec,i,boxImage,timer);
    undoStack->push(command);
    addCollectionAction->setEnabled(true);
    addImageAction->setEnabled(true);
    //MBox.close();
}
}

void MainWindow::AddIntoBox()
{
    int i=boxImage->imageGrid->count();
    AddCollectionCommand * command=new AddCollectionCommand("Add Flickr",flickVec,i,boxImage,timer);
    undoStack->push(command);
}

void MainWindow::TimerStart()
{
timer->start(2000);
curLoc=0;
QToolButton *button = (QToolButton *) boxImage->buttonGroup->checkedButton();
int itemNum = boxImage->imageGrid->indexOf(button);
if (itemNum < 0)
{
  curLoc=0;
}
else
    curLoc=itemNum;
}

void MainWindow::TimerStop()
{
timer->stop();
}
void MainWindow::SlideShow()
{
    if (boxImage->imageGrid->isEmpty())
    {
        return;
    }
    QPixmap pixmap;
    QPixmap pixmap1;
    QLayoutItem *item;
    QIcon icon;

    item = boxImage->imageGrid->itemAt(curLoc);
    icon = (dynamic_cast<QToolButton *>(item->widget()))->icon();
    pixmap=icon.pixmap(150,150);
    pixmap1=pixmap.scaled(300,300,Qt::KeepAspectRatio);
    leftBut->setIcon(QIcon(pixmap1));
    leftBut->setIconSize(QSize(300,300));
    if (curLoc==boxImage->imageGrid->count()-1)
    {
        curLoc=0;
        qDebug()<< "Go back to start";
    }
    else
        curLoc++;


}

void MainWindow::Debug()
{
for (int i=0;i<10;i++)
{
    qDebug()<<flickVec.size()<<"HERE"<<"DEBUG"<<i;
QMessageBox mes;
mes.setIconPixmap(flickVec.at(i));
mes.exec();
}

}

void MainWindow::flkrfail()
{
    QMessageBox mes;
    mes.setText("Fail to load from Flickr");
    addCollectionAction->setEnabled(true);
    addImageAction->setEnabled(true);
    mes.exec();
}

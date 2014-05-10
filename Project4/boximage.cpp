

/* for solution to QDir::homePath() not working:
     http://stackoverflow.com/questions/14033720/qfiledialog-how-to-specify-home-directory
  */

#include <QtWidgets>
#include <QtDebug>
#include "boximage.h"
#include "deleteallcommand.h"
#include "deleteimagecommand.h"
#include "addcollectioncommand.h"
#include "addimagecommand.h"
#include "mainwindow.h"

const int IMAGE_SIZE = 144;
const int BUTTON_SIZE = 150;

BoxImage::BoxImage(QUndoStack *undoStack_incoming,QTimer* Intimer ,QWidget *parent)
  : QFrame(parent)
{
  timer=Intimer;
  undoStack = undoStack_incoming;
  imageGrid = new QGridLayout();
  this->setLayout(imageGrid);
  counter = 0;
  buttonGroup = new QButtonGroup();
  buttonGroup->setExclusive(true);
  for (int i=0;i<5;i++)
    {
      imageGrid->setColumnMinimumWidth(i,BUTTON_SIZE+8);
      imageGrid->setColumnStretch(i,0);
    }
  imageGrid->setOriginCorner(Qt::TopLeftCorner);
  imageGrid->setSpacing(0);
  nrows = 0;
}

/*************

  Slots

**************/



void BoxImage::deleteAllCallback()
{
  QVector<QPixmap> pixmapList;
  QLayoutItem *item;
  QIcon icon;

  for (int i=imageGrid->count()-1;i>=0;i--)
    {
      item = imageGrid->itemAt(i);
      icon = (dynamic_cast<QToolButton *>(item->widget()))->icon();
      pixmapList.append(icon.pixmap(IMAGE_SIZE,IMAGE_SIZE));
    }

  DeleteAllCommand *command = new DeleteAllCommand("delete all",
                           0, counter-1, this,timer);
  undoStack->push(command);
  command->setPixmapList(pixmapList);

}

void BoxImage::deleteImageCallback()
{
  QVector<QPixmap> pixmapList;
  QLayoutItem *item;
  QLayoutItem *item2;
  QIcon icon;

  QToolButton *button = (QToolButton *) buttonGroup->checkedButton();
  int itemNum = imageGrid->indexOf(button);
  if (itemNum < 0)
    return;

  //qDebug() << imageGrid->count();

  item = imageGrid->itemAt(itemNum);
  icon = (dynamic_cast<QToolButton *>(item->widget()))->icon();
  pixmapList.append(icon.pixmap(IMAGE_SIZE,IMAGE_SIZE));
  qDebug() << imageGrid->count();
  DeleteImageCommand *command = new DeleteImageCommand("delete image",
                               itemNum, itemNum, this,timer);
  undoStack->push(command);
  command->setPixmapList(pixmapList);
}


void BoxImage::addCollectionCallback()
{
  insertMultipleImagesCallback(imageGrid->count());
}

void BoxImage::addImageCallback()
{
  insertImageCallback(imageGrid->count());
}

void BoxImage::insertImageCallback()
{
  insertImageCallback(-1);
}


void BoxImage::insertMultipleImagesCallback(int insertLoc_incoming)
{
  int insertLoc = insertLoc_incoming;
  QVector<QPixmap> imageVector;
  if (insertLoc < 0)
    {
      QToolButton *button = (QToolButton *) buttonGroup->checkedButton();
      if (button == 0)
    {
      insertLoc = imageGrid->count();
    }
      int itemNum = imageGrid->indexOf(button);
      if (itemNum < 0)
    return;
      insertLoc = itemNum;
    }

  QString dirName =
    QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                      "/Users/ackerm",
                      QFileDialog::ShowDirsOnly
                      | QFileDialog::DontResolveSymlinks);
  if (dirName.isEmpty() || dirName.isNull())
    return;
  QDir *dir = new QDir(dirName);
  QStringList dirList = dir->entryList();
  for (int i=0;i<dirList.size();i++)
    {
      QString tmpStr2(dirName);
      tmpStr2.append("/").append(dirList.at(i));
      //qDebug() << "appending: " << tmpStr2;
      QPixmap newPixmap(tmpStr2);
      if (newPixmap.isNull())
    ;
      else
    imageVector.append(newPixmap);
    }

  AddCollectionCommand *command = new AddCollectionCommand("add collection",
                         imageVector,insertLoc,
                         this,timer);
  undoStack->push(command);
}

void BoxImage::insertImageCallback(int insertLoc)
{
  QVector<QPixmap> imageVector;
  QString fileName =
    QFileDialog::getOpenFileName(this, tr("Pick an Image"),
                                 QDir::homePath() + QDir::separator() + "*",
                 "Images (*.png *.jpg)");
  if (fileName.isEmpty() || fileName.isNull())
    return;
  QPixmap newPixmap(fileName);
  if (newPixmap.isNull())
    // error message here
    return;
  imageVector.append(newPixmap);

  AddImageCommand *command = new AddImageCommand("add image",
                         imageVector,insertLoc,
                         this,timer);
  undoStack->push(command);

}





/*****************

  functionality

******************/


void BoxImage::insertMultipleImages(QVector<QPixmap> pixmapList, int insertLoc)
{
  QLayoutItem *item;
  QLayoutItem *item2;
  QIcon icon;
  int row;
  qDebug() << "inserting ";
  if (pixmapList.count() == 0)
    return;

  // insertLoc is already set to the item *after* the selected label
  QToolButton *newLabel;
  if (insertLoc == counter)
    {
      int loc = 0;

      for (int i=insertLoc;i<insertLoc+pixmapList.count();i++)
    {
      newLabel = newToolButton(pixmapList.at(loc));
      //	  row = (int)(i/5);
      //	  imageGrid->setRowMinimumHeight(row,BUTTON_SIZE+8);
      //	  imageGrid->setRowStretch(row,0);
      imageGrid->addWidget(newLabel,(int)(i/5),i%5,Qt::AlignTop);
      loc++;
      qDebug() << "\tinserting pixmap at " << i;
    }
    }
  else // this is ugly, but there are 3 parts to the rearrangement
    {
      int currentEnd = imageGrid->count();
      for (int i=0, j=currentEnd;i<pixmapList.count();i++,j++)
    { // add to the end
      item = imageGrid->itemAt(j-1);
      icon = ((QToolButton *)item->widget())->icon();
      newLabel = newToolButton(pixmapList.at(i));
      newLabel->setIcon(QIcon(pixmapList.at(i).scaled(IMAGE_SIZE,IMAGE_SIZE,
                             Qt::KeepAspectRatio)));
      newLabel->setIconSize(QSize(IMAGE_SIZE,IMAGE_SIZE));
      imageGrid->addWidget(newLabel,(int)(j/5),j%5,Qt::AlignTop);
      //qDebug() << "\tmoving pixmaps from " << insertLoc+1 << " to " << j;
    }
      for (int i=currentEnd;i>insertLoc;i--)
    { // move items down
      item = imageGrid->itemAt(i-1);
      icon = ((QToolButton *)item->widget())->icon();
      item2 = imageGrid->itemAt(i);
      ((QToolButton *)item2->widget())->setIcon(icon);
      //qDebug() << "\tmoving pixmaps from " << i-1 << " to " << i;
    }
      for (int i=0;i<pixmapList.count();i++)
    { // insert the imageList
      item = imageGrid->itemAt(insertLoc+i);
      QWidget *widget = item->widget();
      QToolButton *button = dynamic_cast<QToolButton *>(widget);
      button->setIcon(QIcon(pixmapList.at(i).scaled(IMAGE_SIZE,IMAGE_SIZE,
                             Qt::KeepAspectRatio)));
      //qDebug() << "\tadding pixmap to " << insertLoc+1;
    }
    }
  counter=counter+pixmapList.count();
}

QToolButton *BoxImage::newToolButton(QPixmap pixmap_incoming)
{
  QToolButton *newLabel = new QToolButton();
  newLabel->setIcon(QIcon(pixmap_incoming.scaled(IMAGE_SIZE,IMAGE_SIZE,
                         Qt::KeepAspectRatio)));
  newLabel->setIconSize(QSize(IMAGE_SIZE,IMAGE_SIZE));
  newLabel->setMaximumSize(BUTTON_SIZE,BUTTON_SIZE);
  newLabel->setMinimumSize(BUTTON_SIZE,BUTTON_SIZE);
  newLabel->setCheckable(true);
  buttonGroup->addButton(newLabel);
  return newLabel;
}

void BoxImage::deleteImages(int begin, int end)
{
  QLayoutItem *item;
  QLayoutItem *item2;
  QIcon icon;
  QVector<QPixmap> pixmapList;
  int loc = begin;
  loc = imageGrid->count();
  int length = end-begin+1;

  //qDebug() << "deleting " << imageGrid->count() << " items";
  for (int i=end+1,j=begin;i<imageGrid->count();i++, j++)
      {
        item = imageGrid->itemAt(i);
    icon = ((QToolButton *)item->widget())->icon();
    item2 = imageGrid->itemAt(j);
    ((QToolButton *)item2->widget())->setIcon(icon);
    //qDebug() << "\tmoving pixmaps from " << i << " to " << j;
      }

  for (int i=0;i<length;i++)
    {
      int loc = imageGrid->count(); // for debugging
      item = imageGrid->takeAt(imageGrid->count()-1);
      icon = (dynamic_cast<QToolButton *>(item->widget()))->icon();
      pixmapList.append(icon.pixmap(IMAGE_SIZE,IMAGE_SIZE));
      delete item->widget();
      delete item;
      counter --;
      //qDebug() << "\tdeleting " << imageGrid->count()-1;
    }
}


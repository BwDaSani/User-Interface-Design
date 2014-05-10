#ifndef BOXIMAGE_H
#define BOXIMAGE_H

#include <QtWidgets>

class BoxImage : public QFrame
{
  Q_OBJECT

 public:
  explicit BoxImage(QUndoStack *undoStack_incoming, QTimer *Intimer, QWidget *parent = 0);
  void insertMultipleImages(QVector<QPixmap> pixmapVector, int loc);
  void deleteImages(int begin, int end);
  QButtonGroup *buttonGroup;
  QGridLayout *imageGrid;

 protected:
  int nrows;
  int counter;


  QUndoStack *undoStack;
  QClipboard *clip;
  void appendImage(QString fileName);
  void appendImage(QPixmap pixmap_incoming);
  QToolButton *newToolButton(QPixmap pixmap_incoming);
  void insertImageCallback(int insertLoc);
  //void insertImage(QString filename, int loc);
  QTimer *timer;


public slots:
  void addCollectionCallback();
  void addImageCallback();
  void deleteAllCallback();
  void deleteImageCallback();
  void insertImageCallback();
  void insertMultipleImagesCallback(int insertLoc);
  //void pasteCallback();
};

#endif // BOXIMAGE_H

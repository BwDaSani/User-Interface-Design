#include "mainwindow.h"
#include "imagecollector.h"
#include <QtGui>

/***************************************************************************

ImageCollector retrieves a single pixmap from the web based on a url.

This is designed to be a singleton class.

Mark Ackerman    February 2014

To use this:

1.  Create an ImageCollector instance.  You will probably need only one.

2.  Get an image by calling loadImage() with the appropriate url in a
    QString.  

3.  You can connect to the pixmapAvailable(pixmap) signal to get the 
    pixmap when the image is finished downloading.

*****************************************************************************/
/****************************************************************************
**
** Copyright (C) 2010, 2013, 2014 University of Michigan
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  
**
** Based on Qt example code.
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/


/*   create an instance of ImageCollector */
ImageCollector::ImageCollector()
{
  manager = new QNetworkAccessManager;
  connect(manager,SIGNAL(finished(QNetworkReply *)),
          this,SLOT(imageFinishedLoading(QNetworkReply *)));
}

/* Then use this to retrieve a single url.  You give it the url, and
   this creates a thread to download the file.  ImageCollector emits a
   signal, pixmapAvailable(pixmap) when it's done that has the correct
   pixmap as its parameter. */
void ImageCollector::loadImage(QString url_incoming)
{
  QNetworkRequest request;
  request.setUrl(QUrl(url_incoming));
  QNetworkReply *reply = manager->get(request);
}


void ImageCollector::imageFinishedLoading(QNetworkReply * reply)
{
  switch (reply->error())
    {
    case QNetworkReply::NoError:
      {
	QImage image; // on stack, no delete nec
	image.loadFromData(reply->readAll());
	QPixmap pixmap = QPixmap::fromImage(image); 
	// on stack, should be copied over automatically
	emit pixmapAvailable(pixmap);
	break;
      }
    default:
      qDebug() << "error in image load for " << reply->url().toString()
	       << ", error: " << reply->errorString();
    }
  reply->deleteLater();
}


ImageCollector::~ImageCollector()
{
  
}


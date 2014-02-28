/*
 * QGuidoWidget.cpp
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */
#include <QPainter>
#include <QPaintEvent>

#include <assert.h>
#include <QtDebug>

#include "QGuidoWidget.h"
#include "QGuidoPainter.h"
#include "QPageManager.h"

#define PIXMAP_CACHE_SIZE 100000
#include <QPixmapCache>
#include <QPixmap>

//-------------------------------------------------------------------------
QGuidoWidget::QGuidoWidget( QWidget *parent )
    : QWidget(parent)
{
	QPixmapCache::setCacheLimit( PIXMAP_CACHE_SIZE );

	mGuidoPainter = QGuidoPainter::createGuidoPainter();
	
	if ( !mGuidoPainter )
		// QGuidoPainter::createGuidoPainter() returned NULL. 
		// You forgot to call QGuidoPainter::startGuidoEngine.
		assert(0);

	mPageManager = new QPageManager();
}

//-------------------------------------------------------------------------
QGuidoWidget::~QGuidoWidget()
{
	QGuidoPainter::destroyGuidoPainter( mGuidoPainter );
}


//-------------------------------------------------------------------------
void QGuidoWidget::paintEvent(QPaintEvent * event)
{
	event->accept();

	QPixmap pixmap;
    
	if (!QPixmapCache::find(key(), pixmap)) 
	{
		pixmap = generatePixmap();
		QPixmapCache::insert(key(), pixmap);
	}
    
	QPainter painter(this);
	painter.drawPixmap( event->rect() , pixmap , event->rect() );
}

//-------------------------------------------------------------------------
QPixmap QGuidoWidget::generatePixmap()
{
	QPixmap pixmap( width(), height() );

	QRect drawRect(0,0,width(), height());
	QSizeF originalSize = mPageManager->totalSize();
	float xScale = drawRect.width() / originalSize.width();
	float yScale = drawRect.height() / originalSize.height();

	QPainter painter( &pixmap );

	painter.setRenderHint( QPainter::Antialiasing , true );
	
	painter.setBrush(QBrush(QColor(255,255,255)));
	painter.setPen(QPen(QColor(255,255,255)));
	painter.drawRect( drawRect );
	
	for ( int i = mPageManager->firstVisiblePage() ; i <= mPageManager->lastVisiblePage() ; i++ )
	{
		QRectF pageRect;
		QPointF scaledPoint = mPageManager->pagePos(i);
		scaledPoint.setX( scaledPoint.x() * xScale );
		scaledPoint.setY( scaledPoint.y() * yScale );
		QSizeF scaledSize = mPageManager->pageSize(i);
		scaledSize.setWidth( scaledSize.width() * xScale );
		scaledSize.setHeight( scaledSize.height() * yScale );
		pageRect.moveTo( scaledPoint );
		pageRect.setSize( scaledSize );
		mGuidoPainter->draw( &painter , i , pageRect.toRect() );
	}
	return pixmap;
}

//-------------------------------------------------------------------------
QString QGuidoWidget::key() const
{
	QString result;
	result.sprintf("%p", static_cast<const void *>(this));
	return result;
}

//-------------------------------------------------------------------------
void QGuidoWidget::resizeEvent(QResizeEvent *)
{
	QPixmapCache::remove(key());

	QSize s1 = QSize( width() , heightForWidth(width()) );
	float r = float( s1.width() ) / float( s1.height() );
	QSize s2( height() * r , height() );
	resize( ( s1.width() < s2.width() ) ? s1 : s2 );
}

//-------------------------------------------------------------------------
bool QGuidoWidget::setPage(int pageIndex)
{
	if ( pageIndex != firstVisiblePage() )
	{
		bool result = mPageManager->setPage(pageIndex);
		clearCache();
		return result;
	}
	else
		return true;
}

//-------------------------------------------------------------------------
int QGuidoWidget::pageCount() const
{
	return mGuidoPainter->pageCount();
}

//-------------------------------------------------------------------------
int QGuidoWidget::firstVisiblePage() const
{
	return mPageManager->firstVisiblePage();
}

//-------------------------------------------------------------------------
bool QGuidoWidget::setGMNFile(const QString& fileName)
{
	if ( mGuidoPainter->setGMNFile( fileName ) )
	{
		updateGuidoPagesSizes();
		clearCache();
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------
bool QGuidoWidget::setGMNCode(const QString& gmnCode, const QString& path)
{
	if ( mGuidoPainter->setGMNCode( gmnCode, path.size() ? path.toUtf8().data() : 0) )
	{
		updateGuidoPagesSizes();
		clearCache();
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------
void QGuidoWidget::setARHandler(ARHandler ar)
{ 
	if (ar) {
		mGuidoPainter->setARHandler(ar); 
		updateGuidoPagesSizes();
		clearCache();
	}
}

//-------------------------------------------------------------------------
bool QGuidoWidget::isGMNValid() const
{
	return mGuidoPainter->isGMNValid();
}

//-------------------------------------------------------------------------
QString QGuidoWidget::gmnCode() const
{
	return mGuidoPainter->gmnCode();
}

//-------------------------------------------------------------------------
int QGuidoWidget::heightForWidth ( int w ) const
{
	QSizeF s = sizeHint();
	return int( s.height() * float(w) / s.width() );
}

//-------------------------------------------------------------------------
QSizeF QGuidoWidget::pageSizeMM(int pageIndex) const
{
	return mGuidoPainter->pageSizeMM(pageIndex);
}

//-------------------------------------------------------------------------
QString QGuidoWidget::fileName() const
{
	return mGuidoPainter->fileName();
}

//-------------------------------------------------------------------------
void QGuidoWidget::setGridHeight( int height )
{
	mPageManager->setGridHeight( height );
	clearCache();
}

//-------------------------------------------------------------------------
void QGuidoWidget::setGridWidth( int width )
{
	mPageManager->setGridWidth( width );
	clearCache();
}

//-------------------------------------------------------------------------
int QGuidoWidget::lastVisiblePage() const
{
	return mPageManager->lastVisiblePage();
}

//-------------------------------------------------------------------------
int QGuidoWidget::gridHeight() const
{
	return mPageManager->gridHeight();
}

//-------------------------------------------------------------------------
int QGuidoWidget::gridWidth() const
{
	return mPageManager->gridWidth();
}

//-------------------------------------------------------------------------
QString QGuidoWidget::getLastErrorMessage() const
{
	return mGuidoPainter->getLastErrorMessage();
}

//-------------------------------------------------------------------------
void QGuidoWidget::getLastParseErrorLine(int &line, int &col) const
{
    mGuidoPainter->getLastParseErrorLine(line, col);
}

//-------------------------------------------------------------------------
void QGuidoWidget::setGuidoLayoutSettings(const GuidoLayoutSettings& layoutSettings)
{
	mGuidoPainter->setGuidoLayoutSettings( layoutSettings );
	updateGuidoPagesSizes();
	clearCache();
}

//-------------------------------------------------------------------------
GuidoLayoutSettings QGuidoWidget::guidoLayoutSettings() const
{
	return mGuidoPainter->guidoLayoutSettings();
}

//-------------------------------------------------------------------------
void QGuidoWidget::updateGuidoPagesSizes()
{
	QList<QSizeF> pages;
	for ( int i = 1 ; i <= pageCount() ; i++ )
		pages << pageSizeMM(i);

	mPageManager->setPages(pages);
}

//-------------------------------------------------------------------------
void QGuidoWidget::clearCache()
{
	updateGeometry();
	QPixmapCache::remove(key());
	update();
}

//-------------------------------------------------------------------------
void QGuidoWidget::setResizePageToMusic(bool isOn)
{
	if ( isOn != isResizePageToMusic() )
	{
		mGuidoPainter->setResizePageToMusic(isOn);
		updateGuidoPagesSizes();
		void clearCache();
	}
}

//-------------------------------------------------------------------------
bool QGuidoWidget::isResizePageToMusic() const
{ 
	return mGuidoPainter->isResizePageToMusic(); 
}

//-------------------------------------------------------------------------
void QGuidoWidget::setGuidoPageFormat(const GuidoPageFormat& pageFormat)
{
	mGuidoPainter->setGuidoPageFormat(pageFormat);
	updateGuidoPagesSizes();
	clearCache();
}

//-------------------------------------------------------------------------
GuidoPageFormat QGuidoWidget::guidoPageFormat() const
{
	return mGuidoPainter->guidoPageFormat();
}

//-------------------------------------------------------------------------
void QGuidoWidget::resetSystemsDistance()
{
	mGuidoPainter->resetSystemsDistance();
	updateGuidoPagesSizes();
	clearCache();
}

//-------------------------------------------------------------------------
void QGuidoWidget::setSystemsDistance(float distance)
{
	mGuidoPainter->setSystemsDistance(distance);
	updateGuidoPagesSizes();
	clearCache();
}

//-------------------------------------------------------------------------
float QGuidoWidget::getSystemsDistance() const
{
	return mGuidoPainter->getSystemsDistance();
}

//-------------------------------------------------------------------------
QSize QGuidoWidget::sizeHint() const
{
	return mPageManager->totalSize().toSize();
}

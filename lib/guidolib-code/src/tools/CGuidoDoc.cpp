
/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

// ===========================================================================
//	CGuidoDoc.cpp	
// ===========================================================================

#include "CGuidoDoc.h"

// ---------------------------------------------------------------------------
//		* CGuidoDoc
// ---------------------------------------------------------------------------			
CGuidoDoc::CGuidoDoc()
{
	mGR = 0;
	mAR = 0;
	mZoom = 1;
	mZoomFactor = float(5) / float(4);
	mPageNum = 1;
	ClearSettings();
}

// ---------------------------------------------------------------------------
//		~CGuidoDoc
// ---------------------------------------------------------------------------			
CGuidoDoc::~CGuidoDoc()
{
	FreeARGR();
}


// ---------------------------------------------------------------------------

//		ParseFileARGR

// ---------------------------------------------------------------------------			

GuidoErrCode 

CGuidoDoc::ParseFileARGR( const char * inFilePath ) 

{

	GuidoLayoutSettings * settings = 0; // GetSettings(); TODO: default settings



	mAR = 0;

	mGR = 0;



	GuidoErrCode err = GuidoParseFile( inFilePath, &mAR );



	if( err == guidoNoErr )

		err = GuidoAR2GR( mAR, settings, &mGR );

	else

		FreeARGR();

	

	return err;

}



// ---------------------------------------------------------------------------

//		ParseFileARGR

// ---------------------------------------------------------------------------			

GuidoErrCode 

CGuidoDoc::ParseStringARGR( const char * str ) 

{

	GuidoLayoutSettings * settings = 0; // GetSettings(); TODO: default settings

	mAR = 0;

	mGR = 0;

	GuidoErrCode err = GuidoParseString( str, &mAR );



	if( err == guidoNoErr )

		err = GuidoAR2GR( mAR, settings, &mGR );

	else

		FreeARGR();

	return err;

}


// ---------------------------------------------------------------------------
//		AR2GR
// ---------------------------------------------------------------------------			
// - Builds the GR from the AR, using current settings
GuidoErrCode
CGuidoDoc::AR2GR()
{
	if( mAR == 0 ) return guidoErrInvalidHandle;
	
	if( mGR != 0 )
		GuidoFreeGR( mGR ), mGR = 0;

	GuidoLayoutSettings * settings = GetSettings();

	return GuidoAR2GR( mAR, settings, &mGR );
}

// ---------------------------------------------------------------------------
//		UpdateGR
// ---------------------------------------------------------------------------			
GuidoErrCode
CGuidoDoc::UpdateGR()
{
	GuidoLayoutSettings * settings = GetSettings();
	return GuidoUpdateGR( GetGR(), settings );
}

// ---------------------------------------------------------------------------
//		FreeARGR
// ---------------------------------------------------------------------------			
void
CGuidoDoc::FreeARGR()
{
	GuidoFreeGR( mGR );
	GuidoFreeAR( mAR );

	mGR = 0;
	mAR = 0;
}

// ---------------------------------------------------------------------------
//		GetSettings
// ---------------------------------------------------------------------------			
GuidoLayoutSettings *
CGuidoDoc::GetSettings()
{
	return &mGuidoSettings;
}

// ---------------------------------------------------------------------------
//		SetSettings
// ---------------------------------------------------------------------------			
void CGuidoDoc::SetSettings(GuidoLayoutSettings * setting)
{
	mGuidoSettings.systemsDistance = setting->systemsDistance;
 	mGuidoSettings.systemsDistribution = setting->systemsDistribution;
	mGuidoSettings.systemsDistribLimit = setting->systemsDistribLimit;
	mGuidoSettings.force = setting->force;
    mGuidoSettings.spring = setting->spring;
	mGuidoSettings.neighborhoodSpacing = setting->neighborhoodSpacing;
 	mGuidoSettings.optimalPageFill = setting->optimalPageFill;
}

// ---------------------------------------------------------------------------
//		ClearSettings
// ---------------------------------------------------------------------------			
void
CGuidoDoc::ClearSettings()
{
	GuidoGetDefaultLayoutSettings( &mGuidoSettings );
}

// ---------------------------------------------------------------------------
//		HandleError
// ---------------------------------------------------------------------------			
void
CGuidoDoc::HandleError( GuidoErrCode inCode )
{
	mLastError = inCode;
	mParseErrorLine = GuidoGetParseErrorLine();
}

// ---------------------------------------------------------------------------
//		FormatsAreEqual
// ---------------------------------------------------------------------------			
// Return true if the two page formats are equivalent.
bool 
CGuidoDoc::FormatsAreEqual( const GuidoPageFormat & a, const GuidoPageFormat & b ) 
{
	return (a.width == b.width
		&&  a.height == b.height
		&&  a.marginleft == b.marginleft
		&&  a.margintop == b.margintop
		&&  a.marginright == b.marginright
		&&  a.marginbottom == b.marginbottom);
}

// ---------------------------------------------------------------------------
//		SetPageNum
// ---------------------------------------------------------------------------			
bool 
CGuidoDoc::SetPageNum( int in )
{
	int maxPage = PageCount();
	if( maxPage < 1 )
		maxPage = 1;

	if( in > maxPage )			in = maxPage;	
	else if( in < 1 )			in = 1;

	if( in == mPageNum )
		return false;
	
	mPageNum = in;
	return true;
}

// ---------------------------------------------------------------------------
//		ZoomIn
// ---------------------------------------------------------------------------			
void 
CGuidoDoc::ZoomIn()
{
	SetZoom( GetZoom() * mZoomFactor );
}

// ---------------------------------------------------------------------------
//		ZoomOut
// ---------------------------------------------------------------------------			
void 
CGuidoDoc::ZoomOut()
{
	SetZoom( GetZoom() / mZoomFactor );
}

// ---------------------------------------------------------------------------
//		SetZoom
// ---------------------------------------------------------------------------			
void 
CGuidoDoc::SetZoom( float in )
{
	if( in <= 0.001f )	// min zoom: 1 / 1000
		in = 0.001f;
	else if( in > 1000 )	// max zoom: 1000
		in = 1000;
	else if( in >= 0.99f && in <= 1.01f )	// normalize
		in = 1;

	mZoom = in;
}


/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMusicalObject.h" // for DURATIONs
#include "ARNote.h" // To get noteFormat parameters
#include "GRStem.h"
#include "GRGlobalStem.h"
#include "GREvent.h"
#include "GRMusic.h" 
#include "VGDevice.h" 
#include "secureio.h"
#include "GRSingleNote.h"
#include "GRStdNoteHead.h"

#include "GUIDOEngine.h"	// for AddGGSOutput

NVPoint GRStem::sRefpos;

GRStem::GRStem(GRGlobalStem * gstem) : mColRef(NULL), fOffsetStartPosition(0), fDrawActivated(true),
    fNoteHeadType(kFullHeadSymbol), fHeadOrientation(ARTHead::NOTSET), fLastHeadOrientation(ARTHead::NOTSET)
{
	mStemDir = dirOFF;
	
	setColRef(gstem->getColRef());
	
    mOffset = gstem->getOffset();
	mSize   = gstem->getSize();

	sRefpos.x = -30;		// HARDCODED !!!
}

GRStem::GRStem( GREvent * sngnot,
	const TYPE_DURATION & durtempl,
	GDirection dir,
	float length,
	float notebreite) :
	mColRef(NULL),
	fOffsetStartPosition(0),
	fDrawActivated(true),
    fNoteHeadType(kFullHeadSymbol),
    fHeadOrientation(ARTHead::NOTSET),
    fLastHeadOrientation(ARTHead::NOTSET)
    
{
	if (durtempl >= DURATION_1)
	{
		// mStemLen = 0;
		mStemDir = dirOFF;
		return; 
	}
	mStemDir = dir;

	mStemLen = length;

	setColRef(sngnot->getColRef());

	mOffset = sngnot->getOffset();

	mSize = sngnot->getSize();

	// dependant on the direction, I have different
	// types (do I?)

	mLeftSpace = 0;
	mRightSpace = 0;

	if (dir == dirUP)
		mBoundingBox.top = (GCoord)(-mStemLen);
	else if (dir == dirDOWN)
		mBoundingBox.bottom = (GCoord)(mStemLen);
	sRefpos.x = (GCoord)(- notebreite * 0.5f);

	GRSingleNote *singleNote = dynamic_cast<GRSingleNote *>(sngnot);
	if (singleNote)
	{
        fHeadOrientation = singleNote->getHeadState();

		GRStdNoteHead *noteHead = singleNote->getNoteHead();

		if (noteHead)
		{
			noteHead->setGlobalStemDirection(dir);

			fNoteHeadType = noteHead->getSymbol();

			//ConstMusicalSymbolID noteHeadSymbolTmp = noteHead->getSymbol();
			// - Adjust stem length if it's a cross notehead
			if (fNoteHeadType == kFullXHeadSymbol)
			{
				setFirstSegmentDrawingState(false);

				if (dir == dirUP)
					setOffsetStartPosition(4);
				else if (dir == dirDOWN)
					setOffsetStartPosition(-4);
			}
            // - Adjust stem length if it's a triangle notehead
			else if (fNoteHeadType == kFullTriangleHeadSymbol || fNoteHeadType == kHalfTriangleHeadSymbol)
			{
				if (dir == dirUP)
					setOffsetStartPosition(47);
				else if (dir == dirDOWN)
					setFirstSegmentDrawingState(false);
			}
            // - Adjust stem length if it's a reversed triangle notehead
			else if (fNoteHeadType == kFullReversedTriangleHeadSymbol || fNoteHeadType == kHalfReversedTriangleHeadSymbol)
			{
				if (dir == dirUP)
					setFirstSegmentDrawingState(false);
				else if (dir == dirDOWN)
					setOffsetStartPosition(-47);
			}
		}
	}
}

GRStem::~GRStem()
{
	delete [] mColRef;
}

void 
GRStem::setStemLength(float inLen)
{
	mStemLen = inLen;
	mBoundingBox.top = 0;
	mBoundingBox.bottom = 0;
	if (mStemDir == dirUP)
		mBoundingBox.top = (GCoord)(-mStemLen);
	else if (mStemDir == dirDOWN)
		mBoundingBox.bottom = (GCoord)(mStemLen);
}
	
void 
GRStem::setStemDir(GDirection dir)
{
	mStemDir = dir;
	mBoundingBox.top = 0;
	mBoundingBox.bottom = 0;
	if (mStemDir == dirUP)
		mBoundingBox.top = (GCoord)(- mStemLen);
	else if (mStemDir == dirDOWN)
		mBoundingBox.bottom = (GCoord)(mStemLen);
}

// TODO: draw only one scaled symbol
void GRStem::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw)
		return;
	if (mStemDir == dirOFF)
        return;
	if (mSize < kMinNoteSize)
        return;			// size is too small, don't draw

	// - Setup colors
	const unsigned char * colref = getColRef();
	const VGColor prevTextColor = hdc.GetFontColor();
  	if (colref) hdc.SetFontColor( VGColor( colref ));

	// - Setup text align 
	hdc.SetFontAlign(getTextAlign());

	// - Draw
	// dependant on the direction, we do different things ...

	unsigned int tmpSymbol;
	
	const float spaceBySize = LSPACE * mSize;
	const float halfSpaceBySize = 0.5f * spaceBySize;

	if (mStemDir == dirUP)
	{
		tmpSymbol =  kStemUp1Symbol;

        float stemOffsetY = 0;

		if (fDrawActivated && (fNoteHeadType == kFullHeadSymbol || fNoteHeadType == kHalfNoteHeadSymbol))
        {
            if ((fHeadOrientation == ARTHead::NOTSET && fLastHeadOrientation == ARTHead::NOTSET) || fHeadOrientation != ARTHead::NOTSET)
            {
                if (fHeadOrientation == ARTHead::REVERSE || fHeadOrientation == ARTHead::RIGHT)
                    stemOffsetY = spaceBySize / 6;
                else     
                    stemOffsetY = - spaceBySize / 6;
            }
            else
            {
                if (fLastHeadOrientation == ARTHead::REVERSE || fLastHeadOrientation == ARTHead::RIGHT)
                    stemOffsetY = spaceBySize / 6;
                else     
                    stemOffsetY = - spaceBySize / 6;
            }
        }

        if (fDrawActivated)
            GRNotationElement::DrawSymbol(hdc, tmpSymbol, 0, stemOffsetY);
		
		tmpSymbol = kStemUp2Symbol;  // wrong in EPS font...

		// Draws until the length has been completed ...
		float offsy = - halfSpaceBySize + fOffsetStartPosition;

		while (- offsy < mStemLen) // * mSize)
		{
			if ((spaceBySize - offsy ) > mStemLen) // * mSize)
			{
				offsy = - mStemLen /* * mSize)*/ + spaceBySize;
			
				GRNotationElement::DrawSymbol( hdc, tmpSymbol, 0, offsy);
				break;
			}
			GRNotationElement::DrawSymbol( hdc, tmpSymbol, 0, offsy );
			offsy -= halfSpaceBySize;
		}
	}
	else if (mStemDir == dirDOWN)
	{
		tmpSymbol = kStemDown1Symbol;

        float stemOffsetY = 0;

		if (fDrawActivated && (fNoteHeadType == kFullHeadSymbol || fNoteHeadType == kHalfNoteHeadSymbol))
        {
            if ((fHeadOrientation == ARTHead::NOTSET && fLastHeadOrientation == ARTHead::NOTSET) || fHeadOrientation != ARTHead::NOTSET)
            {
                if (fHeadOrientation == ARTHead::REVERSE || fHeadOrientation == ARTHead::LEFT)
                    stemOffsetY = - spaceBySize / 6;
                else     
                    stemOffsetY = spaceBySize / 6;
            }
            else
            {
                if (fLastHeadOrientation == ARTHead::REVERSE || fLastHeadOrientation == ARTHead::LEFT)
                    stemOffsetY = - spaceBySize / 6;
                else     
                    stemOffsetY = spaceBySize / 6;
            }
        }

        if (fDrawActivated)
            GRNotationElement::DrawSymbol(hdc, tmpSymbol, 0, stemOffsetY);
		
		tmpSymbol = kStemDown2Symbol;
		
		// Draws until the length has been completed ...		
		float offsy = halfSpaceBySize + fOffsetStartPosition;

		while( offsy < mStemLen ) // * mSize)
		{
			if(( offsy + spaceBySize ) > mStemLen ) 
			{
				offsy = mStemLen - spaceBySize;
				GRNotationElement::DrawSymbol( hdc, tmpSymbol, 0, offsy );
				break;
			}
			GRNotationElement::DrawSymbol( hdc, tmpSymbol, 0, offsy );
			offsy += halfSpaceBySize;
		}
	}

	// - Restore context
	if (colref) hdc.SetFontColor( prevTextColor );  //(TODO: in a parent method)
}

void GRStem::GGSOutput() const
{
	if (mStemDir == dirOFF) return;

	/* this has to be changed so the correct
	   GGS-String is produced ... */

	char buffer[200];
	if (mStemDir == dirUP)
	{
		snprintf(buffer, 200, "\\draw_stem<%ld,%d,%d,%d>\n",
			getID(),
			(int)(mPosition.x + mOffset.x + (GCoord)(sRefpos.x * mSize) 
				+ (GCoord) (60 * mSize)+ ggsoffsetx) , // HARDCODED 60 is the size of the notehead ...
			(int)(mPosition.y + mOffset.y + (GCoord)(sRefpos.y * mSize) + ggsoffsety), 
			(int)mStemLen);

		AddGGSOutput(buffer);
	}
	else if (mStemDir == dirDOWN)
	{
		snprintf(buffer, 200, "\\draw_stem<%ld,%d,%d,%d>\n",
			getID(),
			(int)(mPosition.x + mOffset.x + (GCoord)(sRefpos.x * mSize) + ggsoffsetx),
			(int)(mPosition.y + mOffset.y + (GCoord)(sRefpos.y * mSize) + ggsoffsety),
			(int)(-mStemLen));

		AddGGSOutput(buffer);
	}
}

const NVPoint & GRStem::getReferencePosition() const
{
	return sRefpos;
}

void GRStem::setColRef( const unsigned char * inColor )
{
	if( inColor == 0 ) return;
	
	if (mColRef == 0)
		mColRef = new unsigned char [4];

	mColRef[0] = inColor[0];
	mColRef[1] = inColor[1];
	mColRef[2] = inColor[2];
	mColRef[3] = inColor[3];
}
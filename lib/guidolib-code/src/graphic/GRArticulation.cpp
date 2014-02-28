/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

	-- Articulations of duration ---

		Never combined, with a rare exception for the tenuto.

		- (TODO) Staccatissimo:( v ) aka Dash (no musical symbol in font?)
			Placement: next free space from the notehead.
					can be inside the staff.
			Height: less than a dent. 
			Refpoint: TODO.
			Tag:  \staccmo (advanced guido draft)

		- Staccato: ( . ) 
			Placement: next free space from the notehead.
					can be inside the staff. Must be placed before
					other articulations.
			Height: a quarter of a dent. 
			Refpoint: centered on the symbol. 
			Tag: \stacc
			
		- Pizzicato: (left-hand, snap, pizz, buzz)
			Placement: above the staff, except when parts share a staff.
			Tag : \pizz
						

		- Tenuto:  ( - ) aka Sostenuto or Stress
			Placement: next free space from the notehead.
					can be inside the staff.
			Height: thin: about 1/8 of a dent.
			Refpoint: centered. 
			Tag: \ten

	-- Articulations of force  ---

		Never combined;

		- Accent:   ( > ) 
			Height: one dent
			Placement: outside the staff prefered.
			Refpoint:  centered. 
			Tag: \accent

		- Marcato:  ( ^ ) aka Sforzando
			Height: one dent and half
			Placement: head side prefered. outside the staff prefered.
					based on a line (then it looks like a pyramide)
			Refpoint: centered, 1/4 of the height 
			Tag: \marcato

    -- Others sign handled as articulations --

		- Fermata: (dot in arc)			 
			Placement: above the staff, except when parts share a staff.
						Must be placed outside (after) articulations ans slurs
			Height: a dent and a quarter.
			Refpoint: dot.
			Tag: \fermata
			
		- Short fermata : (dot in ^)
			Placement: above the staff, except when parts share a staff.
						Must be placed outside (after) articulations ans slurs
			Height: a dent and a quarter.
			Refpoint: dot.
			Tag: \shortFermata

		( no longer used here: Breath-mark ( , ) 
			Placement: above the staff, just before the next note or barline.
			Refpoint: middle bottom.
			Tag: \breathMark	)		


	--- Rules:

		(1) Symbols must not cross staff lines (excepts the marcato)
		(2) Symbols are always centered on the note head. A staccato alone on 
			the stem side is centered on this stem.
		(3) Combined articulations must be kept on the same side of the note.
		(4) When combined, the staccato is always centered on the note head, 
			even if it's on the stem size.

	-----

*/

#include <typeinfo>
#include <cassert>

#include "ARStaccato.h"
#include "ARMarcato.h"
#include "ARAccent.h"
#include "ARTenuto.h"
#include "ARFermata.h"
#include "ARBreathMark.h"
#include "ARPizz.h"
#include "ARHarmonic.h"

#include "GRArticulation.h"
#include "GRSingleNote.h"
#include "GRDefine.h"
#include "GRStaff.h"

#include "VGDevice.h" // debug only

using namespace std;

NVPoint GRArticulation::sRefposStacc;
NVPoint GRArticulation::sRefposStaccmoUp;
NVPoint GRArticulation::sRefposStaccmoDown;
NVPoint GRArticulation::sRefposLeftHPizz;
NVPoint GRArticulation::sRefposSnapPizz;
NVPoint GRArticulation::sRefposBuzzPizz;
NVPoint GRArticulation::sRefposFingernailPizz;
NVPoint GRArticulation::sRefposAccent;
NVPoint GRArticulation::sRefposMarcatoUp;
NVPoint GRArticulation::sRefposMarcatoDown;
NVPoint GRArticulation::sRefposTenuto;
NVPoint GRArticulation::sRefposFermataUp;
NVPoint	GRArticulation::sRefposFermataDown;
NVPoint GRArticulation::sRefposShortFermataUp;
NVPoint GRArticulation::sRefposShortFermataDown;
NVPoint GRArticulation::sRefposLongFermataUp;
NVPoint GRArticulation::sRefposLongFermataDown;
NVPoint GRArticulation::sRefposHarmonic;
NVPoint GRArticulation::sRefposBreathMark;

GRArticulation::GRArticulation(ARMusicalTag * inTag, float curLSPACE, bool ownsar)
				: GRTagARNotationElement(inTag, curLSPACE, ownsar)	// , tinfo(typeid(*mtag))
{
	// assert(inTag);
	const std::type_info & tinfo = typeid(*inTag);
	if (tinfo == typeid(ARStaccato))	
	{
		if ( ((ARStaccato *) inTag)->getType() == ARStaccato::HEAVY )
			setupStaccmo();
		else setupStaccato();
	}
	else if (tinfo == typeid(ARAccent))			setupAccent();
	else if (tinfo == typeid(ARMarcato))
    {
        if ( ((ARMarcato *) inTag)->getPositionMarcato() == ARMarcato::BELOW )
		{
			setupMarcatoDown();
		}
		else if ( ((ARMarcato *) inTag)->getPositionMarcato() == ARMarcato::ABOVE )
		{
			setupMarcatoUp();
		}
        else
            setupMarcato();
    }
	else if (tinfo == typeid(ARTenuto))			setupTenuto();
	else if (tinfo == typeid(ARFermata))		
	{
		if ( ((ARFermata *) inTag)->getPositionFermata() == ARFermata::BELOW )
		{
			if ( ((ARFermata *) inTag)->getType() == ARFermata::REGULAR )
				setupFermataDown();
			else if ( ((ARFermata *) inTag)->getType() == ARFermata::SHORT)
				setupShortFermataDown(); 
			else setupLongFermataDown(); 
		}
		else // above the note
		{
			if ( ((ARFermata *) inTag)->getType() == ARFermata::REGULAR )
				setupFermataUp();
			else if ( ((ARFermata *) inTag)->getType() == ARFermata::SHORT)	
				setupShortFermataUp();
			else setupLongFermataUp();
		}
	}
	else if (tinfo == typeid(ARBreathMark))	 	setupBreathMark();
	else if (tinfo == typeid(ARHarmonic))		setupHarmonic();
	else if (tinfo == typeid(ARPizz))			
	{
		if ( ((ARPizz *) inTag)->getType() == ARPizz::SNAP )
			setupSnapPizz();
		else if ( ((ARPizz *) inTag)->getType() == ARPizz::BUZZ)
			setupBuzzPizz();
		else if ( ((ARPizz *) inTag)->getType() == ARPizz::FINGERNAIL)
			setupFingernailPizz();
		else setupLeftHPizz();
	}
	else setupStaccato();
}

GRArticulation::~GRArticulation()
{
	// associatons are handled in destructor
	// of GRNotationElement
}

/*----------------Setup fonctions-------------------------------------------------*/

void GRArticulation::setupStaccato()
{
	setArticulationSymbol( kStaccatoSymbol );
	const float height = LSPACE * float(0.38);		
	sRefposStacc = NVPoint(-mLeftSpace, 0 );		
	mBoundingBox.Set( -mLeftSpace, -height*0.5f, mRightSpace, 0.5f*height );
	mArticulationFlag = kFlagStaccato;
}

void GRArticulation::setupStaccmo()
{
	setArticulationSymbol( kStaccmoUpSymbol);
	mArticulationFlag = kFlagStaccmo;
	
	//setStaccmoDirection(true); // useful ?
}

void GRArticulation::setupLeftHPizz()
{
	setArticulationSymbol(kLeftHPizzSymbol);
	const float height = LSPACE*float(1.2);		
	sRefposLeftHPizz = NVPoint(-mLeftSpace, 0);	
	mBoundingBox.Set(-mLeftSpace, height*float(0.5), mRightSpace, -height*float(0.5)); 
	mArticulationFlag = kFlagPizz;
}

void GRArticulation::setupSnapPizz()
{
	setArticulationSymbol(kSnapPizzSymbol);
	const float height = LSPACE * float(1.17);
	sRefposSnapPizz = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, height*float(0.29), mRightSpace, -height*float(0.71));
	mArticulationFlag = kFlagPizz;
}

void GRArticulation::setupBuzzPizz()
{
	setArticulationSymbol(kBuzzPizzSymbol);
	const float height = LSPACE * float(0.68);
	sRefposBuzzPizz = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace - float(0.72)*height, height*float(0.5), mRightSpace, -height*float(0.5));
	mArticulationFlag = kFlagPizz;
}

void GRArticulation::setupFingernailPizz()
{
	setArticulationSymbol(kFingernailPizzSymbol);
	const float height = LSPACE * float (0.9);
	sRefposFingernailPizz = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, 0, mRightSpace, -height);
	mArticulationFlag = kFlagPizz;
}

void GRArticulation::setupAccent()
{
	setArticulationSymbol( kAccentSymbol );
	const float height = LSPACE * float(1);
	sRefposAccent = NVPoint(-mLeftSpace, height * float(0.5));		
	mBoundingBox.Set( -mLeftSpace, -height * float(0.5), mRightSpace, height * float(0.5));
	mArticulationFlag = kFlagAccent;
}

void GRArticulation::setupMarcato()
{
	mArticulationFlag = kFlagMarcato;
	setArticulationSymbol( kMarcatoUpSymbol );

	setMarcatoDirection( true );
}

void GRArticulation::setupMarcatoUp()
{
    mArticulationFlag = kFlagMarcatoUp;
	setArticulationSymbol( kMarcatoUpSymbol );

	setMarcatoDirection( true );
}

void GRArticulation::setupMarcatoDown()
{
	mArticulationFlag = kFlagMarcatoDown;
	setArticulationSymbol( kMarcatoDownSymbol );

	setMarcatoDirection( false );
}

void GRArticulation::setupTenuto()
{
	setArticulationSymbol( kTenutoSymbol );
	const float height = LSPACE * float(0.15);
	sRefposTenuto = NVPoint(-mLeftSpace,0);		
	mBoundingBox.Set( -mLeftSpace, -height*float(0.5), mRightSpace, height*float(0.5) );
	mArticulationFlag = kFlagTenuto;
}

void GRArticulation::setupFermataUp()
{
	setArticulationSymbol( kFermataUpSymbol );
	const float height = LSPACE * float(1.33);
	sRefposFermataUp = NVPoint(-mLeftSpace, -0.3f*LSPACE);	//reference = center of the dot
	mBoundingBox.Set( -mLeftSpace, -height-0.3f*LSPACE, mRightSpace, -0.3f*LSPACE );
	mArticulationFlag = kFlagFermataUp;
}

void GRArticulation::setupFermataDown()
{
	setArticulationSymbol(kFermataDownSymbol);	
	const float height = LSPACE * float(1.33);
	sRefposFermataDown = NVPoint (-mLeftSpace,-0.2f*LSPACE);		//reference = center of dot
	mBoundingBox.Set(-mLeftSpace, height-0.2f*LSPACE, mRightSpace, -0.2f*LSPACE);
	mArticulationFlag = kFlagFermataDown;
}

void GRArticulation::setupShortFermataUp()
{
	setArticulationSymbol( kShortFermataUpSymbol );
	const float height = LSPACE * 1.66f;
	sRefposShortFermataUp = NVPoint(-mLeftSpace, -0.34f*LSPACE);	//reference = center of dot
	mBoundingBox.Set(-mLeftSpace, -height-0.34f*LSPACE, mRightSpace, -0.34f*LSPACE);
	mArticulationFlag = kFlagFermataUp;
}

void GRArticulation::setupShortFermataDown()
{
	setArticulationSymbol(kShortFermataDownSymbol);
	const float height = LSPACE * 1.66f;
	sRefposShortFermataDown = NVPoint(-mLeftSpace, float(1.5)*LSPACE); //reference = center of dot
	mBoundingBox.Set(-mLeftSpace, 1.5f*LSPACE-height, mRightSpace, 1.5f*LSPACE);
	mArticulationFlag = kFlagFermataDown;
}

void GRArticulation::setupLongFermataUp()
{
	setArticulationSymbol(kLongFermataUpSymbol);
	const float height = LSPACE * 1.33f;
	sRefposLongFermataUp = NVPoint(-mLeftSpace, -0.2f*LSPACE);
	mBoundingBox.Set(-mLeftSpace, -0.2f*LSPACE-height, mRightSpace, -0.2f*LSPACE);
	mArticulationFlag = kFlagFermataUp;
}

void GRArticulation::setupLongFermataDown()
{
	setArticulationSymbol(kLongFermataDownSymbol);
	const float height = LSPACE * float(1.33);
	sRefposLongFermataDown = NVPoint(-mLeftSpace, float(1)*LSPACE);
	mBoundingBox.Set(-mLeftSpace, 1*LSPACE-height, mRightSpace, 1*LSPACE);
	mArticulationFlag = kFlagFermataDown;
}

void GRArticulation::setupHarmonic()
{
	setArticulationSymbol (kHarmonicSymbol);
	const float height = LSPACE * float(0.5);
	sRefposHarmonic = NVPoint(-mLeftSpace, -LSPACE*float(0.5));
	mBoundingBox.Set(-mLeftSpace, -1.5f*height, mRightSpace, -height/2); //TODO !
	mArticulationFlag = kFlagHarmonic;
}

void GRArticulation::setupBreathMark()
{
	setArticulationSymbol( kBreathMarkSymbol );
	const float height = LSPACE * 1.33f;
	sRefposBreathMark = NVPoint(-mLeftSpace, -height * float(0.75));	
	mBoundingBox.Set( -mLeftSpace, -height, mRightSpace, 0 );
	mArticulationFlag = kFlagBreathMark;
}

void GRArticulation::setArticulationSymbol( unsigned int inSymbolID )
{
	mSymbol = inSymbolID;
	const float extent = GetSymbolExtent( inSymbolID );
	mLeftSpace = (extent * 0.5f);
	mRightSpace = (extent * 0.5f);
}

void GRArticulation::setPosition(const NVPoint & point)
{
	GRARNotationElement::setPosition(point);
}

void GRArticulation::setMarcatoDirection( bool upward )
{
	const float height = LSPACE * float(1.66);
	if( upward )
	{
		mSymbol = kMarcatoUpSymbol;
		sRefposMarcatoUp = NVPoint(-mLeftSpace, height * float(0.33));	// up		
		mBoundingBox.Set( -mLeftSpace, - height * float(0.66), mRightSpace, height * float(0.33)); // up
	}
	else
	{
		mSymbol = kMarcatoDownSymbol;
		sRefposMarcatoDown = NVPoint(-mLeftSpace, height * float(0.70));	// down
		mBoundingBox.Set( -mLeftSpace, - height * float(0.30), mRightSpace, height * float(0.70)); // down
	}
}

void GRArticulation::setStaccmoDirection( bool upward )
{
	const float height = LSPACE * float(0.64);
	if( upward )
	{
		mSymbol = kStaccmoUpSymbol;
		sRefposStaccmoUp = NVPoint(-mLeftSpace, -LSPACE*0.5f);	// up		
		mBoundingBox.Set( -mLeftSpace, - height-0.5f*LSPACE, mRightSpace, -LSPACE*0.5f); // up
	}
	else
	{
		mSymbol = kStaccmoDownSymbol;
		sRefposStaccmoDown = NVPoint(-mLeftSpace, 0);	// down
		mBoundingBox.Set( -mLeftSpace, 0, mRightSpace, height); // down
	}
}

// ----------------------------------------------------------------------------
void GRArticulation::tellPosition(GObject * caller, const NVPoint & inPos)	// Called by GREvent when event position
{																			// is set
	GREvent * ev = GREvent::cast(caller);
	if( ev == 0 )
		return;

//	GRStaff * staff = ev->getGRStaff();
//	const float currLSpace = staff->getStaffLSPACE();
//	const float halfSpace = float(0.5) * currLSpace;
//	const float linesCount = staff->getNumlines();

	NVPoint newPoint (inPos);

	switch( mArticulationFlag )
	{
		case kFlagStaccato:		placeStaccato( ev, newPoint );		break;
		case kFlagStaccmo:		placeStaccmo( ev, newPoint);		break;
		case kFlagAccent:		placeAccent( ev, newPoint );		break;
		case kFlagMarcato:		placeMarcato( ev, newPoint );		break;
        case kFlagMarcatoUp:	placeMarcatoUp( ev, newPoint );		break;
        case kFlagMarcatoDown:	placeMarcatoDown( ev, newPoint );	break;
		case kFlagTenuto:		placeTenuto( ev, newPoint );		break;
		case kFlagFermataUp:	placeFermataUp( ev, newPoint );		break;
		case kFlagFermataDown:	placeFermataDown(ev, newPoint );	break;
		case kFlagBreathMark:	placeBreathMark( ev, newPoint );	break;
		case kFlagPizz :		placePizz (ev, newPoint);			break;
		case kFlagHarmonic :	placeHarmonic(ev, newPoint);		break;
	}

	setPosition( newPoint );

	// - TEST, Update bounding box, but only if we're not a breath-mark
	if( mArticulationFlag != kFlagBreathMark )
	{
		ev->addToBoundingBox( this );
		ev->updateBoundingBox();
	}
}

// ----------------------------------------------------------------------------
// The staccato has the highest priority. It is placed first, close to the note.
void GRArticulation::placeStaccato( GREvent * inParent, NVPoint & ioPos )
{
	const int dir = chooseDirection( inParent );
	const bool upward = (dir == dirUP);

	placeAfterNote( inParent, ioPos, upward );
}

void GRArticulation::placeStaccmo( GREvent * inParent, NVPoint & ioPos )
{
	const int dir = chooseDirection( inParent );
	const bool upward = (dir == dirUP);

	setStaccmoDirection( upward );
	placeAfterNote( inParent, ioPos, upward );
}

// ----------------------------------------------------------------------------
// 
void GRArticulation::placePizz(GREvent * inParent, NVPoint & ioPos)
{
	const int dir = chooseDirection (inParent);
	const bool upward = (dir == dirUP);

	GRStaff * staff = inParent->getGRStaff();
	const float currLSpace = staff->getStaffLSPACE();
	//const float halfSpace = float(0.5) * currLSpace;

	if( upward )	// if stemsDown
	{
		placeAfterNote( inParent, ioPos, true );
	}
	else	// if stemsUp
	{
		ioPos.y = (ioPos.y >= inParent->getStemLength()) ? -currLSpace : ioPos.y - (inParent->getStemLength()+currLSpace);
	}
}

//-----------------------------------------------------------------------------
// outside the staff prefered
void GRArticulation::placeAccent( GREvent * inParent, NVPoint & ioPos )
{
//	placeOutsideStaff( inParent, ioPos );
	const int dir = chooseDirection( inParent );
	const bool upward = (dir == dirUP);

	placeAfterNote( inParent, ioPos, upward );
}

// ----------------------------------------------------------------------------
// outside the staff prefered
void GRArticulation::placeMarcato( GREvent * inParent, NVPoint & ioPos )
{
//	placeOutsideStaff( inParent, ioPos );
	const int dir = chooseDirection( inParent );
	const bool upward = (dir == dirUP);

	setMarcatoDirection( upward );
	placeAfterNote( inParent, ioPos, upward );
}

// ----------------------------------------------------------------------------
// outside the staff prefered
void GRArticulation::placeMarcatoUp( GREvent * inParent, NVPoint & ioPos )
{
	const bool upward = true;

	setMarcatoDirection( upward );
	placeAfterNote( inParent, ioPos, upward );
}

// ----------------------------------------------------------------------------
// outside the staff prefered
void GRArticulation::placeMarcatoDown( GREvent * inParent, NVPoint & ioPos )
{
	const bool upward = false;

	setMarcatoDirection( upward );
	placeAfterNote( inParent, ioPos, upward );
}

// ----------------------------------------------------------------------------
// The tenuto is placed just after the staccato, if any
void GRArticulation::placeTenuto( GREvent * inParent, NVPoint & ioPos )
{
//	placeCloseToNote( inParent, ioPos );
	const int dir = chooseDirection( inParent );
	const bool upward = (dir == dirUP);

	placeAfterNote( inParent, ioPos, upward );
}

// ----------------------------------------------------------------------------
// harmonic
void GRArticulation::placeHarmonic(GREvent * inParent, NVPoint & ioPos)
{
	const int dir = chooseDirection (inParent);
	const bool othersAreUpward = (dir == dirUP);

	/*GRStaff * staff = inParent->getGRStaff();
	const float currLSpace = staff->getStaffLSPACE();
	const float halfSpace = float(0.5)*currLSpace;*/

	if (othersAreUpward) // if stemsDown
	{
		placeAfterNote(inParent, ioPos, true);
	}
	else // if stemsDown
	{
		ioPos.y = (ioPos.y >= inParent->getStemLength()) ? 0 : ioPos.y - (inParent->getStemLength());
	}
}

// ----------------------------------------------------------------------------
void GRArticulation::placeFermataUp( GREvent * inParent, NVPoint & ioPos )
{
	// - Check if the fermata (which is always upward) is on the
	// same side as other articulations (if any)
	const int dir = chooseDirection( inParent );	
	const bool othersAreUpward = (dir == dirUP);

	GRStaff * staff = inParent->getGRStaff();
	const float currLSpace = staff->getStaffLSPACE();
	/*const float halfSpace = float(0.5) * currLSpace;*/

	if( othersAreUpward )	// if stemsDown
	{
		placeAfterNote( inParent, ioPos, true );
	}
	else	// if stemsUp
	{
		ioPos.y = (ioPos.y >= inParent->getStemLength()) ? 0 : ioPos.y - (inParent->getStemLength());
		if (inParent->hasArticulation(kFlagPizz))
			ioPos.y -= (1.5f*currLSpace);
	}
}

void GRArticulation::placeFermataDown(GREvent * inParent, NVPoint & ioPos)
{
	const int dir = chooseDirection(inParent);
	const bool othersAreUpward = (dir == dirUP);
	
	GRStaff * staff = inParent->getGRStaff();
	const float currLSpace = staff->getStaffLSPACE();
	
	if (othersAreUpward)	//stemsDown
	{
		ioPos.y = (ioPos.y + inParent->getStemLength() <= float(4)*currLSpace) ? 
			float(4.5f)*currLSpace : ioPos.y + (inParent->getStemLength()) + 0.5f*currLSpace; //test placement
	}
	else
	{
		placeAfterNote(inParent, ioPos, false);
	}
}

// ----------------------------------------------------------------------------
// - Obsolete, breath-mark is no longer attached to any note.
void GRArticulation::placeBreathMark( GREvent * inParent, NVPoint & ioPos )
{
	const GRStaff * staff = inParent->getGRStaff();
	const float currLSpace = staff->getStaffLSPACE();
	const float halfSpace = float(0.5) * currLSpace;

	// TODO: place just before next element.
	ioPos.y = - halfSpace;
	ioPos.x += float(1.5) * LSPACE;
}

// ----------------------------------------------------------------------------
GDirection GRArticulation::chooseDirection( GREvent * inParent ) const
{
	const GRSingleNote * sng = dynamic_cast<GRSingleNote *>(inParent);
	if( sng && sng->getDirection() == dirUP )
		return dirDOWN;
	else 
		return dirUP;
}

// ----------------------------------------------------------------------------
/** \brief Places an articulation attached to a note.

	Position of articulations are calculated according to the musical
	rules, collisions are avoided. 
 
	Articulations with the same priority can't be combined.
 
	Staccato and Staccatissimo have the priority 1, closest to the note.
	The Tenuto has the priority 2. These three articulations can be inside the staff.

	The Accent and the Marcato have the priority 3.
	The Fermata has the priority 4. These articulations must be outside the staff.
*/

void GRArticulation::placeAfterNote( GREvent * inParent, NVPoint & ioPos, bool upward )
{
	GRStaff * staff = inParent->getGRStaff();
	const float space = staff->getStaffLSPACE();
	const float halfSpace = float(0.5) * space;

	const int externalFlags = (kFlagAccent | kFlagMarcato | kFlagFermataUp);
	const bool hasExternal = inParent->hasArticulation( externalFlags );

	if( upward )
	{
		// -- Walk from the note through the possible attribute positions 
		// Stop when we've reached our attribute position.

		// - Starts at one space from the note.
		ioPos.y -= space;	
		if(( ioPos.y >= 0 ) && positionIsOnStaffLine( ioPos.y, space ))
			ioPos.y -= halfSpace;
	
		// - If we have internal mixed with externals, don't start deeper
		// than one linespace 
		if( hasExternal && ( ioPos.y > halfSpace ))
			ioPos.y = halfSpace;
	
		// - If we're a staccato or a staccatissimo, we have finished.
		if(( mArticulationFlag & ( kFlagStaccato )) != 0 )
			return;	

		// - Move by one position if we passed an existing staccato or staccatissimo
		if( inParent->hasArticulation( kFlagStaccato ))
			ioPos.y += (ioPos.y > 0) ? -space : -halfSpace;
		
		// - If we're a tenuto, then we have finished.
		if( mArticulationFlag == kFlagTenuto )
			return;	

		// - Move by one position if we passed an existing tenuto
		if( inParent->hasArticulation( kFlagTenuto ))
			ioPos.y += (ioPos.y > 0) ? -space : -halfSpace;

		// - So, we're an outside-staff symbol, force being outside-staff.	
		/*if( ioPos.y > -space )
			ioPos.y = -space;
		else
			ioPos.y -= halfSpace;*/
		if (ioPos.y > -halfSpace)
			ioPos.y = -halfSpace;
		else
			/*ioPos.y -= halfSpace*/; // only if...

		// - If we're a pizzicato then we have finished
		if((mArticulationFlag & (kFlagPizz)) != 0)
			{
				ioPos.y -= halfSpace;
				return;
			}
			
		// - Move by one linespace and a half if we passed an existing pizzicato
		if (inParent->hasArticulation(kFlagPizz))
			ioPos.y -= (1.5f*space);

		// - If we're an Accent or a Marcato, then we have finished.
		if(( mArticulationFlag & (kFlagAccent | kFlagMarcato)) != 0 )
			{
			ioPos.y -= halfSpace;
			return;	
			}

		// - Move by one position if we passed an existing Accent,
		// or by 1.5 positions if we passed an existing Marcato
		if( inParent->hasArticulation( kFlagAccent ))
			ioPos.y -= space; 
		else 
		{
			if( inParent->hasArticulation( kFlagMarcato ))
			ioPos.y -= (space + halfSpace);
			else
			ioPos.y += halfSpace; // it's a fermata : we need one less halfspace
		}
	}
	else
	{
		int linesCount = staff->getNumlines();
		const float bottom = (linesCount - 1) * space;
		
		ioPos.y += space;	
		if(( ioPos.y <= bottom ) && positionIsOnStaffLine( ioPos.y, space ))
			ioPos.y += halfSpace;
	
		// - If we have internal mixed with externals, don't start deeper
		// than one linespace 
		if( hasExternal && ( ioPos.y < ( bottom - halfSpace )))
			ioPos.y = (bottom - halfSpace);
	
		// - If we're a staccato or a staccatissimo, we have finished.
		if(( mArticulationFlag & (kFlagStaccato)) != 0 )
			return;	

		// - Move by one position if we passed an existing staccato or staccatissimo
		if( inParent->hasArticulation( kFlagStaccato ))
			ioPos.y += (ioPos.y < bottom) ? space : halfSpace;
		
		// - If we're a tenuto, then we have finished.
		if( mArticulationFlag == kFlagTenuto )
			return;	

		// - Move by one position if we passed an existing tenuto
		if( inParent->hasArticulation( kFlagTenuto ))
			ioPos.y += (ioPos.y < bottom) ? space : halfSpace;

		// - So, we're an outside-staff symbol, force being outside-staff.	
		if( ioPos.y < ( bottom + halfSpace ))
			ioPos.y = ( bottom + halfSpace );
		else
			/*ioPos.y += space*/; // ?

		// - If we're an Accent or a Marcato, then we have finished.
		if(( mArticulationFlag & (kFlagAccent | kFlagMarcato)) != 0 )
		{
			ioPos.y += halfSpace;
			return;	
		}
		// - Move by one position if we passed an existing Accent,
		// or by 1.5 positions if we passed an existing Marcato
		if( inParent->hasArticulation( kFlagAccent ))
			ioPos.y += (space + halfSpace); 
		else if( inParent->hasArticulation( kFlagMarcato ))
			ioPos.y += 2 * space;
	}
}


const NVPoint & GRArticulation::getReferencePosition() const
{
	switch ( mSymbol )	// this depends on the type ...
	{
		case kStaccatoSymbol:			return sRefposStacc;			break;
		case kStaccmoUpSymbol :			return sRefposStaccmoUp;		break;
		case kStaccmoDownSymbol :		return sRefposStaccmoDown;		break;
		case kLeftHPizzSymbol:			return sRefposLeftHPizz;		break;
		case kSnapPizzSymbol :			return sRefposSnapPizz;			break;
		case kBuzzPizzSymbol :			return sRefposBuzzPizz;			break;
		case kFingernailPizzSymbol :	return sRefposFingernailPizz;	break;
		case kAccentSymbol:				return sRefposAccent;			break;
		case kMarcatoUpSymbol:			return sRefposMarcatoUp;		break;
		case kMarcatoDownSymbol:		return sRefposMarcatoDown;		break;
		case kTenutoSymbol:				return sRefposTenuto;			break;
		case kFermataUpSymbol:			return sRefposFermataUp;		break;
		case kFermataDownSymbol:		return sRefposFermataDown;		break;
		case kShortFermataUpSymbol:		return sRefposShortFermataUp;	break;
		case kShortFermataDownSymbol:	return sRefposShortFermataDown; break;
		case kLongFermataUpSymbol:		return sRefposLongFermataUp;	break;
		case kLongFermataDownSymbol :	return sRefposLongFermataDown;  break;
		case kBreathMarkSymbol:			return sRefposBreathMark;		break;
		case kHarmonicSymbol:			return sRefposHarmonic;			break;
		default:						return GObject::sRefposNone;
	}
}

void GRArticulation::GGSOutput() const
{
	GRTagARNotationElement::GGSOutput();
}

void GRArticulation::OnDraw(VGDevice & hdc) const
{
	GRTagARNotationElement::OnDraw(hdc);
}

void GRArticulation::print() const
{
}


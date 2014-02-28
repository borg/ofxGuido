#ifndef GRSingleNote_H
#define GRSingleNote_H

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

#include "ARTHead.h"
#include "GRNote.h"

class GRAccidental;
template <class T> class KF_IPointerList;
typedef KF_IPointerList<GRAccidental> GRAccidentalList;

class GRStaff;
class GRStdNoteHead;
class GRStem;
//class GRNoteDot;
class GRFlag;
class GRGlobalStem;
class GRBeam;

class ARTHead;
class ARAcc;
class ARAlter;

/** \brief Graphical representation of a note event.
*/
class GRSingleNote : public GRNote
{
 
 public:
		GRSingleNote( GRStaff *, ARNote * abstractRepresentationOfNote,
						const TYPE_TIMEPOSITION & relativeTimePositionOfGRNote,
						const TYPE_DURATION & durationOfGRNote );

		virtual ~GRSingleNote();

		virtual void addToOffset( const NVPoint & pt);
		virtual ARTHead::HEADSTATE adjustHeadPosition(ARTHead::HEADSTATE sugHeadState = ARTHead::NORMAL);

		virtual void	setHeadState(const ARTHead * headstate);
		virtual void	setNoteFormat(const ARNoteFormat * frmt);

		virtual int		adjustLength(const TYPE_DURATION & ndur);

		virtual void	addArticulation(ARMusicalTag * mtag);

				void	doCreateNote( const TYPE_DURATION & p_durtemplate /* = DURATION_0*/);

		virtual char*	getGGSInfo(int infotype) const; 
		virtual void	GGSOutput() const;
		virtual void	OnDraw( VGDevice & hdc ) const;
		virtual void	GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;

		virtual void	print() const { }

		virtual void	setSize(float nsize)
		{
			 mSize = nsize;
			 mStemLen *= mSize;
		}

		virtual void	updateBoundingBox();
		virtual void	setHPosition( float inX );
		virtual void	setPosition( const NVPoint & inPos );
		virtual void	recalcVerticalPosition();

		// This is the Position, where Ties and Slurs start
		// and end (these values includes the note-position)
		virtual NVPoint getTieStart() const;
		virtual NVPoint getTieEnd() const;
		virtual void	setFlagOnOff(bool p);

		// -- Stem related --
		virtual void setBeamStem(GRBeam * beam, float pos);

		// This tells the Note, to draw the stem only
		virtual void	drawStemOnly( int flag );	
	  	virtual int		tstStemEndPos( float ) { return 0; }
	  	virtual int		setStemEndPos( float ) { return 0; }

		virtual void    setStemOffsetStartPosition(float inOffset);
		virtual void    setFirstSegmentDrawingState(bool inDrawingState);

	 	virtual GDirection getDirection() const { return mStemDir; }

		// This sets the Direction of the Stem
		virtual void	setStemDirection( GDirection dir );

		// This sets Stem On or Off (1 == on, 0 == off)
		virtual void	setStemOnOff( bool p );

		virtual float	changeStemLength( float inLen );
		virtual float	setStemLength( float inLen );
		virtual GDirection getStemDirection() const;
		virtual NVPoint getStemStartPos() const;
		virtual NVPoint getStemEndPos() const;
		virtual float	getStemLength() const;
	    virtual bool	getStemDirSet() const  	{ return mStemDirSet; }
		virtual bool	getStemLengthSet() const	{ return mStemLengthSet; }

		// -
		virtual const TYPE_DURATION & getDurTemplate() const { return mDurTemplate; }
		virtual int		getNumFaehnchen() const;

				void	removeElements();

		virtual void	tellPosition( GObject * caller, const NVPoint & newPosition ); 

	  	GRStdNoteHead *	getNoteHead() const;
	  	void			extractAccidentals( GRAccidentalList * outList );

        ARTHead::HEADSTATE getHeadState() { return mHeadState; }

  protected:
	  GRStem *			getStem();
	  const GRStem * 	getStem() const;
	  GRFlag *			getFlag();
	  const GRFlag * 	getFlag() const;

	  int 			mNumHelpLines;
	  float 		mNoteBreite; // the width of a note
	  TYPE_DURATION mDurTemplate; // the template for the display

  private:
	void handleAccidental	(const ARAcc* acc);
	void createNote(const TYPE_DURATION & p_durtemplate /*= DURATION_0*/ );

	GRStdNoteHead *	mNoteHead; // exists also in element list.
	GDirection		mStemDir;
	float		mStemLen;
	bool 		mStemLengthSet;
	bool 		mStemDirSet;

	ARTHead::HEADSTATE mHeadState;
};


#endif



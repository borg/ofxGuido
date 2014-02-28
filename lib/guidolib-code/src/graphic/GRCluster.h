#ifndef GRCluster_H
#define GRCluster_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRARCompositeNotationElement.h"
#include "GRPositionTag.h"
#include "ARTHead.h"

class ARCluster;
class ARNoteFormat;
class GRStaff;
class GRSingleNote;

/** \brief Graphical representation of a cluster.
*/
class GRCluster : public GRARCompositeNotationElement,
	              public GRPositionTag
{
public:

				 GRCluster(GRStaff * stf, ARCluster * arcls, GRSingleNote *sngNote, ARNoteFormat * curnoteformat);
	virtual	 	~GRCluster();

	virtual void OnDraw(VGDevice &hdc) const;
	virtual void tellPosition( GObject * caller, const NVPoint & np );

	virtual void addAssociation( GRNotationElement * p );
    virtual GuidoPos AddTail( GRNotationElement * el );

    void setSecondNoteYPosition();

    virtual const unsigned char * getColRef() const { return mColRef; }

    int getNoteCount() const { return gNoteCount;}

    void setClusterOrientation(GDirection inStemDir, ARTHead::HEADSTATE inHeadStateOrientation);

    ARCluster *getARCluster() const;

protected:

    float gdx;
    float gdy;
    float ghdx;
    float ghdy;
    float gSize;

    float gFirstNoteYPosition;
    float gSecondNoteYPosition;
    GRStaff *gStaff;
    TYPE_DURATION gDuration;

    ARTHead::HEADSTATE gClusterOrientation;
    GDirection gStemDir;

    unsigned char *gClusterColor;

    int gNoteCount;
};

#endif



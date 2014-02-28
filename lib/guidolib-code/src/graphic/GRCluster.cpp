/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRCluster.h"

#include "GRStaff.h"

#include "ARCluster.h"

#include "VGDevice.h"
#include "GRTagARNotationElement.h"
#include "GRSingleNote.h"
#include "GRGlobalStem.h"
#include "ARNoteFormat.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

GRCluster::GRCluster(GRStaff * stf, ARCluster * arcls, GRSingleNote *sngNote, ARNoteFormat * curnoteformat) :
						GRARCompositeNotationElement(arcls),
						GRPositionTag(arcls->getEndPosition(), arcls),
                        gStaff(stf),
                        gDuration(0),
                        gClusterOrientation(ARTHead::NORMAL),
                        gStemDir(dirAUTO),
                        gClusterColor(NULL)
{
	assert(stf);
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = stf->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;

	sse->p = (void *) getNewGRSaveStruct();

	mStartEndList.AddTail(sse);

    gDuration = sngNote->getDurTemplate();

    const TagParameterString *tmpColor;

    mTagSize = stf->getSizeRatio();

    if (curnoteformat)
    {
        // - Size
        const TagParameterFloat * tmp = curnoteformat->getSize();
        if (tmp)
            mTagSize = tmp->getValue();

        // - Color
        tmpColor = curnoteformat->getColor();
        if (tmpColor)
        {
            if (!mColRef)
                mColRef = new unsigned char[4];
            tmpColor->getRGB(mColRef);
        }

        // - Offset
		const TagParameterFloat * tmpdx = curnoteformat->getDX();
		const TagParameterFloat * tmpdy = curnoteformat->getDY();

		if (tmpdx)
			mTagOffset.x = (GCoord)(tmpdx->getValue(stf->getStaffLSPACE()));
		if (tmpdy)
			mTagOffset.y = (GCoord)(tmpdy->getValue(stf->getStaffLSPACE()));
    }

    const TagParameterString *tmps = arcls->getColor();
    if (tmps)
    {
        gClusterColor = new unsigned char[4];
        tmps->getRGB(gClusterColor);

        if (*tmps->getValue())
            mColRef = NULL;
    }

    gdx = arcls->getadx();
    gdy = arcls->getady();
    ghdx = arcls->getahdx();
    ghdy = arcls->getahdy();
    gSize = arcls->getSize();

    GREvent *grEvent = dynamic_cast<GREvent *>(sngNote);
    if (grEvent)
    {
        grEvent->getGlobalStem()->setOffsetXY(gdx, -gdy);
        grEvent->getGlobalStem()->setMultiplicatedSize(gSize);
    }

    gdy += mTagOffset.y;
    gdx += mTagOffset.x;

    int *firstNoteParameters = arcls->getFirstNoteParameters();

    gFirstNoteYPosition = stf->getNotePosition(firstNoteParameters[0], firstNoteParameters[1]);
    gSecondNoteYPosition = gFirstNoteYPosition;

    gNoteCount = arcls->getNoteCount();
}

GRCluster::~GRCluster() {}

void GRCluster::OnDraw(VGDevice &hdc) const
{
    if (mDraw)
	{
    	if (mColRef)
        {
            VGColor color(mColRef);
            hdc.PushFillColor(color);
            hdc.PushPen(color, 1);
        }
   		else if (gClusterColor)
        {
            VGColor color(gClusterColor);
            hdc.PushFillColor(color);
            hdc.PushPen(color, 1);
        }

    	NVRect r = getBoundingBox();
    	r += getPosition();

    	float x = r.left + gdx + ghdx;

    	float curLSpace = gStaff->getStaffLSPACE();

    	float xOrientation = 0;

    	if (gClusterOrientation == ARTHead::LEFT && gStemDir == dirDOWN)
        	xOrientation = - 55;
    	else if (gClusterOrientation == ARTHead::RIGHT && gStemDir == dirUP)
        	xOrientation = 55;

    	// - Quarter notes and less
    	if (gDuration < DURATION_2 )
        {
            const float xCoords [] = {x + (xOrientation - 31) * mTagSize * gSize,
                x + (xOrientation + 29) * mTagSize * gSize,
                x + (xOrientation + 29) * mTagSize * gSize,
                x + (xOrientation - 31) * mTagSize * gSize};
            const float yCoords [] = {gFirstNoteYPosition - gdy - ghdy - curLSpace / 2,
                gFirstNoteYPosition - gdy - ghdy - curLSpace / 2,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2};

            hdc.Polygon(xCoords, yCoords, 4);
        }
	    else
        {
            const float xCoords1 [] = {x + (xOrientation - 31) * mTagSize * gSize,
                x + (xOrientation + 29) * mTagSize * gSize,
                x + (xOrientation + 29) * mTagSize * gSize,
                x + (xOrientation - 31) * mTagSize * gSize};
            const float yCoords1 [] = {gFirstNoteYPosition - gdy - ghdy - curLSpace / 2,
                gFirstNoteYPosition - gdy - ghdy - curLSpace / 2,
                gFirstNoteYPosition - gdy - ghdy - curLSpace / 2 + 6,
                gFirstNoteYPosition - gdy - ghdy - curLSpace / 2 + 6};
            const float xCoords2 [] = {x + (xOrientation + 23) * mTagSize * gSize,
                x + (xOrientation + 29) * mTagSize * gSize,
                x + (xOrientation + 29) * mTagSize * gSize,
                x + (xOrientation + 23) * mTagSize * gSize};
            const float yCoords2 [] = {gFirstNoteYPosition - gdy - ghdy - curLSpace / 2,
                gFirstNoteYPosition - gdy - ghdy - curLSpace / 2,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2};
            const float xCoords3 [] = {x + (xOrientation - 31) * mTagSize * gSize,
                x + (xOrientation + 29) * mTagSize * gSize,
                x + (xOrientation + 29) * mTagSize * gSize,
                x + (xOrientation - 31) * mTagSize * gSize};
            const float yCoords3 [] = {gSecondNoteYPosition - gdy - ghdy + curLSpace / 2 - 6,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2 - 6,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2};
            const float xCoords4 [] = {x + (xOrientation - 31) * mTagSize * gSize,
                x + (xOrientation - 25) * mTagSize * gSize,
                x + (xOrientation - 25) * mTagSize * gSize,
                x + (xOrientation - 31) * mTagSize * gSize};
            const float yCoords4 [] = {gFirstNoteYPosition - gdy - ghdy - curLSpace / 2,
                gFirstNoteYPosition - gdy - ghdy - curLSpace / 2,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2,
                gSecondNoteYPosition - gdy - ghdy + curLSpace / 2};

            hdc.Polygon(xCoords1, yCoords1, 4);
            hdc.Polygon(xCoords2, yCoords2, 4);
            hdc.Polygon(xCoords3, yCoords3, 4);
            hdc.Polygon(xCoords4, yCoords4, 4);
        }

	    // - Restore context
	    if (mColRef || gClusterColor)
	    {
		    hdc.PopPen();
		    hdc.PopFillColor();
	    }
	}
}

void GRCluster::addAssociation(GRNotationElement *p)
{
	GRARCompositeNotationElement::addAssociation(p);
}

GuidoPos GRCluster::AddTail(GRNotationElement * el)
{
	GuidoPos mypos = GRARCompositeNotationElement::AddTail(el);

	// now, we remove the associatons ...
	// of those elements, that are members of 
	// the composite-sructure ...

	GuidoPos elpos = NULL;
	if (mAssociated && (elpos = mAssociated->GetElementPos(el)) != NULL)
	{
		mAssociated->RemoveElementAt(elpos);
	}

	return mypos;
}

//-------------------------------------------------------------------
void GRCluster::tellPosition(GObject * caller, const NVPoint & np)
{
	GREvent *ev = dynamic_cast<GREvent *>(caller);
	
    if (ev)
	{
        NVPoint newPoint (np);
        newPoint.y = -LSPACE;

        setPosition(newPoint);
	}
}

void GRCluster::setSecondNoteYPosition()
{
    ARCluster *arCluster = getARCluster();

    int *secondNoteParameters = arCluster->getSecondNoteParameters();

    gSecondNoteYPosition = gStaff->getNotePosition(secondNoteParameters[0], secondNoteParameters[1]);

    if (gFirstNoteYPosition > gSecondNoteYPosition)
    {
        float tmpSwitch      = gSecondNoteYPosition;
        gSecondNoteYPosition = gFirstNoteYPosition;
        gFirstNoteYPosition  = tmpSwitch;
    }
}

void GRCluster::setClusterOrientation(GDirection inStemDir, ARTHead::HEADSTATE inHeadStateOrientation)
{
    gClusterOrientation = inHeadStateOrientation;
    gStemDir            = inStemDir;
}

ARCluster *GRCluster::getARCluster() const
{
	return /*dynamic*/static_cast<ARCluster*>(getAbstractRepresentation());
}

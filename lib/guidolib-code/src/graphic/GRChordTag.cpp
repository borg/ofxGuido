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

#include "ARUserChordTag.h"

#include "GRStaff.h"
// #include "GREmpty.h"
#include "GRSpring.h"
#include "GRSystem.h"
#include "GRSystemTag.h"
#include "GRChordTag.h"
#include "GRVoice.h"


GRChordTag::GRChordTag(GRStaff * grstaff, ARUserChordTag * pchordtag )
						: GRPTagARNotationElement(pchordtag)
{
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;

	sse->grsystem = grstaff->getGRSystem();
	sse->startElement = NULL;
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endElement = NULL;
	sse->p = NULL;

	mStartEndList.AddTail(sse);
}

GRChordTag::~GRChordTag()
{
}

void GRChordTag::addAssociation(GRNotationElement * grnot)
{
//	GRPTagARNotationElement::addAssociation(grnot);
}

void GRChordTag::removeAssociation(GRNotationElement * grnot)
{
//	GRPTagARNotationElement::removeAssociation(grnot);
}

void GRChordTag::RangeEnd(GRStaff * grstaff)
{
}

void GRChordTag::tellPosition(GObject * obj, const NVPoint & pt)
{
}

/** \brief Called when the linked GRSystemTag gets a position-update
*/
void GRChordTag::checkPosition(const GRSystem * grsys)
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grsys);
	if (!sse) return;

	assert(sse);
	mIsSystemCall = true;	
	// ...
	mIsSystemCall = false;
}

const char* GRChordTag::getLabel() const
{
	const ARUserChordTag * puct = dynamic_cast<const ARUserChordTag *>(getAbstractRepresentation());
	return puct ? puct->getLabelValue() : "";
}

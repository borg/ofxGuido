#ifndef GREmpty_H
#define GREmpty_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GREvent.h"

/** \brief An empty (invisible) event.
*/
class GREmpty : public GREvent  
{
	public:

				GREmpty( GRStaff * inStaff,
							ARMusicalEvent * abstractRepresentationOfEmpty,
							bool p_ownsAR = false );

				GREmpty( GRStaff * inStaff,
							ARMusicalEvent * abstractRepresentationOfEmpty,
							const TYPE_TIMEPOSITION theRelativeTimePositionOfGR,
							const TYPE_DURATION theDurationOfGR);
		
		virtual ~GREmpty();

		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;
		virtual void tellPosition( GObject * caller, const NVPoint & newPosition );

		virtual void print() const	{ }
		virtual void updateBoundingBox();
		
	protected: 
};

#endif


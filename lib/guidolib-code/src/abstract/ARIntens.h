#ifndef ARIntens_H
#define ARIntens_H

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

#include "ARMTParameter.h"

/** \brief The intensity tag.

	p, pp, ppp, f, ff, ffff, mf, mp, sf
*/

class ARIntens :  public ARMTParameter 
{
	public:

			ARIntens(const char * txt, float val = -1.0f);

			ARIntens() : ARMTParameter(), value(-1.0f)
			 {
			 	relativeTimePosition.set( -1, 1 );
			 	duration = DURATION_0;
			 }
		virtual void setTagParameterList(TagParameterList & tpl);

		virtual const NVstring & getText() const { return mIntensText; }
		virtual float getValue() const { return value; }

		virtual void print() const;
		// virtual ostream & operator<<(ostream &os) const;
		virtual void PrintName(std::ostream &os) const;
		virtual void PrintParameters(std::ostream &os) const;

		virtual void	browse(TimeUnwrap& mapper) const;


	protected:
		NVstring mIntensText; // der entsprechende Text ...
		float value;

		static ListOfTPLs ltpls;
};

#endif

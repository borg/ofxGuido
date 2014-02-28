#ifndef ARAutoBeam_H
#define ARAutoBeam_H

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

#include "ARBeam.h"

/** \brief not yet documented
*/
class ARAutoBeam : public ARBeam
  // , public ARPositionTag
{
  public:
		ARAutoBeam();

		/* ARAutoBeam(POSITION psp, POSITION pep)
		{
			sp = psp;
			ep = pep;
		} */

		virtual ~ARAutoBeam() {}
		virtual void print() const {}
		virtual void PrintName(std::ostream & os) const;

		// virtual ostream &operator<<(std::ostream &os) const;

};

#endif


#ifndef ARDoubleBar_H
#define ARDoubleBar_H

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

#include "ARBar.h"

/** \brief not yet documented
*/
class ARDoubleBar : public ARBar  
{
public:
	ARDoubleBar();
	virtual ~ARDoubleBar();
	virtual void print() const;
	// virtual ostream & operator<<(ostream &os) const;
	virtual void PrintName(std::ostream & os) const;

};

#endif	/* ARDoubleBar_H */

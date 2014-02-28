#ifndef ARDRRENZ_H
#define ARDRRENZ_H

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
// ARDrHoos.h: declaration/interface of class ARDrHoos.
//
//////////////////////////////////////////////////////////////////////

#include "ARMTParameter.h"
// #include "ARMusicalObject.h"

/** \brief not yet documented
*/
class ARDrRenz :
	// public ARMusicalObject,
	public ARMTParameter
{
public:
	ARDrRenz(int p_inv = 0);
	virtual ~ARDrRenz();

	virtual void print() const { };

	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;
	virtual void setTagParameterList(TagParameterList & tpl);


	int getInverse() const { return inverse; }

protected:
	static ListOfTPLs ltpls;
	int inverse;
};

#endif

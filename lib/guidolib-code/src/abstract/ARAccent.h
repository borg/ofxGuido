#ifndef ARAccent_H
#define ARAccent_H

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
#include "ARPositionTag.h"

/** \brief not yet documented
*/
class ARAccent : public ARMTParameter,  public ARPositionTag
{		
public:			

			ARAccent() : ARMTParameter()
			{
				rangesetting = ONLY;
			}

	virtual ~ARAccent() 
	{
		// delete TagParameterPointer ...
	};

	virtual void setTagParameterList(TagParameterList & tpl);

	// routines to access TagParameters ..
	virtual void print() const;	
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;	

	virtual void	browse(TimeUnwrap& mapper) const;

protected:

	// TagParameters ..
	// TagParameterString *textformat;
	// TagParameterFloat  *fsize;
	// TagParameterInt	  *fnumber;

	// this is important for allowed
	// TagParameters ....
	static ListOfTPLs ltpls;
};

#endif

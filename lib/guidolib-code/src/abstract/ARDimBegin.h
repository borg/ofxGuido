#ifndef ARDimBegin_H
#define ARDimBegin_H

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

#include "ARTagBegin.h"

/** \brief not yet documented
*/
class ARDimBegin : public ARTagBegin
{
	public:

			ARDimBegin(int id = 0) : ARTagBegin(id) {}

		virtual ~ARDimBegin() {}

		virtual ostream & operator<<(ostream &os) const
		{
			return os << " //dimBegin ";
		}
};

#endif

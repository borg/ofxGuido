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

// #include "GRSystem.h"

#include "GRSystemTag.h"
#include "GRSystemTagInterface.h"

GRSystemTag::GRSystemTag(GRSystemTagInterface *pitag)
{
	mNeedsSpring = 0;
	itag = pitag;
}

void GRSystemTag::checkPosition(const GRSystem *grsys)
{
	if (itag)
	{
		itag->checkPosition(grsys);
	}

}

void GRSystemTag::print() const
{
}

void GRSystemTag::OnDraw( VGDevice & hdc ) const
{
}

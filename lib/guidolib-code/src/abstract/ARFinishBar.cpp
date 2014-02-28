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

#include <iostream>
#include "ARFinishBar.h"

ARFinishBar::ARFinishBar()
{
	rangesetting = NO;
}

ARFinishBar::~ARFinishBar()
{

}

void ARFinishBar::print() const
{
	/* fprintf(stderr,"(%.2f:Bar ",(float) relativeTimePosition);
	fprintf(stderr,"%d ", barnumber);
	fprintf(stderr,")"); */
}

/* ostream & ARDoubleBar::operator<<(ostream &os) const
{
	return os << "\\doublebar ";
} */
void ARFinishBar::PrintName(std::ostream &os) const
{
	os << "\\endBar";
} 

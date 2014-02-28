/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include "ARBreathMark.h"

// #include "TagParameterInt.h"
// #include "ListOfStrings.h"
#include "TagParameterList.h"

// ListOfTPLs ARBreathMark::ltpls(1);

ARBreathMark::ARBreathMark()
{
}

ARBreathMark::~ARBreathMark()
{

}

void ARBreathMark::setTagParameterList(TagParameterList & tpl)
{
/*	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( "" );
		CreateListOfTPLs(ltpls,lstrs);
	}
*/

	tpl.RemoveAll();
}

void ARBreathMark::PrintName( std::ostream & os ) const
{
	os << "\\breathMark";
}

/*
void ARBreathMark::PrintParameters(std::ostream & os) const
{
}*/


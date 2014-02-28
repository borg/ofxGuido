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
#include "ARIntens.h"

#include "GRDefine.h"

#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"

ListOfTPLs ARIntens::ltpls(1);

ARIntens::ARIntens(const char * txt, float val)
	: ARMTParameter(), value(val)
{
	mIntensText = txt;
}

void ARIntens::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "S,type,,r;F,value,-1.0,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterString * tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			mIntensText = tps->getValue();

			TagParameterFloat * tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
			assert(tpf);

			if (tpf->pflag != TagParameter::NOTSET)
			{
				value = tpf->getValue();
			}
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
void ARIntens::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kIntens);
}

void ARIntens::print() const
{
}

void ARIntens::PrintName(std::ostream &os) const
{
		os << "\\intens";
}

void ARIntens::PrintParameters(std::ostream &os) const
{
	os << "<\"" << mIntensText << "\"";
	if (value != -1.0)
	{
		os << "," << value * kVirtualToCm << "cm";
	}
	os << ">";
}




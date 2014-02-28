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
#include <sstream>
#include "secureio.h"

#include "ARMeter.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"


ListOfTPLs ARMeter::ltpls(1);

ARMeter::ARMeter()
{
	mtype = NONE;
	autoBarlines = 1;

    numerator = 0;
    denominator = 0;
}

ARMeter::ARMeter(int p_numerator, int p_denominator)
{
	numerator = p_numerator;
	denominator = p_denominator;

    std::stringstream bufferSStream;
    bufferSStream << numerator << "/" << denominator;

	mMeterName = bufferSStream.str().c_str();
	mtype = NUMERIC;
	autoBarlines = 1;
}


ARMeter::~ARMeter()
{
}


void ARMeter::print() const
{
}

std::ostream & ARMeter::operator<<(std::ostream & os) const
{
	// ATTENTION: evtl. problem, if we impl. a meter-sig that has no name
	if (mtype == NONE) return os;
	return os << "\\meter<\"" << mMeterName << "\"> ";
}

void ARMeter::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,type,4/4,r;F,size,1.0,o;S,autoBarlines,on,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
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

			mMeterName = tps->getValue();

			TagParameterFloat * tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
			assert(tpf);

			size = tpf->getValue();

			tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			std::string off ("off");
			if (off == tps->getValue())
				autoBarlines = 0;
			else
				autoBarlines = 1;


            //Meter string analysis to set numerator/denominator
            std::string meterStr(mMeterName);
            std::string delimiterSlash = "/";

            size_t posSlash = 0;
            std::string completeNumeratorStr;
            std::string denominatorStr;

            posSlash = meterStr.find(delimiterSlash);
            completeNumeratorStr = meterStr.substr(0, posSlash);
            denominatorStr = meterStr;
            denominatorStr.erase(0, posSlash + 1);

            if (sscanf(denominatorStr.c_str(), "%d", &denominator) == 1)
            {
                while(denominator > 99)
                    denominator = (int)(denominator / 10);


                /* Numerator check */

                int tmpNumeratorX = 0;
                std::string delimiterPlus = "+";
                size_t posPlus = 0;
                std::string tpmNumeratorXStr;
                while ((posPlus = completeNumeratorStr.find(delimiterPlus)) != std::string::npos)
                {
                    tpmNumeratorXStr = completeNumeratorStr.substr(0, posPlus);
                    if (sscanf(tpmNumeratorXStr.c_str(), "%d", &tmpNumeratorX) == 1)
                    {
                        /* We keep only the first 2 figures */
                        while(tmpNumeratorX > 99)
                            tmpNumeratorX = (int)(tmpNumeratorX / 10);

                        numeratorsVector.push_back(tmpNumeratorX);
                    }
                    completeNumeratorStr.erase(0, posPlus + delimiterPlus.length());
                }

                if (sscanf(completeNumeratorStr.c_str(), "%d", &tmpNumeratorX) == 1)
                { // last sum member
                    /* We keep only the first 2 figures */
                    while(tmpNumeratorX > 99)
                        tmpNumeratorX = (int)(tmpNumeratorX / 10);

                    numeratorsVector.push_back(tmpNumeratorX);
                }

                if (!numeratorsVector.empty())
                    mtype = NUMERIC;
            }
            else
            { // read error
                if (mMeterName == "C")
                {
                    mtype = C;
                    numeratorsVector.push_back(4);
                    denominator = 4;
                }
                else if (mMeterName == "C/")
                {
                    mtype = C2;
                    numeratorsVector.push_back(2);
                    denominator = 2;
                }
                else
                {
                    numeratorsVector.push_back(0);
                    denominator = 1;
                    mtype = NONE;
                }
            }
		}

		delete rtpl;
	}
	else
	{
		numeratorsVector.push_back(0);
		denominator = 1;
		mtype = NONE;
	}

    if (!numeratorsVector.empty())
    {
        for(size_t i = 0; i < numeratorsVector.size(); i++)
            numerator += numeratorsVector[i];
    }

    tpl.RemoveAll();
}

bool ARMeter::IsStateTag() const
{
	return true;
}

TYPE_TIMEPOSITION ARMeter::getMeterTime() const
{
	return TYPE_TIMEPOSITION(numerator,denominator);
}
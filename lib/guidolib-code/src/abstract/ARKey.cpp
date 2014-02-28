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
#include <string.h>
#include <ctype.h>

#include "ARKey.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

using namespace std;

int gd_noteName2pc(const char *name);


ListOfTPLs ARKey::ltpls(1);

ARKey::ARKey(const TYPE_TIMEPOSITION & timeposition)
	: ARMTParameter(timeposition), mIsFree(false)
{
	fKeyNumber = 0;
	memset(fOctarray,0,NUMNOTES*(sizeof(int))); // octarray auf 0 setzen;
}


ARKey::ARKey(int p_keynumber) : mIsFree(false)
{
	fKeyNumber = p_keynumber;
	memset(fOctarray,0,NUMNOTES*(sizeof(int))); // octarray auf 0 setzen;
}

ARKey::ARKey() : mIsFree(false)
{
	fKeyNumber = 0; // no accidentals
	memset(fOctarray,0,NUMNOTES*(sizeof(int))); // octarray auf 0 setzen;
}

ARKey::ARKey(const ARKey & key)
{
	mIsFree = key.mIsFree;
	fKeyNumber = key.fKeyNumber;

	key.getOctArray((int *) &fOctarray);
	key.getFreeKeyArray (fAccarray);

}

bool ARKey::operator ==(const ARKey & k) const
{
	if (mIsFree != k.mIsFree)		return false;
	if (fKeyNumber != k.fKeyNumber)	return false;
	for (int i =0;i<NUMNOTES;i++)
	{
		if (fAccarray[i] != k.fAccarray[i])			return false;
		if (fOctarray[i] != k.fOctarray[i])		return false;
	}

	// they are equal!
	return true;
	
}
void ARKey::print() const
{
}

void ARKey::PrintName(std::ostream & os) const
{
	os << "\\key";
}
void ARKey::PrintParameters(std::ostream & os) const
{
	os << "<" << fKeyNumber << "> ";
}

void ARKey::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl

		// either a key-string ("G") or a key-number key=3
		lstrs.AddTail(( "S,key,,r"));
		lstrs.AddTail(( "I,key,,r"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if( ret >= 0 && rtpl )
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterString * tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			NVstring name = tps->getValue();

			// ist free-Tag gesetzt?
			if (name.substr(0, 5) == "free=" ) {
				mIsFree = true;
//				getKeyArray(name.substr(5, name.length()-5));
				newgetKeyArray (name.substr(5, name.length()-5));
			}
			else {
				mIsFree = false;
				int t = (int)name[0];
				int major = (t == toupper(t));

				t = toupper(t);				
				switch (t) 
				{
					case 'F': 	fKeyNumber = -1;	break;
					case 'C': 	fKeyNumber = 0;	break;
					case 'G': 	fKeyNumber = 1;	break;
					case 'D': 	fKeyNumber = 2;	break;
					case 'A': 	fKeyNumber = 3;	break;
					case 'E': 	fKeyNumber = 4;	break;
					case 'H':
					case 'B':	fKeyNumber = 5;	break;
					default:	fKeyNumber = 0;
				}
				
				if (!major)				fKeyNumber -= 3;		// minus 3 accidentals  (A-Major ->  a-minor ...)				
				if (name.length() > 1)
				{
					t = name[1];
					if (t == '#')		fKeyNumber += 7;
					else if (t == '&')	fKeyNumber -= 7;
				}
			}
		}
		else if (ret == 1)
		{
			// then, we now the match for the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();
			TagParameterInt * tpi = TagParameterInt::cast(rtpl->GetNext(pos));
			assert(tpi);
			fKeyNumber = tpi->getValue();
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}


float ARKey::getAccidental (const char*& ptr) const
{
	float accidental = 0.f;
	if (*ptr == '[') {
		ptr++;
		string acc;
		while (*ptr && (*ptr != ']')) acc += *ptr++;
		if (*ptr == ']') {
			accidental = (float)atof(acc.c_str());
			ptr++;
		}
	}
	else while (*ptr) {
		if (*ptr == '#') accidental += 1.f;
		else if (*ptr == '&') accidental -= 1.f;
		else break;
		ptr++;			
	}
	return accidental;
}

int ARKey::getNote (const char*& ptr) const
{
	string notename;
	while ((*ptr==' ') || (*ptr=='	')) ptr++;
	while (isalpha(*ptr)) notename += *ptr++;
	return gd_noteName2pc(notename.c_str());
}

bool ARKey::getOctave (const char*& ptr, int& oct) const
{
	int sign = 1;
	if (*ptr == '-') {
		sign = -1;
		ptr++;
	}
	else if (*ptr == '+')
		ptr++;

	if (isdigit (*ptr)) {
		oct = (*ptr++ - '0') * sign;
		return true;
	}
	return false;
}

void ARKey::newgetKeyArray(const std::string& inString)
{
	int loop=0;
	const char* ptr = inString.c_str();
	memset(fAccarray, 0, sizeof(fAccarray)); // mkarray auf 0 setzen.

	float currentAccidental = 0.f;
	while (*ptr && (loop < 10)) {
		int note = getNote (ptr);
		if( note > 1) {
			float accidental = getAccidental(ptr);
			int index = 0;
			switch(note) 
			{
				case NOTE_C:	index=0; break;
				case NOTE_CIS:	break;				// do nothing (yet)
				case NOTE_D:	index=1; break;
				case NOTE_DIS:	break;
				case NOTE_E:	index=2; break;
				case NOTE_F:	index=3; break;
				case NOTE_FIS:	break;
				case NOTE_G:	index=4; break;
				case NOTE_GIS:	break;
				case NOTE_A:	index=5; break;
				case NOTE_AIS:	break;
				case NOTE_H:	index=6; break;
				default: return;
			}
			if (accidental)
				fAccarray[index] = currentAccidental = accidental;
			else 
				fAccarray[index] = currentAccidental;
			int oct;
			if (getOctave (ptr, oct)) fOctarray[index] = oct;

//			cout << "at index " << index << " acc: " << fAccarray[index] << endl;
		}
		else break; // unknown notename
	}
}

void ARKey::getKeyArray(std::string inString) 
{
	size_t i,j,index;
	int acc = 0;	// can be < 0
	string notename, accidental;

	memset(fAccarray, 0, NUMNOTES * sizeof(int)); // mkarray auf 0 setzen.

	while (inString.length()) 
	{
		index = 0;
		for (i = 0; i < inString.length() && isalpha(inString[i]); i++) { }
		notename = inString.substr(0,i);

		for (j=i; j< inString.length() && (inString[j]=='#' || inString[j]=='&'); j++) { }		
		accidental = inString.substr(i,j-i);		// get type of accidental

		acc = (int)accidental.length();
		if (acc > 2) return; // Error !
		if (acc == 2 && (accidental[0] != accidental[1]) )
			return; // accidentals like &# or #& make no sense
		acc *= ((accidental[0] == '#') ? 1 : -1 );

		// if correct notename, store accidental  at correct position in  mkarray
		switch(gd_noteName2pc(notename.c_str())) 
		{
			case NOTE_C: index=0; fAccarray[0]=(float)acc; break;
			case NOTE_CIS: break; // do nothing (yet)
			case NOTE_D: index=1; fAccarray[1]=(float)acc; break;
			case NOTE_DIS: break; // do nothing (yet)
			case NOTE_E: index=2; fAccarray[2]=(float)acc; break;
			case NOTE_F: index=3; fAccarray[3]=(float)acc; break;
			case NOTE_FIS: break; // do nothing (yet)
			case NOTE_G: index=4; fAccarray[4]=(float)acc; break;
			case NOTE_GIS: break; // do nothing (yet)
			case NOTE_A: index=5; fAccarray[5]=(float)acc; break;
			case NOTE_AIS: break; // do nothing (yet)
			case NOTE_H: index=6; fAccarray[6]=(float)acc; break;
			default: return; // if neede, do error handling here
		}


		// In which octave should accidental be put
		if (j < inString.length()) 
		{
			if(inString[j] == '+')
			{
				++j;
				if( j < inString.length())
				switch (inString[j]) 
				{
					case '0': fOctarray[index]=0; ++j; break;
					case '1': fOctarray[index]=1; ++j; break;
					case '2': fOctarray[index]=2; ++j; break;
					default: return; // here additional octaves or errors could be handled
				}
			}
			else if(inString[j]=='-')
			{
				++j;
				if( j < inString.length())
				switch (inString[j]) 
				{
					case '0': fOctarray[index]= 0; ++j; break;
					case '1': fOctarray[index]=-1; ++j; break;
					case '2': fOctarray[index]=-2; ++j; break;
					default: return; // here additional octaves or errors could be handled
				}
			}
			else if (isdigit(inString[j]))
			{
				switch (inString[j]) 
				{
					case '0': fOctarray[index]=0; ++j; break;
					case '1': fOctarray[index]=1; ++j; break;
					case '2': fOctarray[index]=2; ++j; break;
					default: return; // here additional octaves or errors could be handled
				}
			}
		}
		inString = inString.substr( j, inString.length()-j);
	}
}

void ARKey::getFreeKeyArray(float * keyArray) const
{
	for (int i=0;i<NUMNOTES;i++) keyArray[i]=fAccarray[i];
}

void ARKey::getOctArray(int * OctArray) const
{
	for (int i=0;i<NUMNOTES;i++) OctArray[i]=fOctarray[i];
}


bool ARKey::IsStateTag() const
{
	return true;
}

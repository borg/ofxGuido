#ifndef ARLyrics_H
#define ARLyrics_H

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

#include "ARPositionTag.h"
#include "ARMTParameter.h"
//#include "ARMusicalObject.h"
//#include "ARTagBegin.h"
#include "GuidoDefs.h"		// for LSPACE

class ARText;

/** \brief Implement the lyrics Tag
*/
class ARLyrics : public ARMTParameter, public ARPositionTag
{
public:

	friend class ARText;
	
					ARLyrics();
	virtual		 	~ARLyrics(); 

	virtual void setTagParameterList(TagParameterList & theTagParameterList);
	
	virtual const char* getText() const;
	
	virtual void print() const;
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;
	
	virtual int MatchListOfTPLsWithTPL(const ListOfTPLs & ltpls,TagParameterList & tpl,
							TagParameterList ** rtpl);


	virtual void  CreateListOfTPLs( ListOfTPLs & ltpl, ListOfStrings & lstrs);
	
	const char* getTextformat() const;
	const char* getFont() const;
	const char* getFAttrib() const;
	int getFSize(float curLSPACE = LSPACE) const;

protected:

	TagParameterString * mText; // the corresponding text ...
	TagParameterString * mTextFormat;
	TagParameterString * mFont;
	TagParameterFloat  * mFSize;
	TagParameterString * mFAttrib;

	static ListOfTPLs ltpls;

};

#endif

#ifndef ARStaffFormat_H
#define ARStaffFormat_H

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

/** \brief not yet documented
*/
class ARStaffFormat : public ARMTParameter
{
public:
	ARStaffFormat();
	ARStaffFormat(const ARStaffFormat &);
	virtual ~ARStaffFormat();

	virtual bool IsStateTag() const		{ return true; }
	
	virtual void setTagParameterList( TagParameterList& tpl);
	virtual void print() const { }
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;

	const TagParameterString * getStyle() const		{ return style; }
	float getLineThickness() const					{ return fLineThickness; }

protected:
	TagParameterString *style;
	float				fLineThickness;
	static ListOfTPLs   ltpls;

};

#endif 

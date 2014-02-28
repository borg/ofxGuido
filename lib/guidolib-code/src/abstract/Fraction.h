#ifndef Fraction_H
#define Fraction_H

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

/** \brief Numerator and denominator
*/
class Fraction
{
  public:

		Fraction() : numerator(0), denominator(1), dval(0.0) { }

		Fraction( int theNumerator, int theDenominator );
		Fraction( double x );
		Fraction( int value ) : numerator(value), denominator(1)	{ dval = (double)value; }

		void print(std::ostream&) const;

		int getNumerator() const;
		int getDenominator() const;
		void normalize();

		// greatest common denominator
		static int gcd(int i1, int i2);
		
		// smallest common multiple
		static int scm(int i1, int i2);

		void set( int num, int denom = 1 );
		void setDenominator(int newDenominator);
		void setNumerator(int newNumerator);

		void invert(); 

		// test if fraction is like m/(n^k) is used for the tuplet creation
		int isMultiple(int n);		// Result: exponent (>=0) or -1

		bool 	isOdd() const;
		bool	isEven() const;

      Fraction getBiggestFullNote(int exp) const;
      Fraction getReallySmallerNote(int exp) const;

		operator double() const;
		operator float() const;
		inline operator bool() const				{ return numerator != 0; }
		bool operator >(const Fraction & in) const  { return (dval > in.dval); }
		bool operator ==(const Fraction & in) const { return (dval == in.dval); }
		bool operator !=(const Fraction & in) const { return (dval != in.dval); }
		bool operator <(const Fraction & in) const  { return (dval < in.dval); }
		bool operator >=(const Fraction & in) const { return (dval >= in.dval); }
		bool operator <=(const Fraction & in) const { return (dval <= in.dval); }
		Fraction & operator +=(const Fraction &tmp);
		Fraction & operator -=(const Fraction &tmp);
		Fraction & operator %=(const Fraction &tmp);
 		Fraction & operator *=(const Fraction &tmp);
		Fraction & operator *=(const int tmp);

	private:
		int numerator;
		int denominator;
		double dval;
};

std::ostream& operator<< (std::ostream& os, const Fraction& f);


//## Other Operations (inline)
inline Fraction::operator float() const			{ return (float)dval; }
inline Fraction::operator double() const		{ return dval; }

inline int Fraction::getNumerator() const		{ return numerator; }
inline int Fraction::getDenominator() const		{ return denominator; }

Fraction operator +(const Fraction & tmp1, const Fraction & tmp);
Fraction operator -(const Fraction & tmp1, const Fraction & tmp);
Fraction operator *(const Fraction & tmp1, const Fraction & tmp);
Fraction operator *(const Fraction & tmp1, const int multiplier);
Fraction operator %(const Fraction & tmp1, const Fraction & tmp2);

extern const  Fraction Frac_n1;
extern const  Fraction Frac_0;
extern const  Fraction Frac_7_4;
extern const  Fraction Frac_3_2;
extern const  Fraction Frac_1;
extern const  Fraction Frac_7_8;
extern const  Fraction Frac_3_4;
extern const  Fraction Frac_1_2;
extern const  Fraction Frac_7_16;
extern const  Fraction Frac_3_8;
extern const  Fraction Frac_1_4;
extern const  Fraction Frac_7_32;
extern const  Fraction Frac_3_16;
extern const  Fraction Frac_1_8;
extern const  Fraction Frac_7_64;
extern const  Fraction Frac_3_32;
extern const  Fraction Frac_3_64;
extern const  Fraction Frac_3_128;
extern const  Fraction Frac_7_128;
extern const  Fraction Frac_7_256;
extern const  Fraction Frac_1_12;
extern const  Fraction Frac_1_16;
extern const  Fraction Frac_1_32;
extern const  Fraction Frac_1_64;
extern const  Fraction Frac_1_128;
extern const  Fraction Frac_Max;


#endif


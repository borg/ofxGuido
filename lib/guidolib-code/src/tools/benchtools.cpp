/*
  GUIDO Library
  Copyright (C) 2010 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "benchtools.h"

#ifdef WIN32
# include <windows.h>
#else
# include <sys/time.h>
#endif

using namespace std;


//_________________________________________________________________________________
// Returns the number of clock cycles elapsed since the last reset of the processor
//_________________________________________________________________________________
//#if !defined(WIN32) || defined(__MINGW32__)
//__uint64  rdtsc(void)
//{
//	union {
//		__uint32 i32[2];
//		__uint64 i64;
//	} count;
//
//	__asm__ __volatile__("rdtsc" : "=a" (count.i32[0]), "=d" (count.i32[1]));
//
//     return count.i64;
//}
//#else
//__uint64 __cdecl rdtsc(void)
//{
//   __asm {
//		XOR eax, eax
//		CPUID
//		rdtsc
//	}
//}
//
//#endif

#define kTimeOffset  60*60*24*365*40
//_________________________________________________________________________________
// time returned as usec
// (absolute time not exactly known but could be computed from time offset)
//_________________________________________________________________________________
#ifdef WIN32
__uint64  getTime (void)	{ return GetTickCount() * 1000; }
#else
__uint64  getTime(void)
{
	struct timeval time;
	if (gettimeofday(&time, 0) == 0)
		return ((time.tv_sec - kTimeOffset) * 1000000) + time.tv_usec;
	return 0;
}
#endif

//_________________________________________________________________________________
// estimates the count of ticks in a usec
//_________________________________________________________________________________
/*
float  bench::usec2ticks(void)
{
	static float gUSec2ticks = 0;

	if (!gUSec2ticks) {
		__uint64 start = rdtsc();
		__uint64 t1 = getTime();
		while ( (getTime() - t1) < 400000)
			;
		__uint64 ticks = rdtsc() - start;
		gUSec2ticks= ticks / 400000.f;
	}
	return gUSec2ticks;
}

//_________________________________________________________________________________
void  bench::init(const char* sigs[], unsigned short size)
{
	usec2ticks();			// initializes the usec to ticks value
	gBenchs.clear();
	unsigned int i = 0;
	while (sigs[i] ) {
		gBenchs.push_back( BenchUnit(sigs[i], TSignal::create(sigs[i], size, 0.f)));
		i++;
	}
}

//_________________________________________________________________________________
static STSignal get (const string& name, const vector<BenchUnit>& from)
{
	for (unsigned int i = 0; i < from.size(); i++)
		if (from[i].first == name) return from[i].second;
	return 0;
}

//_________________________________________________________________________________
void bench::put(const string& sig, float val)
{
	if (gRunning) {
		STSignal s = get(sig, gBenchs);
		if (s) *s << val;
		else cerr << "bench::put - no bench named \"" << sig << "\"" << endl;
	}
}

//_________________________________________________________________________________
void  bench::start()		{ gRunning = true; }
void  bench::stop()			{ gRunning = false; }
bool  bench::running()		{ return gRunning; }

#define kFieldSep	" "
//_________________________________________________________________________________
void  bench::write(const std::string& name)
{
	ofstream file (name.c_str());
	int avail = 0;
	for (unsigned int i = 0; i < gBenchs.size(); i++) {
		if (gBenchs[i].second->available() > avail) avail = gBenchs[i].second->available();
		file << gBenchs[i].first << kFieldSep;
	}
	file << endl;
	for (int n = 0; n < avail; n++) {
		for (unsigned int i = 0; i < gBenchs.size(); i++)
			file << (int)(*(gBenchs[i].second))[n] << kFieldSep;
		file << endl;
	}
	file.close();
}
*/

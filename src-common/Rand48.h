// Rand48.h
// this file is part of Context Free
// ---------------------
// Copyright (C) 2008 John Horigan - john@glyphic.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
// 
// John Horigan can be contacted at john@glyphic.com or at
// John Horigan, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//


#ifndef INCLUDE_RAND48_H
#define INCLUDE_RAND48_H

#include <limits.h>
// If limits.h defines ULLONG_MAX then use the unsigned long long version.
// Otherwise use the unsigned short[3] version

class Rand48 {
public:
#ifdef ULLONG_MAX
    unsigned long long mSeed;
#else
    unsigned short  mSeed0;
    unsigned short  mSeed1;
    unsigned short  mSeed2;
#endif    
    static Rand48   Common;
    static bool     BigEndian;
    
#ifdef ULLONG_MAX
    Rand48(): mSeed(0) {}
    Rand48(unsigned short seed0, unsigned short seed1, unsigned short seed2)
        { seed(seed0, seed1, seed2); }
#else
    Rand48(): mSeed0(0), mSeed1(0), mSeed2(0) {}
    Rand48(unsigned short seed0, unsigned short seed1, unsigned short seed2)
    : mSeed0(seed0), mSeed1(seed1), mSeed2(seed2) {}
#endif
    Rand48(long seed) { setRand(seed); }
    
    double getDouble(bool bump = true);
    long getLong(bool bump = true);
    long getPositive(bool bump = true);
    unsigned long getUnsigned(bool bump = true);
    
    void seed(unsigned short seed0, unsigned short seed1, unsigned short seed2);
    void seed(const unsigned short* seed);
    void setRand(long seed);
    
    Rand48& operator^=(const Rand48& r)
    {
#ifdef ULLONG_MAX
        mSeed ^= r.mSeed;
#else
        mSeed0 ^= r.mSeed0;
        mSeed1 ^= r.mSeed1;
        mSeed2 ^= r.mSeed2;
#endif
        return *this;
    };
    void xorChar(char c, unsigned i);
    void bump();
};

#endif  // INCLUDE_RAND48_H
    
    

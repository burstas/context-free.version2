// Rand48.cpp
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


#include "Rand48.h"
#include <math.h>

#define	RAND48_SEED_0	(0x330e)
#define	RAND48_SEED_1	(0xabcd)
#define	RAND48_SEED_2	(0x1234)
#define	RAND48_MULT_0	(0xe66d)
#define	RAND48_MULT_1	(0xdeec)
#define	RAND48_MULT_2	(0x0005)
#define RAND48_MULT     (0x0005deece66dLL)
#define	RAND48_ADD	(0x000b)

Rand48 Rand48::Common(RAND48_SEED_0, RAND48_SEED_1, RAND48_SEED_2);
bool Rand48::BigEndian = false;

double Rand48::getDouble(bool doBump)
{
    if (doBump) bump();
#ifdef ULLONG_MAX
	return  ldexp((double) mSeed, -48);
#else
	return  ldexp((double) mSeed0, -48) +
            ldexp((double) mSeed1, -32) +
            ldexp((double) mSeed2, -16);
#endif
}

long Rand48::getLong(bool doBump)
{
    if (doBump) bump();
#ifdef ULLONG_MAX
    return (long) (mSeed >> 16);
#else
    return ((long) mSeed2 << 16) + (long) mSeed1;
#endif
}

long Rand48::getPositive(bool doBump)
{
    if (doBump) bump();
#ifdef ULLONG_MAX
    return (long) (mSeed >> 17);
#else
    return ((long) mSeed2 << 15) + ((long) mSeed1 >> 1);
#endif
}

unsigned long Rand48::getUnsigned(bool doBump)
{
    if (doBump) bump();
#ifdef ULLONG_MAX
    return (unsigned long) (mSeed >> 16);
#else
    return ((unsigned long) mSeed2 << 16) + (unsigned long) mSeed1;
#endif
}

void Rand48::seed(unsigned short seed0, unsigned short seed1, unsigned short seed2)
{
#ifdef ULLONG_MAX
    mSeed =  (unsigned long long)seed0 | 
            ((unsigned long long)seed1) << 16 |
            ((unsigned long long)seed2) << 32;
#else
    mSeed0 = seed0; mSeed1 = seed1; mSeed2 = seed2;
#endif
}

void Rand48::seed(const unsigned short* seed)
{
#ifdef ULLONG_MAX
    mSeed =  (unsigned long long)seed[0] | 
    ((unsigned long long)seed[1]) << 16 |
    ((unsigned long long)seed[2]) << 32;
#else
    mSeed0 = seed[0]; mSeed1 = seed[1]; mSeed2 = seed[2];
#endif
}

void Rand48::setRand(long seed)
{
#ifdef ULLONG_MAX
    mSeed = ((unsigned long long)RAND48_SEED_0 << 32) + (unsigned long long)seed;
#else
	mSeed0 = RAND48_SEED_0;
	mSeed1 = (unsigned short) seed;
	mSeed2 = (unsigned short) (seed >> 16);
#endif
}

void Rand48::xorChar(char c, unsigned i)
{
    if (BigEndian) i ^= 1;
#ifdef ULLONG_MAX
    mSeed ^= ((unsigned long long)c) << (i * 8);
#else
    unsigned offset = (i & 1) * 8;
    switch (i) {
        case 0:
        case 1:
            mSeed0 ^= ((unsigned short)c) << offset;
            break;
        case 2:
        case 3:
            mSeed1 ^= ((unsigned short)c) << offset;
            break;
        case 4:
        case 5:
            mSeed2 ^= ((unsigned short)c) << offset;
            break;
    }
#endif
}

void Rand48::bump()
{
#ifdef ULLONG_MAX
    mSeed = (mSeed * RAND48_MULT + (unsigned long long) RAND48_ADD) & 0xffffffffffffLL;
#else
	unsigned long accu;
	unsigned short temp[2];
    
	accu = (unsigned long) RAND48_MULT_0 * (unsigned long) mSeed0 +
    (unsigned long) RAND48_ADD;
	temp[0] = (unsigned short) accu;	/* lower 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += (unsigned long) RAND48_MULT_0 * (unsigned long) mSeed1 +
    (unsigned long) RAND48_MULT_1 * (unsigned long) mSeed0;
	temp[1] = (unsigned short) accu;	/* middle 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += RAND48_MULT_0 * mSeed2 + RAND48_MULT_1 * mSeed1 + RAND48_MULT_2 * mSeed0;
	mSeed0 = temp[0];
	mSeed1 = temp[1];
	mSeed2 = (unsigned short) accu;
#endif
}


// primShape.cpp
// Context Free
// ---------------------
// Copyright (C) 2006 John Horigan
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

#include "primShape.h"
#include <math.h>
#include "agg_basics.h"

using namespace agg;

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

static const point_d squareV[4] = { 
    point_d( 0.5,  0.5),
    point_d(-0.5,  0.5),
    point_d(-0.5, -0.5),
    point_d( 0.5, -0.5)
};

primShape primShape::square(squareV, 4);

static const double h = 0.5 / cos(M_PI/6.0);
static const double hp = h;
static const double hn = -h / 2.0;

static const point_d triV[3] = { 
    point_d( 0.0, hp),
    point_d(-0.5, hn),
    point_d( 0.5, hn)
};

primShape primShape::triangle(triV, 3);

static const double t = sqrt(2.0) / 4.0;

static const point_d circleV[8] = { 
    point_d( 0.5,  0.0),
    point_d(   t,    t),
    point_d( 0.0,  0.5),
    point_d(  -t,    t),
    point_d(-0.5,  0.0),
    point_d(  -t,   -t),
    point_d( 0.0, -0.5),
    point_d(   t,   -t)
};
primShape primShape::circle(circleV, 8);

primShape* primShape::shapeMap[numTypes] = { 
    &primShape::circle, 
    &primShape::square, 
    &primShape::triangle,
    0,
    0
};

bool primShape::isPrimShape(agg::path_storage* p)
{
    return (p == &circle) || (p == &square) || (p == &triangle);
}


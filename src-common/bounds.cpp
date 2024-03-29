// bounds.cpp
// this file is part of Context Free
// ---------------------
// Copyright (C) 2006-2007 Mark Lentczner - markl@glyphic.com
// Copyright (C) 2006-2008 John Horigan - john@glyphic.com
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
// Mark Lentczner can be contacted at markl@glyphic.com or at
// Mark Lentczner, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//


#include "bounds.h"

#include "agg_trans_affine.h"
#include "primShape.h"
#include "shape.h"
#include "agg_basics.h"

double Bounds::mScale;

void Bounds::setScale(double scale)
{
    mScale = scale;
}


Bounds::Bounds(const agg::trans_affine& trans, 
               agg::path_storage* path, pathAttr* attr, double dilation,
               double scale)
{
    double centx = attr->mCentroid.x;
    double centy = attr->mCentroid.y;
    trans.transform(&centx, &centy);

    mValid = attr->boundingRect(path, trans, mMin_X, mMin_Y, mMax_X, mMax_Y, scale);
    
    if (mValid && mMin_X <= centx && mMax_X >= centx &&
                  mMin_Y <= centy && mMax_Y >= centy && dilation != 1.0)
    {
        mMin_X = dilation * (mMin_X - centx) + centx;
        mMax_X = dilation * (mMax_X - centx) + centx;
        mMin_Y = dilation * (mMin_Y - centy) + centy;
        mMax_Y = dilation * (mMax_Y - centy) + centy;
    }
}

void
Bounds::merge(const Bounds& other)
{
	if (!other.mValid) return;
	
    if (!mValid) {
        mMax_X = other.mMax_X;
        mMin_X = other.mMin_X;
        mMax_Y = other.mMax_Y;
        mMin_Y = other.mMin_Y;
        mValid = true;
		return;
    }

	if (mMax_X < other.mMax_X) mMax_X = other.mMax_X;
	if (mMin_X > other.mMin_X) mMin_X = other.mMin_X;
	if (mMax_Y < other.mMax_Y) mMax_Y = other.mMax_Y;
	if (mMin_Y > other.mMin_Y) mMin_Y = other.mMin_Y;
}

Bounds
Bounds::interpolate(const Bounds& other, double alpha) const
{
	double beta = 1.0 - alpha;
	
	Bounds r;
	if (!mValid || !other.mValid) return r;
	
	r.mMax_X = beta * mMax_X + alpha * other.mMax_X;
	r.mMin_X = beta * mMin_X + alpha * other.mMin_X;
	r.mMax_Y = beta * mMax_Y + alpha * other.mMax_Y;
	r.mMin_Y = beta * mMin_Y + alpha * other.mMin_Y;
	r.mValid = true;
	
	return r;
}

Bounds
Bounds::slewCenter(const Bounds& other, double alpha) const
{
	Bounds r;
	if (!mValid || !other.mValid) return r;
	
	double offsetX
		= alpha * ((other.mMax_X + other.mMin_X) - (mMax_X + mMin_X)) / 2.0;
	double offsetY
		= alpha * ((other.mMax_Y + other.mMin_Y) - (mMax_Y + mMin_Y)) / 2.0;

	double absX = fabs(offsetX);
	double absY = fabs(offsetY);
	
	r.mMax_X = mMax_X + absX + offsetX;
	r.mMin_X = mMin_X - absX + offsetX;
	r.mMax_Y = mMax_Y + absY + offsetY;
	r.mMin_Y = mMin_Y - absY + offsetY;
	r.mValid = true;
	
	return r;
}

void
Bounds::gather(const Bounds& other, double weight)
{
	if (!other.mValid) return;
	
    if (!mValid) {
        mMax_X = weight * other.mMax_X;
        mMin_X = weight * other.mMin_X;
        mMax_Y = weight * other.mMax_Y;
        mMin_Y = weight * other.mMin_Y;
        mValid = true;
		return;
    }

	mMax_X += weight * other.mMax_X;
	mMin_X += weight * other.mMin_X;
	mMax_Y += weight * other.mMax_Y;
	mMin_Y += weight * other.mMin_Y;
}

double
Bounds::computeScale(int& width, int& height, double border,
			bool modify, agg::trans_affine* trans, bool exact)
{
    double scale;
    double virtual_width = mMax_X - mMin_X;
    double virtual_height = mMax_Y - mMin_Y;
    double target_width = width - 2.0 * border;
    double target_height = height - 2.0 * border;
    	
    if (!mValid) virtual_width = virtual_height = 1.0;
    
	int newWidth = width;
	int newHeight = height;
	
    if (virtual_width / target_width >
        virtual_height / target_height)
	{
        scale = target_width / virtual_width;
		newHeight = (int)floor(scale * virtual_height + 2.0 * border + 0.5);
		if (!exact)
            newHeight = newHeight + ((newHeight ^ height) & 0x1);

        if (modify) {
			height = newHeight;
		}
    }
    else {
        scale = target_height / virtual_height;
		newWidth = (int)floor(scale * virtual_width + 2.0 * border + 0.5);
		if (!exact)
            newWidth = newWidth + ((newWidth ^ width) & 0x1);
		
		if (modify) {
			width = newWidth;
		}
    }

    if (trans) {
        double offsetX = scale * (mMax_X + mMin_X) / 2.0 - newWidth / 2.0;
        double offsetY = scale * (mMax_Y + mMin_Y) / 2.0 - newHeight / 2.0;

        agg::trans_affine_scaling sc(scale);
        agg::trans_affine_translation tr(-offsetX, -offsetY);
        *trans = sc;
        *trans *= tr;
    }
	
	return scale;
}

void
Bounds::update(const agg::trans_affine& trns, 
               agg::path_storage* path, pathAttr* attr, double scale)
{
    Bounds b(trns, path, attr, 1.0, scale);
    merge(b);
}


// bounds.h
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


#ifndef INCLUDE_BOUNDS_H
#define INCLUDE_BOUNDS_H

#include "agg_path_storage.h"

namespace agg { struct trans_affine; }
struct pathAttr;

class Bounds {
    public:
        Bounds() : mValid(false) {}
		
		Bounds(const agg::trans_affine& trans,
			agg::path_storage* path, pathAttr* attr, double dilation = mScale,
            double scale = 1.0);
			// set bounds to be the bounds of this shape, transformed
		
		bool valid() { return mValid; }
		
		void merge(const Bounds&);
			// merge the other bounds into this bounds
		
		Bounds operator+(const Bounds& other)
			{ Bounds t(*this); t.merge(other); return t; }
			
		Bounds& operator+=(const Bounds& other)
			{ merge(other); return *this; }
		
		Bounds interpolate(const Bounds& other, double alpha) const;
			// compute the interpolation between this bounds and the other
			// an alpha of 0.0 has no effect, an alpha of 1.0 give the other
		Bounds slewCenter(const Bounds& other, double alpha) const;
			// compute a new bounds who's center is slewed alpha of the way
			// toward the other center, yet incorporates all of the current
			// bounds
			
		double computeScale(int& width, int& height, double border,
					bool modify = false, agg::trans_affine* trans = 0,
                    bool exact = false);
			// Computes the scale factor of fitting this bounds into a canvas
			// of the given width and height, with the provided fixed border.
			// If modify is true, width and height are reset to the scaled size.
			// If trans isn't null, it is set to the needed transformation.
            // If exact is true then the diensions are not bumped to preserve parity.

        static void setScale(double scale);
    
		void gather(const Bounds& other, double weight);
		
        void update(const agg::trans_affine& trns, 
                    agg::path_storage* path, pathAttr* attr, double scale);
    
    double  mMin_X, mMin_Y, mMax_X, mMax_Y;
        bool    mValid;
private:
    static double mScale;
};

#endif // INCLUDE_BOUNDS_H

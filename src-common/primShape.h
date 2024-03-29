// primShape.h
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

#ifndef INCLUDE_PRIMSHAPE_H
#define INCLUDE_PRIMSHAPE_H

#include "agg_path_storage.h"

class primShape : public agg::path_storage
{
	public:
		enum {circleType = 0, squareType = 1, triangleType = 2, 
        loopStartType = 3, loopEndType = 4, numTypes = 5};
        primShape(const agg::point_d* p, unsigned s)
        {
            move_to(p[0].x, p[0].y);
            for (unsigned i = 1; i < s; ++i) line_to(p[i].x, p[i].y);
            end_poly(agg::path_flags_close);
        }
        primShape() {}
    
		static primShape circle;
		static primShape square;
		static primShape triangle;
        static primShape* shapeMap[numTypes];
        static bool isPrimShape(unsigned v) { return v < numTypes; }
        static bool isPrimShape(agg::path_storage* p);
		
	private:
        primShape& operator=(primShape& s);
};
	
#endif // INCLUDE_PRIMSHAPE_H

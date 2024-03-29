// upload.h
// this file is part of Context Free
// ---------------------
// Copyright (C) 2006 Mark Lentczner - markl@glyphic.com
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
// Mark Lentczner can be contacted at markl@glyphic.com or at
// Mark Lentczner, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//

#ifndef AGG_COPY_RECT_INCLUDED
#define AGG_COPY_RECT_INCLUDED

#include "agg_color_gray.h"
#include "agg_color_rgba.h"
#include "agg_renderer_base.h"

namespace agg
{

	template<class FromColor, class ToColor>
	class ColorConverter
	{
	public:
		ToColor f(const FromColor& c)
			{ return c; }
	};

	template <>
        agg::rgba8
        ColorConverter<agg::gray8, agg::rgba8>::f(const agg::gray8& c)
	{
            return agg::rgba8(c.v, c.v, c.v, c.a);
	}
    
    
	template<class SrcPixelFormat, class DstPixelFormat>
	void copy_rect(
		SrcPixelFormat& srcFormat, 
		DstPixelFormat& dstFormat, 
		const rect_i* rect_src_ptr = 0, 
		int dst_dx = 0, 
		int dst_dy = 0
		)
	{
		renderer_base<SrcPixelFormat> srcRenderer(srcFormat);
		renderer_base<DstPixelFormat> dstRenderer(dstFormat);
		srcRenderer.reset_clipping(true);
		dstRenderer.reset_clipping(true);
		
		rect_i rsrc(0, 0, srcRenderer.width(), srcRenderer.height());
		if(rect_src_ptr)
		{
			rsrc.x1 = rect_src_ptr->x1; 
			rsrc.y1 = rect_src_ptr->y1;
			rsrc.x2 = rect_src_ptr->x2 + 1;
			rsrc.y2 = rect_src_ptr->y2 + 1;
		}

		rect_i rdst(rsrc.x1 + dst_dx, rsrc.y1 + dst_dy,
				  rsrc.x2 + dst_dx, rsrc.y2 + dst_dy);

		rect_i rc = dstRenderer.clip_rect_area(rdst, rsrc,
			srcRenderer.width(), srcRenderer.height());
			// this rect is 0,0,w,h
			
		if(rc.x2 <= rc.x1  ||  rc.y2 <= rc.y1)
			return;
		
		typedef typename SrcPixelFormat::color_type SrcColor;
		typedef typename DstPixelFormat::color_type DstColor;
		ColorConverter<SrcColor, DstColor> cvt;
			
		// no need to use renderers to do this, everything has been clipped
		for (int y = 0; y < rc.y2; ++y) {
			for (int x = 0; x < rc.x2; ++x) {
				dstFormat.copy_pixel(rdst.x1 + x, rdst.y1 + y,
					cvt.f(srcFormat.pixel(rsrc.x1 + x, rsrc.y1 + y)));
			}
		}
	}
}

#endif // AGG_COPY_RECT_INCLUDED

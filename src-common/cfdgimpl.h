// cfdgimpl.h
// this file is part of Context Free
// ---------------------
// Copyright (C) 2006-2008 Mark Lentczner - markl@glyphic.com
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


#ifndef INCLUDE_CFDGIMPL_H
#define INCLUDE_CFDGIMPL_H

#include <string>
#include <vector>

#include "agg_color_rgba.h"
#include "cfdg.h"
#include "shape.h"

class CFDGImpl : public CFDG {
    public:
        enum {newShape = 0, ruleType = 1, pathType = 2};
    private:
        Shape m_initialShape;
        bool mBackgroundSet;

        std::vector<Rule> mRules;
        
        struct ShapeType {
            std::string  name;
            bool    hasRules;
            int    shapeType;
            
            ShapeType(const std::string& s) 
            : name(s), hasRules(false), shapeType(newShape) { }
        };
        
        std::vector<ShapeType> m_shapeTypes;
        
        AbstractSystem* m_system;

        int m_Parameters;

        double m_minSize;
        double m_minArea;
        
        agg::trans_affine mTileTransform;
        double mTileX, mTileY;
        bool mTiled;
        bool mSized;
        agg::point_d mTileOffset;
        
    public:
        CFDGImpl(AbstractSystem*);
        virtual ~CFDGImpl();
        
        virtual Renderer* renderer(
                int width, int height, float minSize,
                int variation, double border = 2.0);
        
        bool isTiled(agg::trans_affine* tr = 0, double* x = 0, double* y = 0) const;
        bool isSized(double* x = 0, double* y = 0) const;

    public:
        AbstractSystem* system() { return m_system; }
        
        void            setInitialShape(const Shape& s);
        const Shape&    getInitialShape();
        
        void addRule(const Rule& r, bool isShape);
        void rulesLoaded();
        int numRules();
        Rule * findRule(int shapetype, double r);

        std::string  decodeShapeName(int shapetype);
        int     encodeShapeName(const std::string& s);
        int     getShapeType(const std::string& s);
        int     getShapeType(int shapetype);
        bool    shapeHasRules(int shapetype);

        enum Parameter {Color = 1, Alpha = 2};
        void addParameter(Parameter);

        void setBackgroundColor(const agg::rgba& bk);
        void setTiled(const agg::trans_affine& tr, const double& x, const double& y);
        void setSized(const agg::trans_affine& tr);
};



#endif // INCLUDE_CFDGIMPL_H

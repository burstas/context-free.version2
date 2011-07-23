/*
 *

 Context Free Design Grammar - version 0.2

 Copyright (C) 2005 Chris Coyne - ccoyne77@gmail.com
 Copyright (C) 2005 Mark Lentczner - markl@glyphic.com
 Copyright (C) 2005 John Horigan - john@glyphic.com

 [Send me anything cool you make with it or of it.]

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published
 by the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 02111-1307  USA

 *
 */


#include "cfdg.h"

#include <string>

#include "builder.h"
#include "cfdgimpl.h"
#include "yglue.h"
#include <string.h>
#include <math.h>

using namespace std;




AbstractSystem::~AbstractSystem() { }

void AbstractSystem::stats(const Stats&)
    { }

Canvas::~Canvas() { }

Renderer::~Renderer() { }



CFDG::~CFDG() { }


extern "C" {
    int yyparse();
}

Builder* builder = 0;

CFDG*
CFDG::ParseFile(const char* firstFile, AbstractSystem* system, int variation)
{
    Builder b(new CFDGImpl(system), variation);
    builder = &b;

    yg_Reset();
    
    b.m_filesToLoad.push(firstFile);

    while (!b.m_filesToLoad.empty()) {
        string fname = b.m_filesToLoad.front();
        b.m_filesToLoad.pop();
        
        b.m_input = system->openFileForRead(fname);
        if (!b.m_input || !b.m_input->good()) {
			delete b.m_input;
			b.m_input = 0;
            system->message(true, "Couldn't open rules file %s", fname.c_str());
            return 0;
        }
        
        b.m_currentPath = fname;
        system->message(false, "Reading rules file %s", fname.c_str());

        yg_ResetForNextFile();
        
        int yyresult = yyparse();

        if (b.m_input != &cin) delete b.m_input;
        b.m_input = 0;
        
        if (yyresult != 0)
            return 0;
    }
    
    if (b.m_CFDG->getInitialShape().mShapeType < 0) {
        system->message(true, "No startshape declared.");
        return 0;
    }

    b.m_CFDG->rulesLoaded();
    system->message(false, "%d rules loaded", b.m_CFDG->numRules());
    
    CFDG* result = b.m_CFDG;
    b.m_CFDG = 0;
    builder = 0;
    return result;
}


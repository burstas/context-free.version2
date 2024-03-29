// tempfile.cpp
// this file is part of Context Free
// ---------------------
// Copyright (C) 2006-2008 Mark Lentczner - markl@glyphic.com
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


#include "tempfile.h"

#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include <iostream>

using namespace std;


ref_ptr<TempFile>
TempFile::build(AbstractSystem* system, const char* prefix, const char* type, int num)
{
    return Countable::build(new TempFile(system, prefix, type, num));
}


std::ostream*
TempFile::forWrite()
{
    if (mWritten) {
        cerr << "TempFile::forWrite already wrote to " << mPath << endl;
    }
    mWritten = true;
    mSystem->message(false, "Writing %s temp file %d", mType.c_str(), mNum);
    return mSystem->tempFileForWrite(mPath);
}

std::istream*
TempFile::forRead()
{
    if (!mWritten) {
        cerr << "TempFile::forRead temp file never written, " << mPath << endl;
    }
    mSystem->message(false, "Reading %s temp file %d", mType.c_str(), mNum);
    return mSystem->tempFileForRead(mPath);
}

TempFile::TempFile(AbstractSystem* system, const char* prefix, const char* type, int num)
    : mSystem(system), mPath(prefix), mType(type), mNum(num), mWritten(false)
    { }

TempFile::~TempFile()
{
    if (mWritten) {
        mSystem->message(false, "Deleting %s temp file %d", mType.c_str(), mNum);
        unlink(mPath.c_str());
    }
}


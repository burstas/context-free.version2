// posixSystem.cpp
// Context Free
// ---------------------
// Copyright (C) 2005-2007 Mark Lentczner - markl@glyphic.com
// Copyright (C) 2007-2008 John Horigan - john@glyphic.com
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
// John Horigan can be contacted at john@glyphic.com or at
// John Horigan, 1209 Villa St., Mountain View, CA 94041-1123, USA
//
//

#include "posixSystem.h"

#include <cstdlib>
#include "stdarg.h"

#include <string>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;


void
PosixSystem::message(bool error, const char* fmt, ...)
{
    mError = mError || error;
    
	if (mQuiet && !error) return;

	char buf[256];
	{
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, sizeof(buf), fmt, args);
		buf[sizeof(buf)-1] = '\0';
		va_end(args);
	}
	
	ostream& out = error ? cerr : cout;
	out << buf << "                            " << endl;
}

void
PosixSystem::syntaxError(const std::string&, int)
{
  // this version doesn't high-light the line in error
  // the parser will have already called message() with
  // the syntax error information
}

istream*
PosixSystem::openFileForRead(const string& path)
{
    if (path == "-") return &cin;
	return new ifstream(path.c_str(), ios::binary);
}

istream*
PosixSystem::tempFileForRead(const string& path)
{
	return new ifstream(path.c_str(), ios::binary);
}

ostream*
PosixSystem::tempFileForWrite(string& prefixInNameOut)
{
#ifdef WIN32
    char* path = _tempnam(0, prefixInNameOut.c_str());
    prefixInNameOut = path;
    ofstream* f = new ofstream(path, ios::binary | ios::out | ios::trunc);
    free((void*)path);
    return f;
#else
	string t = "/tmp/" + prefixInNameOut + "XXXXXX";
	char* b = new char[t.size() + 1];
	strcpy(b, t.c_str());
	
	ofstream* f = 0;
	
	// Please ignore the warning about mktemp() being unsafe. There is no 
	// standard way to turn the descriptor returned by mkstemp() into an
	// ostream object. We could add the boost library to the project (NO!!!!)
	// or roll our own file descriptor ostream class (code exists on the
	// internet). But it just doesn't seem worth the bother. mktemp() is 
	// unsafe for some types of programs, but who cares if an attacker takes
	// down your Context Free.
	if (mktemp(b)) {
		f = new ofstream;
		f->open(b, ios::binary);
		prefixInNameOut = b;
	}
	delete b;
	
	return f;
#endif
}
		
string
PosixSystem::relativeFilePath(const string& base, const string& rel)
{
	string s = base;
	
	string::size_type i = s.rfind('/');
	if (i == string::npos) {
		return rel;
	}
	i += 1;
	s.replace(i, s.length() - i, rel);	
	return s;
}

void
PosixSystem::stats(const Stats& s)
{
	if (mQuiet || mError) return;

    cout << "    " << s.shapeCount << " shapes";

    if (s.toDoCount > 0)
        cout << " - " << s.toDoCount << " expansions to do";
    else
        cout << "                            ";

    cout << "        \r";
	cout.flush();
}



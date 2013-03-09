/*!
  \copyright (c) RDO-Team, 2011
  \file      rdostream.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      05.07.2009
  \brief     
  \indent    4T
*/

#ifndef _UTILS_RDOSTREAM_H_
#define _UTILS_RDOSTREAM_H_

// ----------------------------------------------------------------------- INCLUDES
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

typedef  std::basic_istream<tchar, std::char_traits<tchar> >   istream;
typedef  std::basic_ostream<tchar, std::char_traits<tchar> >   ostream;
typedef  std::basic_ifstream<tchar, std::char_traits<tchar> >  ifstream;
typedef  std::basic_ofstream<tchar, std::char_traits<tchar> >  ofstream;
typedef  std::basic_stringstream<tchar>                        stringstream;
typedef  std::basic_ostringstream<tchar>                       ostringstream;

// --------------------------------------------------------------------------------
// -------------------- stream
// --------------------------------------------------------------------------------
class stream: public rdo::stringstream
{
public:
	stream(ios_base::openmode mode)
		: rdo::stringstream(mode)
	{}
	virtual rbool isBinary() const = 0;
};

// --------------------------------------------------------------------------------
// -------------------- binarystream
// --------------------------------------------------------------------------------
class binarystream: public stream
{
public:
	binarystream()
		: stream(ios_base::in | ios_base::out | ios_base::binary)
	{}
	virtual rbool isBinary() const { return true; }
};

// --------------------------------------------------------------------------------
// -------------------- textstream
// --------------------------------------------------------------------------------
class textstream: public stream
{
public:
	textstream()
		: stream(ios_base::in | ios_base::out)
	{}
	virtual rbool isBinary() const { return false; }
};

CLOSE_RDO_NAMESPACE

#endif // _UTILS_RDOSTREAM_H_

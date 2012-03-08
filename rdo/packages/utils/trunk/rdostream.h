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
#include <sstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- stream
// --------------------------------------------------------------------------------
class stream: public std::stringstream
{
public:
	stream(ios_base::openmode mode)
		: std::stringstream(mode)
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

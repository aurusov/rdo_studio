/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdointerface.h
 * author   : Урусов Андрей
 * date     : 05.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOSTREAM_H_
#define _RDOSTREAM_H_

// ====================================================================== INCLUDES
#include <sstream>
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- stream
// ----------------------------------------------------------------------------
class stream: public std::stringstream
{
public:
	stream(ios_base::openmode mode)
		: std::stringstream(mode)
	{}
	virtual bool isBinary() const = 0;
};

// ----------------------------------------------------------------------------
// ---------- binarystream
// ----------------------------------------------------------------------------
class binarystream: public stream
{
public:
	binarystream()
		: stream(ios_base::in | ios_base::out | ios_base::binary)
	{}
	virtual bool isBinary() const { return true; }
};

// ----------------------------------------------------------------------------
// ---------- textstream
// ----------------------------------------------------------------------------
class textstream: public stream
{
public:
	textstream()
		: stream(ios_base::in | ios_base::out)
	{}
	virtual bool isBinary() const { return false; }
};

CLOSE_RDO_NAMESPACE

#endif // _RDOSTREAM_H_

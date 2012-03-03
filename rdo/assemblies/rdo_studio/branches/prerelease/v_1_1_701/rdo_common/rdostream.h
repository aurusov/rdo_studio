#ifndef RDOSTREAM_H
#define RDOSTREAM_H

#if _MSC_VER > 1000
#pragma once
#endif

// ====================================================================== INCLUDES
#include <sstream>
// ====================================================================== SYNOPSIS
// ===============================================================================

namespace rdo {

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

}; // namespace rdo

#endif // RDOSTREAM_H

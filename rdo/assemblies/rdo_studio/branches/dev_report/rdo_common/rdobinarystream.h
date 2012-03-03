#ifndef RDOBINARYSTREAM_H
#define RDOBINARYSTREAM_H

#if _MSC_VER > 1000
#pragma once
#endif

//#pragma warning(disable: 4996)
//#pragma warning(disable: 4018)

#include <sstream>
#include <vector>

namespace rdo {

// ----------------------------------------------------------------------------
// ---------- binarystream
// ----------------------------------------------------------------------------
class binarystream: public std::stringstream
{
public:
	binarystream( ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary ):
		std::stringstream( mode )
	{
	}
	virtual bool isBinary() const { return true; }
};

// ----------------------------------------------------------------------------
// ---------- stringstream
// ----------------------------------------------------------------------------
class stringstream: public binarystream
{
public:
	stringstream(): binarystream( ios_base::in | ios_base::out ) {};
	virtual bool isBinary() const { return false; }
};

}; // namespace rdo

//#pragma warning(default: 4996)
//#pragma warning(default: 4018)

#endif // RDOBINARYSTREAM_H

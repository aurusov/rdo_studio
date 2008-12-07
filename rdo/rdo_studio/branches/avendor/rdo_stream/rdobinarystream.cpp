#include "rdobinarystream.h"

using namespace std;
using namespace rdo;

// ----------------------------------------------------------------------------
// ---------- binarystream
// ----------------------------------------------------------------------------
binarystream::binarystream( ios_base::openmode mode ): iostream( &buf )
{
	buf.stream   = this;
	buf.openmode = mode;
}

binarystream::binarybuf::binarybuf():
	streambuf(),
	current( 0 ),
	openmode( ios_base::in | ios_base::out | ios_base::binary )
{
	initPtr();
}

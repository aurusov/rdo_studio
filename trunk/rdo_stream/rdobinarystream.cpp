#include "rdobinarystream.h"

using namespace std;
using namespace rdo;

// ----------------------------------------------------------------------------
// ---------- binarystream
// ----------------------------------------------------------------------------
binarystream::binarystream( ios_base::openmode mode ): iostream( &buf )
{
	buf.openmode = mode;
}

binarystream::binarybuf::binarybuf():
	streambuf(),
	current( 0 ),
	openmode( ios_base::in | ios_base::out | ios_base::binary )
{
	setg( vec.begin(), vec.begin(), vec.end() );
	setp( vec.begin(), vec.end() );
}

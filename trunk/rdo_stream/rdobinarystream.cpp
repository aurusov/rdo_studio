#include "rdobinarystream.h"

using namespace std;
using namespace rdo;

// ----------------------------------------------------------------------------
// ---------- binarystream
// ----------------------------------------------------------------------------
binarystream::binarybuf::binarybuf():
	streambuf(),
	current( 0 )
{
	setg( vec.begin(), vec.begin(), vec.end() );
	setp( vec.begin(), vec.end() );
}

binarystream::binarystream(): iostream( &buf )
{
}

#include "stdafx.h"
#include "bkemul.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------
// ---------- BKEmul
// --------------------------------------------------------------
BKEmul emul;

BKEmul::BKEmul(): colorMonitor( !true )
{
	memory.resize( 64 * 1024 );
	std::vector< unsigned char >::iterator it = memory.begin();
	while ( it != memory.end() ) {
		(*it++) = 0;
	}
}

BKEmul::~BKEmul()
{
}

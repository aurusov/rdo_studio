#include "stdafx.h"
#include "bkemulmemory.h"
#include "bkemul.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKEmulMemory
// --------------------------------------------------------------
BKEmulMemory::BKEmulMemory()
{
	memory.resize( 64 * 1024 );
	std::vector< unsigned char >::iterator it = memory.begin();
	while ( it != memory.end() ) {
		(*it++) = 0xFF;
	}
}

BKEmulMemory::~BKEmulMemory()
{
}

void BKEmulMemory::clear()
{
	std::vector< unsigned char >::iterator it = memory.begin();
	while ( it != memory.end() ) {
		(*it++) = 0;
	}
}

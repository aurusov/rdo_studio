#include "stdafx.h"
#include "bkemulspeaker.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKEmulSpeaker
// --------------------------------------------------------------
BKEmulSpeaker::BKEmulSpeaker():
	count( 0 ),
	last_down( true )
{
}

BKEmulSpeaker::~BKEmulSpeaker()
{
}

void BKEmulSpeaker::reset()
{
	return;
	if ( check() ) {
		down();
	}
}

void BKEmulSpeaker::down()
{
	if ( !last_down ) count++;
	last_down = true;
	return;
	BYTE value = _inp( 0x61 );
	value |= 2;
	_outp( value, 0x61 );
}

void BKEmulSpeaker::up()
{
	if ( last_down ) count++;
	last_down = false;
	return;
	BYTE value = _inp( 0x61 );
	value &= 0xFC;
	_outp( value, 0x61 );
}

bool BKEmulSpeaker::check() const
{
	return true;
	BYTE value = _inp( 0x61 );
	return value & 2 ? true : false;
}

void BKEmulSpeaker::play()
{
	TRACE( "count = %d\r\n", count );
	count = 0;
}

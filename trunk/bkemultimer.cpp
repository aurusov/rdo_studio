#include "stdafx.h"
#include "bkemultimer.h"
#include "bkemul.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKEmulTimer
// --------------------------------------------------------------
BKEmulTimer::BKEmulTimer():
	type( none ),
	tick_count( 0 ),
	tick_koef( 1 ),
	next20( false )
{
}

BKEmulTimer::~BKEmulTimer()
{
}

void BKEmulTimer::reset()
{
	type       = none;
	tick_count = 0;
	tick_koef  = 1;
	next20     = false;
	emul.memory.set_word( 0177706, 0011000 ); // Начальные значения регистров системного таймера
	emul.memory.set_word( 0177710, 0177777 ); // ---- // ----
	emul.memory.set_word( 0177712, 0177400 ); // ---- // ----
}

void BKEmulTimer::tick()
{
	tick_count++;
	if ( tick_count % tick_koef == 0 ) {
		tick_count = 0;
		WORD data = emul.memory.get_word( 0177710 );
		if ( type == t20 && type == t24 ) {
			if ( next20 ) {
				next20 = false;
				data = emul.memory.get_word( 0177706 );
				emul.memory.set_word( 0177710, data );
			} else {
				data--;
				emul.memory.set_word( 0177710, data );
				if ( data == 1 ) {
					next20 = true;
					if ( type == t24 ) emul.cpu.setPR_100();
				}
			}
		} else if ( type == t22 ) {
			data--;
			emul.memory.set_word( 0177710, data );
		}
	}
}

void BKEmulTimer::set177712( WORD value )
{
	BYTE data = value;
	set177712( data );
	data = value >> 8;
	set177713( data );
}

void BKEmulTimer::set177712( BYTE value )
{
	emul.memory.set_word( 0177710, emul.memory.get_word( 0177706 ) );
	if ( value & 04 ) {
		type = t24;
		tick_koef = 8 * (value >> 4);
	} else if ( value & 02 ) {
		type = t22;
		tick_koef = 8 * (value >> 4);
	} else {
		type = t20;
		tick_koef = 8 * (value >> 4);
	}
/*
	if ( value == 020 ) {
		type = t20;
		tick_koef = 8;
	} else if ( value == 0120 ) {
		type = t120;
		tick_koef = 32;
	} else if ( value == 022 ) {
		type = t22;
		tick_koef = 7;
	} else if ( value == 0122 ) {
		type = t122;
		tick_koef = 28;
	} else {
		type = none;
	}
*/
	emul.memory.set_byte( 0177712, value );
}

void BKEmulTimer::set177713( BYTE value )
{
	// Запись в старший байт игнорируется
	emul.memory.set_byte( 0177713, 0xFF );
}

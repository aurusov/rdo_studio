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
	tick_count( 0 )
{
}

BKEmulTimer::~BKEmulTimer()
{
}

void BKEmulTimer::reset()
{
	type       = none;
	tick_count = 0;
	emul.memory.set_word( 0177706, 0011000 ); // Начальные значения регистров системного таймера
	emul.memory.set_word( 0177710, 0177777 ); // ---- // ----
	emul.memory.set_word( 0177712, 0177400 ); // ---- // ----
}

void BKEmulTimer::tick()
{
	if ( isWork() ) {
		tick_count++;
		bool flag;
		switch ( type ) {
			case t20 : flag = tick_count == 15; break;
			case t120: flag = tick_count == 60; break;
			case t22 : flag = tick_count == 140; break;
			case t122: flag = tick_count == 560; break;
			default  : flag = false; break;
		}
		WORD data = emul.memory.get_word( 0177710 );
		if ( flag ) {
			switch ( type ) {
				case t20 :
				case t120: data = 1; break;
				case t22 :
				case t122: data = 0; break;
			}
		} else {
			data--;
		}
		if ( !data ) {
			data = emul.memory.get_word( 0177706 );
//			cpu.setPR_100();
		}
		emul.memory.set_word( 0177710, data );
	}
}

void BKEmulTimer::set177706( WORD value )
{
}

void BKEmulTimer::set177712( WORD value )
{
	emul.memory.set_word( 0177710, emul.memory.get_word( 0177706 ) );
//	timer.setWork( data & 0000020 ? true : false );
}

void BKEmulTimer::set177712( BYTE value )
{
	// Начать/остановить отсчет
//	setWork( data & 020 ? true : false );
	emul.memory.set_byte( 0177712, value );
}

void BKEmulTimer::set177713( BYTE value )
{
	// Запись в 8-15 биты игнорируется
	emul.memory.set_byte( 0177713, 0xFF );
}

#include "stdafx.h"
#include "bkemul.h"
#include "resource.h"

//#include "bkemulapp.h"
//#include "bkemulmainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKEmul
// --------------------------------------------------------------
BKEmul emul;

BKEmul::BKEmul():
	powerOn( false ),
	BK_SYS_Timer_work( false )
{
}

BKEmul::~BKEmul()
{
}

void BKEmul::doSpeaker() const
{
}

void BKEmul::powerON()
{
	powerOn = true;
	video.updateMonitor();
	memory.clear();
	loadROM( "MONITOR_10" );
	loadROM( "BASIC_10_120" );
	loadROM( "BASIC_10_140" );
	loadROM( "BASIC_10_160" );
	reset();
}

void BKEmul::powerOFF()
{
	powerOn = false;
}

void BKEmul::reset()
{
	BK_SYS_Timer_work = false;

	R_177717_byte_read = 0100000 >> 8; // Задает начальный адрес и флаг отжатой клавиши
	                                   // ст. байт = 0100000 - адрес старта процессора
	R_177716_write     = 0000000;      // 0100 = 0

	cpu.reset();
	keyboard.reset();
/*
	memory.set_word( 0100000, 0012700 );
	memory.set_word( 0100002, 0040000 );
	memory.set_word( 0100004, 0012701 );
	memory.set_word( 0100006, 0020000 );
	memory.set_word( 0100010, 0012720 );
	memory.set_word( 0100012, 0177777 );
	memory.set_word( 0100014, 0077103 );
	memory.set_word( 0100016, 0000000 );
//	memory.set_word( 0100010, 0000167 );
//	memory.set_word( 0100012, 0177770 );
*/
}

void BKEmul::softReset()
{
	R_177717_byte_read = 0120000 >> 8;
	R_177716_write     = 0000000;

	cpu.reset();
	keyboard.reset();
}

void BKEmul::loadROM( const std::string& rom ) const
{
	loadIntoROM( ::FindResource( NULL, rom.c_str(), "ROM" ) );
}

void BKEmul::loadFont( const std::string& font ) const
{
	loadIntoROM( ::FindResource( NULL, font.c_str(), "BK_FONT" ) );
}

void BKEmul::loadIntoROM( const HRSRC& res ) const
{
	if ( res ) {
		HGLOBAL mem = ::LoadResource( NULL, res );
		if ( mem ) {
			WORD address;
			memcpy( &address, (char*)mem, 02 );
			WORD size;
			memcpy( &size, (char*)mem + 02, 02 );
			memcpy( (char*)memory.getMemory() + address, (char*)mem + 04, size );
		}
	}
}

void BKEmul::nextIteration()
{
	try {
		for ( int i = 0; i < 1000; i++ ) {
//			CClientDC dc( enulApp.mainFrame );
//			std::string str = format( "R7 = 0%0o", cpu.regs[ 6 ] );
//			dc.TextOut( 550, 30, str.c_str(), str.length() );
			cpu.nextIteration();
		}
	} catch( BKMemoryAccessError& /*e*/ ) {
//		e.report();
	}
}

BYTE BKEmul::getMemoryByte( WORD address )
{
	switch ( address ) {
		// В регистре состояния клавиатуры по чтению доступны 6 и 7 биты, остальные читаются нулями
		case 0177660: return memory.get_byte( address ) & 0300;
		// В регистре данных клавиатуры доступны биты 0-6, остальные (7-15) читаются нулями
		case 0177662: {
			BYTE res = memory.get_byte( address ) & 0177;
			// Из регистр данных клавиатуры прочитан код символа (регистр 177660, бит 0200 = 0 - прочитан, 1 - поступил)
			memory.set_word( 0177660, memory.get_word( 0177660 ) & 0177577 );
			return res;
		}
		// В регистре смещения доступны 0-7 и 9 биты, остальные не используются (читаются нулями ????)
		case 0177664: return memory.get_byte( address ) & 0377;
		case 0177665: return memory.get_byte( address ) & 0002;
		// В регистре управления таймером биты 8-15 читаются единицами
		case 0177713: return memory.get_byte( address ) & 0377;
		// Вернуть значение системного регистра по чтению
		case 0177716: return keyboard.R_177716_byte_read;
		case 0177717: return R_177717_byte_read;
		// Регистр параллельного программируемого интерфейса содержит нулевую нагрузку
		case 0177714: return 0;
		case 0177715: return 0;
	}
	return memory.get_byte( address );
}

WORD BKEmul::getMemoryWord( WORD address )
{
	address &= oddWordMask;
	WORD data = memory.get_word( address );
	switch ( address ) {
		// В регистре состояния клавиатуры доступны 6 и 7 биты, остальные читаются нулями
		case 0177660: return data & 0000300;
		// В регистре данных клавиатуры доступны биты 0-6, остальные (7-15) читаются нулями
		case 0177662: {
			data &= 0000177;
			// Из регистр данных клавиатуры прочитан код символа (регистр 177660, бит 0200 = 0 - прочитан, 1 - поступил)
			memory.set_word( 0177660, memory.get_word( 0177660 ) & 0177577 );
			return data;
		}
		// В регистре смещения доступны 0-7 и 9 биты, остальные не используются (читаются нулями ????)
		case 0177664: return data & 0001377;
		// В регистре управления таймером биты 8-15 читаются единицами
		case 0177712: return data | 0xFF00;
		// Вернуть значение системного регистра по чтению
		case 0177716: return R_177717_byte_read << 8 | keyboard.R_177716_byte_read;
		// Регистр параллельного программируемого интерфейса содержит нулевую нагрузку
		case 0177714: return 0;
	}
	return data;
}

void BKEmul::setMemoryByte( WORD address, BYTE data )
{
	// Попали в ПЗУ
	if ( ( address >= 0100000 && address < 0176560 ) ||
		 ( address >= 0176570 && address < 0177660 ) ||
		 ( address >  0177665 && address < 0177706 ) ||
		 ( address >  0177717) ) {
		throw BKMemoryAccessError( address, data );
	}

	switch ( address ) {
		// В регистре состояния клавиатуры по записи доступен только 6 бит
		case 0177660: {
			data = (memory.get_byte( address ) & 0200) | (data & 0100);
			memory.set_byte( address, data );
			break;
		}
		case 0177661: memory.set_byte( address, 0 ); break;
		// В системном регистре по записи доступны только 4-7 разряды выходного регистра 177716
		case 0177716: {
			R_177716_write &= 0xFF0F;
			R_177716_write |= data & 0xF0;
			doSpeaker();
			break;
		}
		case 0177717: R_177716_write &= 0x00F0; break;
		// Регистр смещения
		case 0177664:
		case 0177665: {
			// Доступны только 0-7 и 9 биты слова регистра смещения
			if ( address == 0177665 ) {
				data &= 0002;
			}
			WORD R_177664_prev_value = memory.get_word( 0177664 ) & 0001377;
			memory.set_byte( address, data );
			// Значение изменилось, необходима перерисовка экрана
			WORD value = memory.get_word( 0177664 ) & 0001377;
			if ( value != R_177664_prev_value ) {
				if ( (value & 01000) != (R_177664_prev_value & 01000) ) {
					video.setSmallScreen( R_177664_prev_value & 01000 ? true : false );
					// Перерисовать весь экран, т.к. сменился режим работы (полный <=> 1/4)
					video.updateMonitor();
				} else {
					// Перерисовать только рулонный сдвиг
					video.updateScrolling( static_cast<BYTE>(value - R_177664_prev_value) );
				}
			}
			break;
		}
		// Регистр управления таймером
		case 0177712:
		case 0177713: {
			// Запись в 8-15 биты слова игнорируется
			if ( address == 0177713 ) {
				data = 0xFF;
			}
			memory.set_word( 0177710, memory.get_word( 0177706 ) );
			// Начать/остановить отсчет
			BK_SYS_Timer_work = data & 020 ? true : false;
			memory.set_byte( address, data );
			break;
		}
		default: memory.set_byte( address, data );
	}

	// Попали в экранку
	if ( address >= 0040000 && address < 0100000 ) {
		video.updateVideoMemoryByte( address );
	}
}

void BKEmul::setMemoryWord( WORD address, WORD data )
{
	address &= oddWordMask;

	// Попали в ПЗУ
	if ( ( address >= 0100000 && address < 0176560 ) ||
	     ( address >= 0176570 && address < 0177660 ) ||
	     ( address >  0177665 && address < 0177706 ) ||
	     ( address >  0177717 ) ) {
		throw BKMemoryAccessError( address, data, false );
	}

	WORD oldData = memory.get_word( address );
	switch ( address ) {
		// В регистре состояния клавиатуры по записи доступен только 6 бит
		case 0177660: {
			data = ( oldData & 0000200 ) | ( data & 0000100 );
			memory.set_word( address, data );
			break;
		}
		// В системном регистре по записи доступны только 4-7 разряды выходного регистра 177716
		case 0177716: {
			R_177716_write &= 0xFF0F;
			R_177716_write |= data & 0x00F0;
			doSpeaker();
			break;
		}
		// Регистр смещения
		case 0177664: {
			// Доступны только 0-7 и 9 биты
			data &= 0001377;
			WORD R_177664_prev_value = oldData & 0001377;
			memory.set_word( address, data );
			// Значение изменилось, необходима перерисовка экрана
			if ( data != R_177664_prev_value ) {
				if ( (data & 01000) != (R_177664_prev_value & 01000) ) {
					video.setSmallScreen( R_177664_prev_value & 01000 ? true : false );
					// Перерисовать весь экран, т.к. сменился режим работы (полный <=> 1/4)
					video.updateMonitor();
				} else {
					// Перерисовать только рулонный сдвиг
					video.updateScrolling( static_cast<BYTE>(data - R_177664_prev_value) );
				}
			}
			break;
		}
		// В регистр счетчика таймера запись игнорируется
		case 0177710: break;
		// Регистр управления таймером
		case 0177712: {
			// Запись в 8-15 биты игнорируется
			data |= 0xFF00;
			memory.set_word( 0177710, memory.get_word( 0177706 ) );
			// Начать/остановить отсчет
			BK_SYS_Timer_work = data & 0000020 ? true : false;
			memory.set_word( address, data );
			break;
		}
		default: memory.set_word( address, data );
	}

	// Попали в экранку
	if ( address >= 0040000 && address < 0100000 ) {
		video.updateVideoMemoryWord( address );
	}
}

// --------------------------------------------------------------
// ---------- BKMemoryAccessError
// --------------------------------------------------------------
BKEmul::BKMemoryAccessError::BKMemoryAccessError( const WORD _address, const WORD _data, const bool _isByte ):
	address( _address ),
	data( _data ),
	isByte( _isByte )
{
};

void BKEmul::BKMemoryAccessError::report() const
{
	std::string s;
	if ( isByte ) {
		BYTE byte = data;
		s = format( IDS_MEMORYACCESSERROR_BYTE, byte, address );
	} else {
		s = format( IDS_MEMORYACCESSERROR_WORD, data, address );
	}
	AfxMessageBox( s.c_str(), MB_OK | MB_ICONERROR );
}

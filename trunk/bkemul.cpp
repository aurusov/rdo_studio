#include "stdafx.h"
#include "bkemul.h"

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
	colorMonitor( true ),
	BK_SYS_Timer_work( false )
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

BYTE BKEmul::getMemoryByte( WORD mem )
{
	switch ( mem ) {
		// В регистре состояния клавиатуры по чтению доступны 6 и 7 биты, остальные читаются нулями
		case 0177660: return memory[mem] & static_cast<BYTE>(0300);
		// В регистре данных клавиатуры доступны биты 0-6, остальные (7-15) читаются нулями
		case 0177662: {
			BYTE res = memory[mem] & static_cast<BYTE>(0177);
			// Из регистр данных клавиатуры прочитан код символа (регистр 177660, бит 0200 = 0 - прочитан, 1 - поступил)
			setDirectMemoryWord( 0177660, getDirectMemoryWord( 0177660 ) & 0177577 );
			return res;
		}
		// В регистре смещения доступны 0-7 и 9 биты, остальные не используются (читаются нулями ????)
		case 0177664: return memory[mem] & static_cast<BYTE>(0377);
		case 0177665: return memory[mem] & static_cast<BYTE>(0002);
		// В регистре управления таймером биты 8-15 читаются единицами
		case 0177713: return memory[mem] & static_cast<BYTE>(0377);
		// Вернуть значение системного регистра по чтению
		case 0177716: return static_cast<BYTE>(R_177716_read);
		case 0177717: return 0;
		// Регистр параллельного программируемого интерфейса содержит нулевую нагрузку
		case 0177714: return 0;
		case 0177715: return 0;
	}
	return memory[mem];
}

WORD BKEmul::getMemoryWord( WORD mem )
{
	mem &= oddWordMask;
	WORD data = memory[mem] | memory[mem+1];
	switch ( mem ) {
		// В регистре состояния клавиатуры доступны 6 и 7 биты, остальные читаются нулями
		case 0177660: return data & static_cast<WORD>(0000300);
		// В регистре данных клавиатуры доступны биты 0-6, остальные (7-15) читаются нулями
		case 0177662: {
			data &= static_cast<WORD>(0000177);
			// Из регистр данных клавиатуры прочитан код символа (регистр 177660, бит 0200 = 0 - прочитан, 1 - поступил)
			setDirectMemoryWord( 0177660, getDirectMemoryWord( 0177660 ) & 0177577 );
			return data;
		}
		// В регистре смещения доступны 0-7 и 9 биты, остальные не используются (читаются нулями ????)
		case 0177664: return data & static_cast<WORD>(0001377);
		// В регистре управления таймером биты 8-15 читаются единицами
		case 0177712: return data | static_cast<WORD>(0xFF00);
		// Вернуть значение системного регистра по чтению
		case 0177716: return R_177716_read;
		// Регистр параллельного программируемого интерфейса содержит нулевую нагрузку
		case 0177714: return 0;
	}
	return data;
}

void BKEmul::setMemoryByte( WORD mem, BYTE data )
{
	// Попали в ПЗУ
	if ( ( mem >= 0100000 && mem < 0176560 ) ||
		 ( mem >= 0176570 && mem < 0177660 ) ||
		 ( mem >  0177665 && mem < 0177706 ) ||
		 ( mem >  0177717) ) {
		throw BKMemoryAccessError( mem, data );
	}

	switch ( mem ) {
		// В регистре состояния клавиатуры по записи доступен только 6 бит
		case 0177660: {
			data = (memory[mem] & static_cast<BYTE>(0200)) | static_cast<BYTE>(data & 0100);
			memory[mem] = data;
			break;
		}
		case 0177661: memory[mem] = 0; break;
		// В системном регистре по записи доступны только 4-7 разряды выходного регистра 177716
		case 0177716: {
			R_177716_write &= 0xFF0F;
			R_177716_write |= static_cast<BYTE>(data & 0xF0);
			doSpeaker();
			break;
		}
		case 0177717: R_177716_write &= 0x00F0; break;
		// Регистр смещения
		case 0177664:
		case 0177665: {
			// Доступны только 0-7 и 9 биты слова регистра смещения
			if ( mem == 0177665 ) {
				data &= 0002;
			}
			WORD R_177664_prev_value = getDirectMemoryWord( 0177664 ) & 0001377;
			memory[mem] = data;
			// Значение изменилось, необходима перерисовка экрана
			WORD value = getDirectMemoryWord( 0177664 ) & 0001377;
			if ( value != R_177664_prev_value ) {
				if ( (value & 01000) != (R_177664_prev_value & 01000) ) {
					// Перерисовать весь экран, т.к. сменился режим работы (полный <=> 1/4)
//					Emulator->UpdateBKMonitor();
				} else {
					// Перерисовать только рулонный сдвиг
//					Emulator->UpdateBKMonitor_Scrolling((BYTE)(value - R_177664_prev_value));
				}
			}
			break;
		}
		// Регистр управления таймером
		case 0177712:
		case 0177713: {
			// Запись в 8-15 биты слова игнорируется
			if ( mem == 0177713 ) {
				data = 0xFF;
			}
			setDirectMemoryWord( 0177710, getDirectMemoryWord( 0177706 ) );
			// Начать/остановить отсчет
			BK_SYS_Timer_work = data & 020 ? true : false;
			memory[mem] = data;
			break;
		}
		default: memory[mem] = data;
	}

	// Попали в экранку
	if ( mem >= 0040000 && mem < 0100000 ) {
//		Emulator->UpdateBKVideoMemory_byte( mem );
	}
}

void BKEmul::doSpeaker() const
{
}

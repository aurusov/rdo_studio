#include "stdafx.h"
#include "bkemul.h"
#include "bkemulapp.h"
#include "bkemulmainfrm.h"
#include "bkemulchildview.h"

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
		(*it++) = 0xFF;
	}
}

BKEmul::~BKEmul()
{
}

BYTE BKEmul::getMemoryByte( WORD address )
{
	switch ( address ) {
		// В регистре состояния клавиатуры по чтению доступны 6 и 7 биты, остальные читаются нулями
		case 0177660: return get_byte( address ) & static_cast<BYTE>(0300);
		// В регистре данных клавиатуры доступны биты 0-6, остальные (7-15) читаются нулями
		case 0177662: {
			BYTE res = get_byte( address ) & static_cast<BYTE>(0177);
			// Из регистр данных клавиатуры прочитан код символа (регистр 177660, бит 0200 = 0 - прочитан, 1 - поступил)
			set_word( 0177660, get_word( 0177660 ) & 0177577 );
			return res;
		}
		// В регистре смещения доступны 0-7 и 9 биты, остальные не используются (читаются нулями ????)
		case 0177664: return get_byte( address ) & static_cast<BYTE>(0377);
		case 0177665: return get_byte( address ) & static_cast<BYTE>(0002);
		// В регистре управления таймером биты 8-15 читаются единицами
		case 0177713: return get_byte( address ) & static_cast<BYTE>(0377);
		// Вернуть значение системного регистра по чтению
		case 0177716: return static_cast<BYTE>(R_177716_read);
		case 0177717: return 0;
		// Регистр параллельного программируемого интерфейса содержит нулевую нагрузку
		case 0177714: return 0;
		case 0177715: return 0;
	}
	return get_byte( address );
}

WORD BKEmul::getMemoryWord( WORD address )
{
	address &= oddWordMask;
	WORD data = get_word( address );
	switch ( address ) {
		// В регистре состояния клавиатуры доступны 6 и 7 биты, остальные читаются нулями
		case 0177660: return data & static_cast<WORD>(0000300);
		// В регистре данных клавиатуры доступны биты 0-6, остальные (7-15) читаются нулями
		case 0177662: {
			data &= static_cast<WORD>(0000177);
			// Из регистр данных клавиатуры прочитан код символа (регистр 177660, бит 0200 = 0 - прочитан, 1 - поступил)
			set_word( 0177660, get_word( 0177660 ) & 0177577 );
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
			data = (get_byte( address ) & static_cast<BYTE>(0200)) | static_cast<BYTE>(data & 0100);
			set_byte( address, data );
			break;
		}
		case 0177661: set_byte( address, 0 ); break;
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
			if ( address == 0177665 ) {
				data &= 0002;
			}
			WORD R_177664_prev_value = get_word( 0177664 ) & 0001377;
			set_byte( address, data );
			// Значение изменилось, необходима перерисовка экрана
			WORD value = get_word( 0177664 ) & 0001377;
			if ( value != R_177664_prev_value ) {
				if ( (value & 01000) != (R_177664_prev_value & 01000) ) {
					// Перерисовать весь экран, т.к. сменился режим работы (полный <=> 1/4)
					enulApp.mainFrame->childView.updateMonitor();
				} else {
					// Перерисовать только рулонный сдвиг
					enulApp.mainFrame->childView.updateScrolling( static_cast<BYTE>(value - R_177664_prev_value) );
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
			set_word( 0177710, get_word( 0177706 ) );
			// Начать/остановить отсчет
			BK_SYS_Timer_work = data & 020 ? true : false;
			set_byte( address, data );
			break;
		}
		default: set_byte( address, data );
	}

	// Попали в экранку
	if ( address >= 0040000 && address < 0100000 ) {
		enulApp.mainFrame->childView.updateVideoMemoryByte( address );
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
		throw BKMemoryAccessError( address, data );
	}

	WORD oldData = get_word( address );
	switch ( address ) {
		// В регистре состояния клавиатуры по записи доступен только 6 бит
		case 0177660: {
			data = ( oldData & 0000200 ) | ( data & 0000100 );
			set_word( address, data );
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
			set_word( address, data );
			// Значение изменилось, необходима перерисовка экрана
			if ( data != R_177664_prev_value ) {
				if ( (data & 01000) != (R_177664_prev_value & 01000) ) {
					// Перерисовать весь экран, т.к. сменился режим работы (полный <=> 1/4)
					enulApp.mainFrame->childView.updateMonitor();
				} else {
					// Перерисовать только рулонный сдвиг
					enulApp.mainFrame->childView.updateScrolling( static_cast<BYTE>(data - R_177664_prev_value) );
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
			set_word( 0177710, get_word( 0177706 ) );
			// Начать/остановить отсчет
			BK_SYS_Timer_work = data & 0000020 ? true : false;
			set_word( address, data );
			break;
		}
		default: set_word( address, data );
	}

	// Попали в экранку
	if ( address >= 0040000 && address < 0100000 ) {
		enulApp.mainFrame->childView.updateVideoMemoryWord( address );
	}
}

void BKEmul::doSpeaker() const
{
}

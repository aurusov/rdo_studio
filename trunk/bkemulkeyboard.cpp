#include "stdafx.h"
#include "bkemulkeyboard.h"
#include "bkemul.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKEmulKeyboard
// --------------------------------------------------------------
BKEmulKeyboard::BKEmulKeyboard()
{
}

BKEmulKeyboard::~BKEmulKeyboard()
{
	key_list.clear();
}

void BKEmulKeyboard::reset()
{
	RUS = Shift = AR2 = SU = KeyPressed = StopPressed = false;
	ZAGL = true;
	// 0100 = 1 - клавиша отжата
	// 0200 = 1 - признак отсутствия арифметического расширения, должен быть = 1
	R_177716_byte_read = 0300;
	emul.memory.set_byte( 0177660, 0100 );
	key_list.clear();
}

// Проверить, есть ли клавиша в списке нажатых
bool BKEmulKeyboard::isKeyInList( char key ) const
{
	std::vector< char >::const_iterator it = key_list.begin();
	while ( it != key_list.end() ) {
		if ( key == *it++ ) {
			return true;
		}
	}
	return false;
}

// Добавить клавишу в список нажатых, если её там еще нет
void BKEmulKeyboard::addToKeyList( char key )
{
	if ( !isKeyInList( key ) ) {
		key_list.push_back( key );
	}
}

// Удалить клавишу из списка нажатых
void BKEmulKeyboard::removeFromKeyList( char key )
{
	std::vector< char >::iterator it = key_list.begin();
	while ( it != key_list.end() ) {
		if ( key == *it ) {
			key_list.erase( it );
			break;
		}
		it++;
	}
}

bool BKEmulKeyboard::keyDown( UINT key, UINT flags )
{
	bool BK_sysKey = false;
	switch ( key ) {
		// Esc = СТОП
		case VK_ESCAPE:
			if ( !StopPressed ) {
				emul.cpu.setPR_4();
			}
			StopPressed = true;
			BK_sysKey   = true;
			return true;
		// Shift = Нижний регистр (временная смена регистров)
		case VK_SHIFT:
			Shift     = true;
			BK_sysKey = true;
			return true;
		// Alt = АР2 (Alt-left) или СУ (Alt-right)
		case VK_MENU:
			if ( flags & 0x1000000 ) {
				SU = true;
			} else {
				AR2 = true;
			}
			BK_sysKey = true;
			return true;
		// CapsLock = ЗАГЛ/СТР
		case VK_CAPITAL:
			ZAGL      = !ZAGL;
			BK_sysKey = true;
			return true;
	}
	// Если не системная клавиша БК, а обыкновенная (и не клавиша IBM PC)
	BYTE BKScanCode;
	if ( !BK_sysKey && getBKScanCode( key, BKScanCode ) ) {
		addToKeyList( key );
		if ( !KeyPressed && key_list.size() == 1 ) {
			// Индикатор нажатой клавиши (регист 177716, разряд 6, бит 0100 = 0 - нажата, 1 - отжата)
			R_177716_byte_read &= 0277;
			KeyPressed = true;
			// Ctrl = РУС/ЛАТ (РУС = Ctrl-left / ЛАТ = Ctrl-right)
			if ( key == VK_CONTROL ) {
				bool bit_24 = flags & 1 << 24 ? true : false;
				RUS = !bit_24;
				BKScanCode = bit_24 ? 017 : 016;
			}
			// Поместили код нажатой клавиши
			emul.memory.set_byte( 0177662, BKScanCode );
			// Прочитан ли предыдущий код клавиши (регистр 177660, разряд 7, бит 0200 = 1 - в регистре данных клавиатуры есть код нажатой клавиши, 0 - нет)
			if ( !(emul.memory.get_byte( 0177660 ) & 0200 ) ) {
				// В регистр данных клавиатуры поступил код (регист 177660, разряд 7, бит 0200 = 1 - поступил, 0 - прочитан)
				emul.memory.set_byte( 0177660, emul.memory.get_byte( 0177660 ) | 0200 );
			}
			// Разрешено ли прерывание с клавиатуры (регист 177660, разряд 6, бит 0100 = 0 - разрешено, 1 - запрещено)
			if ( !(emul.getMemoryByte( 0177660 ) & 0100) && emul.cpu.getPrior() == 0 ) {
				if ( AR2 || BKScanCode <= 4 || BKScanCode == 013) {
					emul.cpu.setPR_274();
				} else {
					emul.cpu.setPR_60();
				}
			}
			return true;
		}
	}
	return false;
}

bool BKEmulKeyboard::keyUp( UINT key, UINT flags )
{
	removeFromKeyList( key );
	bool BK_sysKey = false;
	switch ( key ) {
		// Esc = СТОП
		case VK_ESCAPE:
			StopPressed = false;
			BK_sysKey   = true;
			return true;
		// Shift = Нижний регистр (временная смена регистров)
		case VK_SHIFT:
			Shift     = false;
			BK_sysKey = true;
			return true;
		// Alt = АР2 (Alt-left) или СУ (Alt-right)
		case VK_MENU:
			if ( flags & 0x1000000 ) {
				SU = false;
			} else {
				AR2 = false;
			}
			BK_sysKey = true;
			return true;
	}
	// Если не системная клавиша БК, а обыкновенная (и не клавиша IBM PC)
	BYTE BKScanCode;
	if ( !BK_sysKey && getBKScanCode( key, BKScanCode ) ) {
		if ( KeyPressed && key_list.empty() ) {
			// Индикатор нажатой клавиши (регист 177716, разряд 6, бит 0100 = 0 - нажата, 1 - отжата)
			R_177716_byte_read |= 0100;
			KeyPressed = false;
			return true;
		}
	}
	return false;
}

bool BKEmulKeyboard::getBKScanCode( UINT PCKey, BYTE& BKScanCode )
{
	typedef struct {
		int  pc;
		BYTE bk;
	} KeyTable;

	const int control_key_count   = 45;
	const int table_key_count     = 35;
	const int shift_key_eng_count = 21;
	const int shift_key_rus_count = 15;

	static KeyTable table_cnt[control_key_count] = {
		{VK_F6     , 0000}, // ШАГ
		{VK_F1     , 0001}, // ПОВТ
		{VK_F4     , 0002}, // ИНД СУ
		{VK_F2     , 0003}, // КТ
		{VK_F5     , 0004}, // БЛОК РЕД
		{VK_LEFT   , 0010}, // Влево
		{VK_NUMPAD4, 0010}, // Влево
		{VK_RETURN , 0012}, // Ввод
		{VK_F3     , 0013}, // =|==>
		{VK_F7     , 0014}, // СБР
		{0         , 0015}, // Установка табуляции
		{VK_CONTROL, 0016}, // РУС // РУС и ЛАТ обрабатывабтся в основном цикле обработки клавиш.
		{VK_CONTROL, 0017}, // ЛАТ // Но GetBKScanCode должна вернуть true, т.к. это обыкновенные клавиши БК (не СТОП, АР2 и др.)
		{0         , 0020}, // Сброс табуляции
		{VK_NUMPAD5, 0022}, // Исходная установка курсора
		{VK_HOME   , 0023}, // Перевод курсора в начало текущей строки
		{VK_TAB    , 0024}, // ТАБ
		{0         , 0025}, // Перемещение курсора в начало следующей строки
		{VK_DELETE , 0026}, // |<===
		{VK_INSERT , 0027}, // |===>
		{VK_BACK   , 0030}, // <=== Забой
		{VK_RIGHT  , 0031}, // Вправо
		{VK_NUMPAD6, 0031}, // Вправо
		{VK_UP     , 0032}, // Вверх
		{VK_NUMPAD8, 0032}, // Вверх
		{VK_DOWN   , 0033}, // Вниз
		{VK_NUMPAD2, 0033}, // Вниз
		{VK_NUMPAD7, 0034}, // Вверх-влево
		{VK_NUMPAD9, 0035}, // Вверх-вправо
		{VK_NUMPAD3, 0036}, // Вниз-вправо
		{VK_NUMPAD1, 0037}, // Вниз-влево
		{VK_SPACE  , 0040}, // Пробел
		{0x30      , 0060}, // 0
		{0x31      , 0061}, // 1
		{0x32      , 0062}, // 2
		{0x33      , 0063}, // 3
		{0x34      , 0064}, // 4
		{0x35      , 0065}, // 5
		{0x36      , 0066}, // 6
		{0x37      , 0067}, // 7
		{0x38      , 0070}, // 8
		{0x39      , 0071}, // 9
		{0xBD      , 0055}, // -
		{0xBB      , 0075}, // =
	};

	static KeyTable table_eng[table_key_count] = {
		{0x41      , 0141}, // a
		{0x42      , 0142}, // b
		{0x43      , 0143}, // c
		{0x44      , 0144}, // d
		{0x45      , 0145}, // e
		{0x46      , 0146}, // f
		{0x47      , 0147}, // g
		{0x48      , 0150}, // h
		{0x49      , 0151}, // i
		{0x4A      , 0152}, // j
		{0x4B      , 0153}, // k
		{0x4C      , 0154}, // l
		{0x4D      , 0155}, // m
		{0x4E      , 0156}, // n
		{0x4F      , 0157}, // o
		{0x50      , 0160}, // p
		{0x51      , 0161}, // q
		{0x52      , 0162}, // r
		{0x53      , 0163}, // s
		{0x54      , 0164}, // t
		{0x55      , 0165}, // u
		{0x56      , 0166}, // v
		{0x57      , 0167}, // w
		{0x58      , 0170}, // x
		{0x59      , 0171}, // y
		{0x5A      , 0172}, // z
		{0xC0      , 0140}, // `
		{0xDC      , 0134}, // '\'
		{0xDB      , 0133}, // [
		{0xDD      , 0135}, // ]
		{0xBA      , 0073}, // ;
		{0xDE      , 0047}, // '
		{0xBC      , 0054}, // ,
		{0xBE      , 0056}, // .
		{0xBF      , 0057}, // /
	};

	static KeyTable table_eng_zagl[table_key_count] = {
		{0x41      , 0101}, // A
		{0x42      , 0102}, // B
		{0x43      , 0103}, // C
		{0x44      , 0104}, // D
		{0x45      , 0105}, // E
		{0x46      , 0106}, // F
		{0x47      , 0107}, // G
		{0x48      , 0110}, // H
		{0x49      , 0111}, // I
		{0x4A      , 0112}, // J
		{0x4B      , 0113}, // K
		{0x4C      , 0114}, // L
		{0x4D      , 0115}, // M
		{0x4E      , 0116}, // N
		{0x4F      , 0117}, // O
		{0x50      , 0120}, // P
		{0x51      , 0121}, // Q
		{0x52      , 0122}, // R
		{0x53      , 0123}, // S
		{0x54      , 0124}, // T
		{0x55      , 0125}, // U
		{0x56      , 0126}, // V
		{0x57      , 0127}, // W
		{0x58      , 0130}, // X
		{0x59      , 0131}, // Y
		{0x5A      , 0132}, // Z
		{0xC0      , 0140}, // `
		{0xDC      , 0134}, // '\'
		{0xDB      , 0133}, // [
		{0xDD      , 0135}, // ]
		{0xBA      , 0073}, // ;
		{0xDE      , 0047}, // '
		{0xBC      , 0054}, // ,
		{0xBE      , 0056}, // .
		{0xBF      , 0057}, // /
	};

	static KeyTable table_rus[table_key_count] = {
		{0xBE      , 0100}, // ю
		{0x46      , 0101}, // а
		{0xBC      , 0102}, // б
		{0x57      , 0103}, // ц
		{0x4C      , 0104}, // д
		{0x54      , 0105}, // е
		{0x41      , 0106}, // ф
		{0x55      , 0107}, // г
		{0xDB      , 0110}, // х
		{0x42      , 0111}, // и
		{0x51      , 0112}, // й
		{0x52      , 0113}, // к
		{0x4B      , 0114}, // л
		{0x56      , 0115}, // м
		{0x59      , 0116}, // н
		{0x4A      , 0117}, // о
		{0x47      , 0120}, // п
		{0x5A      , 0121}, // я
		{0x48      , 0122}, // р
		{0x43      , 0123}, // с
		{0x4E      , 0124}, // т
		{0x45      , 0125}, // у
		{0xBA      , 0126}, // ж
		{0x44      , 0127}, // в
		{0x4D      , 0130}, // ь
		{0x53      , 0131}, // ы
		{0x50      , 0132}, // з
		{0x49      , 0133}, // ш
		{0xDE      , 0134}, // э
		{0x4F      , 0135}, // щ
		{0x58      , 0136}, // ч
		{0xDD      , 0137}, // ъ
		{0xDC      , 0057}, // /
		{0xC0      , 0047}, // '
		{0xBF      , 0056}, // .
	};

	static KeyTable table_rus_zagl[table_key_count] = {
		{0xBE      , 0140}, // Ю
		{0x46      , 0141}, // А
		{0xBC      , 0142}, // Б
		{0x57      , 0143}, // Ц
		{0x4C      , 0144}, // Д
		{0x54      , 0145}, // Е
		{0x41      , 0146}, // Ф
		{0x55      , 0147}, // Г
		{0xDB      , 0150}, // Х
		{0x42      , 0151}, // И
		{0x51      , 0152}, // Й
		{0x52      , 0153}, // К
		{0x4B      , 0154}, // Л
		{0x56      , 0155}, // М
		{0x59      , 0156}, // Н
		{0x4A      , 0157}, // О
		{0x47      , 0160}, // П
		{0x5A      , 0161}, // Я
		{0x48      , 0162}, // Р
		{0x43      , 0163}, // С
		{0x4E      , 0164}, // Т
		{0x45      , 0165}, // У
		{0xBA      , 0166}, // Ж
		{0x44      , 0167}, // В
		{0x4D      , 0170}, // Ь
		{0x53      , 0171}, // Ы
		{0x50      , 0172}, // З
		{0x49      , 0173}, // Ш
		{0xDE      , 0174}, // Э
		{0x4F      , 0175}, // Щ
		{0x58      , 0176}, // Ч
		{0xDD      , 0177}, // Ъ
		{0xDC      , 0057}, // /
		{0xC0      , 0047}, // '
		{0xBF      , 0056}, // .
	};

	static KeyTable table_shift_eng[shift_key_eng_count] = {
		{0xC0      , 0176}, // ~
		{0x31      , 0041}, // !
		{0x32      , 0100}, // @
		{0x33      , 0043}, // #
		{0x34      , 0044}, // $
		{0x35      , 0045}, // %
		{0x36      , 0136}, // ^
		{0x37      , 0046}, // &
		{0x38      , 0052}, // *
		{0x39      , 0050}, // (
		{0x30      , 0051}, // )
		{0xBD      , 0137}, // _
		{0xBB      , 0053}, // +
		{0xDC      , 0174}, // |
		{0xDB      , 0173}, // {
		{0xDD      , 0175}, // }
		{0xBA      , 0072}, // :
		{0xDE      , 0042}, // "
		{0xBC      , 0074}, // <
		{0xBE      , 0076}, // >
		{0xBF      , 0077}  // ?
	};

	static KeyTable table_shift_rus[shift_key_rus_count] = {
		{0x31      , 0041}, // !
		{0x32      , 0042}, // "
		{0xC0      , 0042}, // "
		{0x33      , 0043}, // #
		{0x34      , 0073}, // ;
		{0x35      , 0045}, // %
		{0x36      , 0072}, // :
		{0x37      , 0077}, // ?
		{0x38      , 0052}, // *
		{0x39      , 0050}, // (
		{0x30      , 0051}, // )
		{0xBB      , 0053}, // +
		{0xDC      , 0057}, // /
		{0xBF      , 0054}, // ,
		{0xBD      , 0055}, // -
	};

	KeyTable* table;
	int count;
	if ( Shift ) {
		if ( RUS ) {
			table = table_shift_rus;
			count = shift_key_rus_count;
		} else {
			table = table_shift_eng;
			count = shift_key_eng_count;
		}
	} else {
		table = table_cnt;
		count = control_key_count;
	}
	int i;
	for ( i = 0; i < count; i++ ) {
		if ( table[i].pc == PCKey ) {
			BKScanCode = table[i].bk;
			if ( SU ) {
				BKScanCode &= 037;
			}
			return true;
		}
	}
	if ( RUS ) {
		if ( ZAGL ) {
			table = Shift ? table_rus : table_rus_zagl;
		} else {
			table = Shift ? table_rus_zagl : table_rus;
		}
	} else {
		if ( ZAGL ) {
			table = Shift ? table_eng : table_eng_zagl;
		} else {
			table = Shift ? table_eng_zagl : table_eng;
		}
	}
	for ( i = 0; i < table_key_count; i++ ) {
		if ( table[i].pc == PCKey ) {
			BKScanCode = table[i].bk;
			if ( SU ) {
				BKScanCode &= 037;
			}
			return true;
		}
	}
	return false;
}

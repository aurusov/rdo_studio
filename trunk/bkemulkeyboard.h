#ifndef BKEMULKEYBOARD_H
#define BKEMULKEYBOARD_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulKeyboard
// --------------------------------------------------------------
class BKEmulKeyboard
{
friend class BKEmul;
private:
	bool RUS;
	bool ZAGL;
	bool Shift;
	bool AR2;
	bool SU;
	bool KeyPressed;
	bool StopPressed;

	BYTE R_177716_byte_read; // Регист 0177716 - состояние клавиатуры.

	bool getBKScanCode( UINT PCKey, BYTE& BKScanCode );

	std::vector< char > key_list;
	bool isKeyInList( char key ) const;
	void addToKeyList( char key );
	void removeFromKeyList( char key );

public:
	BKEmulKeyboard();
	virtual ~BKEmulKeyboard();

	void reset();

	bool keyDown( UINT key, UINT flags );
	bool keyUp( UINT key, UINT flags );
};

} // namespace bkemul

#endif // BKEMULKEYBOARD_H

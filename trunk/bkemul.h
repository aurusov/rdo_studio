#ifndef BKEMUL_H
#define BKEMUL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>

namespace bkemul {
// --------------------------------------------------------------
// ---------- BKEmul
// --------------------------------------------------------------
#define oddWordMask 0xFFFE

class BKEmul
{
friend class BKChildView;

private:
	std::vector< unsigned char > memory;
	bool colorMonitor;

	// Регист, имеющий разное значение по чтению/записи.
	WORD R_177716_read;   // Регист 0177716 - состояние клавиатуры.
	WORD R_177716_write;

	bool BK_SYS_Timer_work;     // Системный таймер БК запущен/остановлен

	class BKMemoryAccessError{
	public:
		BKMemoryAccessError( WORD memory, WORD data ) {};
	};

	void doSpeaker() const;

public:
	BKEmul();
	virtual ~BKEmul();

	const unsigned char* getVideoMemory() const { return &memory[0] + 040000; }
	int getVideoMemorySize() const              { return 040000;              }

	bool isColorMonitor() const { return colorMonitor; }

	BYTE getDirectMemoryByte( WORD mem ) {
		switch ( mem ) {
			case 0177716: return static_cast<BYTE>(R_177716_read);
			case 0177714: return 0;
		}
		return memory[mem];
	}
	WORD getDirectMemoryWord( WORD mem ) {
		mem &= oddWordMask;
		switch ( mem ) {
			case 0177716: return R_177716_read;
			case 0177714: return 0;
		}
		WORD data = memory[mem] | memory[mem+1];
		return data;
	}
	void setDirectMemoryByte( WORD mem, BYTE data ) {
		if ( mem == 0177716 ) {
			// По записи доступны только 4-7 разряды выходного регистра 177716
			R_177716_write &= 0xFF0F;
			R_177716_write |= static_cast<BYTE>(data & 0xF0);
		} else {
			memory[mem] = data;
		}
	}
	void setDirectMemoryWord( WORD mem, WORD data )  {
		mem &= oddWordMask;
		if ( mem == 0177716 ) {
			// По записи доступны только 4-7 разряды выходного регистра 177716
			R_177716_write &= 0xFF0F;
			R_177716_write |= data & 0x00F0;
		} else {
			memory[mem]   = LOBYTE( data );
			memory[mem+1] = HIBYTE( data );
		}
	}
	BYTE getMemoryByte( WORD mem );
	WORD getMemoryWord( WORD mem );
	void setMemoryByte( WORD mem, BYTE data );
};

} // namespace bkemul

// --------------------------------------------------------------
extern bkemul::BKEmul emul;

#endif // BKEMUL_H

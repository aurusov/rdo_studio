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
	std::vector< BYTE > memory;
	BYTE get_byte( WORD address ) const {
		return memory[address];
	}
	WORD get_word( WORD address ) const {
		address &= oddWordMask;
		WORD data = memory[address] | memory[address+1];
		return data;
	}
	void set_byte( WORD address, BYTE data ) {
		memory[address] = data;
	}
	void set_word( WORD address, WORD data )  {
		memory[address]   = LOBYTE( data );
		memory[address+1] = HIBYTE( data );
	}

	bool colorMonitor;

	// Регист, имеющий разное значение по чтению/записи.
	WORD R_177716_read;   // Регист 0177716 - состояние клавиатуры.
	WORD R_177716_write;

	bool BK_SYS_Timer_work;     // Системный таймер БК запущен/остановлен

	class BKMemoryAccessError{
	public:
		BKMemoryAccessError( WORD address, WORD data ) {};
	};

	void doSpeaker() const;

public:
	BKEmul();
	virtual ~BKEmul();

	const BYTE* getMemory( WORD address ) const { return &memory[address];    }
	const BYTE* getVideoMemory() const          { return &memory[0] + 040000; }
	int getVideoMemorySize() const              { return 040000;              }

	bool isColorMonitor() const { return colorMonitor; }

	BYTE getMemoryByte( WORD address );
	WORD getMemoryWord( WORD address );
	void setMemoryByte( WORD address, BYTE data );
	void setMemoryWord( WORD address, WORD data );
};

} // namespace bkemul

// --------------------------------------------------------------
extern bkemul::BKEmul emul;

#endif // BKEMUL_H

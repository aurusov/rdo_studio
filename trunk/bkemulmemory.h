#ifndef BKEMULMEMORY_H
#define BKEMULMEMORY_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulMemory
// --------------------------------------------------------------
#define oddWordMask (0xFFFE) // Маска для нечетных операций

class BKEmulMemory
{
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

	// Регист, имеющий разное значение по чтению/записи.
	WORD R_177716_read;   // Регист 0177716 - состояние клавиатуры.
	WORD R_177716_write;

public:
	BKEmulMemory();
	virtual ~BKEmulMemory();

	const BYTE* getMemory( WORD address = 0 ) const { return &memory[address]; }

	BYTE getMemoryByte( WORD address );
	WORD getMemoryWord( WORD address );
	void setMemoryByte( WORD address, BYTE data );
	void setMemoryWord( WORD address, WORD data );
};

// --------------------------------------------------------------
// ---------- BKMemoryAccessError
// --------------------------------------------------------------
class BKMemoryAccessError: public CException {
DECLARE_DYNAMIC( BKMemoryAccessError )
friend class BKEmulMemory;
private:
	WORD address;
	WORD data;
	bool isByte;

	BKMemoryAccessError( const WORD _address, const WORD _data, const bool _isByte = true );

	virtual int ReportError( UINT nType = MB_OK, UINT nMessageID = 0 );
};

} // namespace bkemul

#endif // BKEMULMEMORY_H

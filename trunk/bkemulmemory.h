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
friend class BKEmul;
friend class BKEmulKeyboard;
friend class BKEmulTimer;

private:
	std::vector< BYTE > memory;
	BYTE get_byte( WORD address ) const {
		return memory[address];
	}
	WORD get_word( WORD address ) const {
		address &= oddWordMask;
		WORD data = memory[address + 1] << 8 | memory[address];
		return data;
	}
	void set_byte( WORD address, BYTE data ) {
		memory[address] = data;
	}
	void set_word( WORD address, WORD data )  {
		memory[address]   = LOBYTE( data );
		memory[address+1] = HIBYTE( data );
	}

	void clear();

public:
	BKEmulMemory();
	virtual ~BKEmulMemory();

	const BYTE* getMemory( WORD address = 0 ) const { return &memory[address]; }
};

} // namespace bkemul

#endif // BKEMULMEMORY_H

#ifndef BKEMUL_H
#define BKEMUL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "bkemulcpu.h"
#include "bkemulmemory.h"
#include "bkemulvideo.h"

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmul
// --------------------------------------------------------------
class BKEmul
{
private:
	bool powerOn;
	bool BK_SYS_Timer_work;     // Системный таймер БК запущен/остановлен

	// Регист, имеющий разное значение по чтению/записи.
	WORD R_177716_read;   // Регист 0177716 - состояние клавиатуры.
	WORD R_177716_write;

	void doSpeaker() const;

public:
	BKEmul();
	virtual ~BKEmul();

	BKEmulCPU    cpu;
	BKEmulMemory memory;
	BKEmulVideo  video;

	void powerON();
	void powerOFF();
	void reset();
	bool isPowerON()       { return powerOn; }
	void nextIteration();

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
friend class BKEmul;
private:
	WORD address;
	WORD data;
	bool isByte;

	BKMemoryAccessError( const WORD _address, const WORD _data, const bool _isByte = true );

	virtual int ReportError( UINT nType = MB_OK, UINT nMessageID = 0 );
};


} // namespace bkemul

// --------------------------------------------------------------
extern bkemul::BKEmul emul;

#endif // BKEMUL_H

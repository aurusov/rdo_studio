#ifndef BKEMUL_H
#define BKEMUL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "bkemulcpu.h"
#include "bkemulmemory.h"
#include "bkemulvideo.h"
#include "bkemulkeyboard.h"
#include "bkemulspeaker.h"
#include "bkemultaperecorder.h"

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmul
// --------------------------------------------------------------
class BKEmul
{
private:
	bool powerOn;
	bool pause;
	int  speed;
	bool BK_SYS_Timer_work;     // Системный таймер БК запущен/остановлен

	// Регист, имеющий разное значение по чтению/записи.
	BYTE R_177717_byte_read;   // Регист 0177716 - состояние клавиатуры.
	WORD R_177716_write;

	void doSpeaker() const;

	void loadIntoROM( const HRSRC& res ) const;

public:
	BKEmul();
	virtual ~BKEmul();

	BKEmulCPU          cpu;
	BKEmulMemory       memory;
	BKEmulVideo        video;
	BKEmulKeyboard     keyboard;
	BKEmulSpeaker      speaker;
	BKEmulTapeRecorder taperecorder;

	void powerON();
	void powerOFF();
	bool getPause() const              { return pause;   }
	void setPause( const bool value );
	void reset();
	void softReset();
	bool isPowerON()                   { return powerOn; }
	void nextIteration();
	int  getSpeed() const              { return speed;   }
	void setSpeed( const int value );
	bool canSetSpeed( const int value );

	void loadROM( const std::string& rom ) const;
	void loadFont( const std::string& font ) const;

	BYTE getMemoryByte( WORD address );
	WORD getMemoryWord( WORD address );
	void setMemoryByte( WORD address, BYTE data );
	void setMemoryWord( WORD address, WORD data );

	class BKMemoryAccessError
	{
	friend class BKEmul;
	private:
		WORD address;
		WORD data;
		bool isByte;

		BKMemoryAccessError( const WORD _address, const WORD _data, const bool _isByte = true );

		void report() const;
	};
};

} // namespace bkemul

// --------------------------------------------------------------
extern bkemul::BKEmul emul;

#endif // BKEMUL_H

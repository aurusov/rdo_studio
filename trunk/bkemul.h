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
friend class BKEmulMemory;

private:
	bool BK_SYS_Timer_work;     // Системный таймер БК запущен/остановлен

	void doSpeaker() const;

public:
	BKEmul();
	virtual ~BKEmul();

	BKEmulCPU    cpu;
	BKEmulMemory memory;
	BKEmulVideo  video;

	void powerON();
};

} // namespace bkemul

// --------------------------------------------------------------
extern bkemul::BKEmul emul;

#endif // BKEMUL_H

#include "stdafx.h"
#include "bkemul.h"

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

BKEmul::BKEmul(): BK_SYS_Timer_work( false )
{
}

BKEmul::~BKEmul()
{
}

void BKEmul::powerON()
{
	cpu.reset();
}

void BKEmul::doSpeaker() const
{
}

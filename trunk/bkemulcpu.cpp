#include "stdafx.h"
#include "bkemulcpu.h"
#include "bkemul.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKEmulCPU
// --------------------------------------------------------------
#define Check_FN_word(A)        ((WORD)A & 0x8000)
#define Check_FZ_word(A)        ((WORD)A == 0)
#define Check_FN_byte(A)        ((BYTE)A & 0x80)
#define Check_FZ_byte(A)        ((BYTE)A == 0)
#define XOR(A, B)               ((A && !B) || (!A && B))

#define R0   regs[0] // Регистр R0
#define R1   regs[1] // Регистр R1
#define R2   regs[2] // Регистр R2
#define R3   regs[3] // Регистр R3
#define R4   regs[4] // Регистр R4
#define R5   regs[5] // Регистр R5
#define R6   regs[6] // Регистр R6 или SP- указатель стека
#define R7   regs[7] // Регистр R7 или PC - счетчик команд

#define INTERRUPT( A ) {       \
	R6 -= (WORD)2;             \
	Memory_word[R6] = MFPS();  \
	R6 -= (WORD)2;             \
	Memory_word[R6] = R7;      \
	R7 = Memory_word[0##A];    \
	MTPS(Memory_word[0##A+2]); \
}

BKEmulCPU::BKEmulCPU()
{
	regs.resize( 8 );

	doCommandGroup_0.push_back( &BKEmulCPU::BK_doGroup_1 );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doMOV );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doCMP );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doBIT );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doBIC );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doBIS );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doADD );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doGroup_2 );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doGroup_3 );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doMOVB );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doCMPB );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doBITB );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doBICB );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doBISB );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doSUB );
	doCommandGroup_0.push_back( &BKEmulCPU::BK_doUnknown );

	doCommandGroup_1_1.push_back( &BKEmulCPU::BK_doHALT );
	doCommandGroup_1_1.push_back( &BKEmulCPU::BK_doWAIT );
	doCommandGroup_1_1.push_back( &BKEmulCPU::BK_doRTI );
	doCommandGroup_1_1.push_back( &BKEmulCPU::BK_doBPT );
	doCommandGroup_1_1.push_back( &BKEmulCPU::BK_doIOT );
	doCommandGroup_1_1.push_back( &BKEmulCPU::BK_doRESET );
	doCommandGroup_1_1.push_back( &BKEmulCPU::BK_doRTT );

	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doNOP );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doCLC );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doCLV );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doCLZ );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doCLN );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doCCC );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doNOP );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doSEC );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doSEV );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doSEZ );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doSEN );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_2.push_back( &BKEmulCPU::BK_doSCC );

	doCommandGroup_1_3.push_back( &BKEmulCPU::BK_doBR );
	doCommandGroup_1_3.push_back( &BKEmulCPU::BK_doBNE );
	doCommandGroup_1_3.push_back( &BKEmulCPU::BK_doBEQ );
	doCommandGroup_1_3.push_back( &BKEmulCPU::BK_doBGE );
	doCommandGroup_1_3.push_back( &BKEmulCPU::BK_doBLT );
	doCommandGroup_1_3.push_back( &BKEmulCPU::BK_doBGT );
	doCommandGroup_1_3.push_back( &BKEmulCPU::BK_doBLE );

	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doCLR );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doCOM );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doINC );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doDEC );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doNEG );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doADC );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doSBC );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doTST );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doROR );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doROL );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doASR );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doASL );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doMARK );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_1_4.push_back( &BKEmulCPU::BK_doSXT );

	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doBPL );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doBMI );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doBHI );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doBLOS );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doBVC );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doBVS );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doBCC );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doBCS );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doEMT );
	doCommandGroup_3_1.push_back( &BKEmulCPU::BK_doTRAP );

	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doCLRB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doCOMB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doINCB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doDECB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doNEGB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doADCB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doSBCB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doTSTB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doRORB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doROLB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doASRB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doASLB );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doMTPS );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doUnknown );
	doCommandGroup_3_2.push_back( &BKEmulCPU::BK_doMFPS );
}

BKEmulCPU::~BKEmulCPU()
{
}

void BKEmulCPU::reset()
{
	R0 = R1 = R2 = R3 = R4 = R5 = R6 = R7 = 0;

	// Слово состояние процессора после включения питания должно быть 0340
	FC = FV = FZ = FN = FT = false;
	FP1 = FP2 = FP3 = true;

	R7 = emul.getMemoryWord( 0177716 ) & 0xFF00;
}

void BKEmulCPU::nextIteration()
{
	command = emul.getMemoryWord( R7 );
	R7 += static_cast<WORD>(2);
	WORD commandID = command >> 12;
	commandID &= 0x000F;
	(this->*doCommandGroup_0[commandID])();
}

WORD BKEmulCPU::MFPS()
{
	WORD res = 0;
	if (FC)  res |= 0000001;
	if (FV)  res |= 0000002;
	if (FZ)  res |= 0000004;
	if (FN)  res |= 0000010;
	if (FT)  res |= 0000020;
	if (FP1) res |= 0000040;
	if (FP2) res |= 0000100;
	if (FP3) res |= 0000200;
	return res;
}

void BKEmulCPU::MTPS( WORD data )
{
	FC  = data & 0000001 ? true : false;
	FV  = data & 0000002 ? true : false;
	FZ  = data & 0000004 ? true : false;
	FN  = data & 0000010 ? true : false;
	FT  = data & 0000020 ? true : false;
	FP1 = data & 0000040 ? true : false;
	FP2 = data & 0000100 ? true : false;
	FP3 = data & 0000200 ? true : false;
}

void BKEmulCPU::BK_doGroup_1()
{
	if ( command < 0000007 ) {
		// HALT .. RTT
		(this->*doCommandGroup_1_1[command])();
	} else if ( command < 0000400 ) {
		if ( command < 0000100 ) {
			BK_doUnknown();
		} else if ( command < 0000200 ) {
			BK_doJMP();
		} else if ( command < 0000210 ) {
			BK_doRTS();
		} else if ( command < 0000300 ) {
			if ( command < 0000240 ) {
				BK_doUnknown();
			} else {
				// NOP, CLC .. SCC
				(this->*doCommandGroup_1_2[command - 0000240])();
			}
		} else {
			BK_doSWAB();
		}
	} else if ( command < 0005000 ) {
		if ( command < 0004000 ) {
			// Переходы 0 000 nnn nxx xxx
			short_jmp = command << 1;
			short_jmp &= 0x01FE;
			if ( short_jmp & 0x0100 ) {
				short_jmp |= 0xFF00;
			}
			WORD jmp_id = command >> 8;
			jmp_id--;
			(this->*doCommandGroup_1_3[jmp_id])();
		} else {
			BK_doJSR();
		}
	} else if ( command < 0007000 ) {
		// CLR .. ALS, MARK, SXT
		WORD command_id = command - 0005000;
		command_id >>= 6;
		command_id &= 017;
		(this->*doCommandGroup_1_4[command_id])();
	} else {
		BK_doUnknown();
	}
}

void BKEmulCPU::BK_doMOV()
{
}

void BKEmulCPU::BK_doCMP()
{
}

void BKEmulCPU::BK_doBIT()
{
}

void BKEmulCPU::BK_doBIC()
{
}

void BKEmulCPU::BK_doBIS()
{
}

void BKEmulCPU::BK_doADD()
{
}

void BKEmulCPU::BK_doGroup_2()
{
	WORD command_id = command & 0007000;
	switch ( command_id ) {
		case 0004000: BK_doXOR(); break;
		case 0007000: BK_doSOB(); break;
		default     : BK_doUnknown();
	}
}

void BKEmulCPU::BK_doGroup_3()
{
	if (command < 0105000) {
		// Переходы 1 000 nnn nxx xxx, EMT, TRAP
		short_jmp = command << 1;
		short_jmp &= 0x01FE;
		if ( short_jmp & 0x0100 ) {
			short_jmp |= 0xFF00;
		}
		WORD jmp_id = command >> 8;
		jmp_id &= 0x000F;
		(this->*doCommandGroup_3_1[jmp_id])();
	} else if ( command < 0107000 ) {
		// CLRB .. ALSB, MTPS, MFPS
		WORD command_id = command - 0105000;
		command_id >>= 6;
		command_id &= 017;
		(this->*doCommandGroup_3_2[command_id])();
	} else {
		BK_doUnknown();
	}
}

void BKEmulCPU::BK_doMOVB()
{
}

void BKEmulCPU::BK_doCMPB()
{
}

void BKEmulCPU::BK_doBITB()
{
}

void BKEmulCPU::BK_doBICB()
{
}

void BKEmulCPU::BK_doBISB()
{
}

void BKEmulCPU::BK_doSUB()
{
}

void BKEmulCPU::BK_doUnknown()
{
}

void BKEmulCPU::BK_doJMP()
{
}

void BKEmulCPU::BK_doJSR()
{
}

void BKEmulCPU::BK_doRTS()
{
}

void BKEmulCPU::BK_doSWAB()
{
}

void BKEmulCPU::BK_doHALT()
{
}

void BKEmulCPU::BK_doWAIT()
{
}

void BKEmulCPU::BK_doRTI()
{
}

void BKEmulCPU::BK_doBPT()
{
}

void BKEmulCPU::BK_doIOT()
{
}

void BKEmulCPU::BK_doRESET()
{
}

void BKEmulCPU::BK_doRTT()
{
}

void BKEmulCPU::BK_doNOP()
{
}

void BKEmulCPU::BK_doCLC()
{
}

void BKEmulCPU::BK_doCLV()
{
}

void BKEmulCPU::BK_doCLZ()
{
}

void BKEmulCPU::BK_doCLN()
{
}

void BKEmulCPU::BK_doCCC()
{
}

void BKEmulCPU::BK_doSEC()
{
}

void BKEmulCPU::BK_doSEV()
{
}

void BKEmulCPU::BK_doSEZ()
{
}

void BKEmulCPU::BK_doSEN()
{
}

void BKEmulCPU::BK_doSCC()
{
}

void BKEmulCPU::BK_doBR()
{
}

void BKEmulCPU::BK_doBNE()
{
}

void BKEmulCPU::BK_doBEQ()
{
}

void BKEmulCPU::BK_doBGE()
{
}

void BKEmulCPU::BK_doBLT()
{
}

void BKEmulCPU::BK_doBGT()
{
}

void BKEmulCPU::BK_doBLE()
{
}

void BKEmulCPU::BK_doCLR()
{
}

void BKEmulCPU::BK_doCOM()
{
}

void BKEmulCPU::BK_doINC()
{
}

void BKEmulCPU::BK_doDEC()
{
}

void BKEmulCPU::BK_doNEG()
{
}

void BKEmulCPU::BK_doADC()
{
}

void BKEmulCPU::BK_doSBC()
{
}

void BKEmulCPU::BK_doTST()
{
}

void BKEmulCPU::BK_doROR()
{
}

void BKEmulCPU::BK_doROL()
{
}

void BKEmulCPU::BK_doASR()
{
}

void BKEmulCPU::BK_doASL()
{
}

void BKEmulCPU::BK_doMARK()
{
}

void BKEmulCPU::BK_doSXT()
{
}

void BKEmulCPU::BK_doXOR()
{
}

void BKEmulCPU::BK_doSOB()
{
}

void BKEmulCPU::BK_doBPL()
{
}

void BKEmulCPU::BK_doBMI()
{
}

void BKEmulCPU::BK_doBHI()
{
}

void BKEmulCPU::BK_doBLOS()
{
}

void BKEmulCPU::BK_doBVC()
{
}

void BKEmulCPU::BK_doBVS()
{
}

void BKEmulCPU::BK_doBCC()
{
}

void BKEmulCPU::BK_doBCS()
{
}

void BKEmulCPU::BK_doEMT()
{
}

void BKEmulCPU::BK_doTRAP()
{
}

void BKEmulCPU::BK_doCLRB()
{
}

void BKEmulCPU::BK_doCOMB()
{
}

void BKEmulCPU::BK_doINCB()
{
}

void BKEmulCPU::BK_doDECB()
{
}

void BKEmulCPU::BK_doNEGB()
{
}

void BKEmulCPU::BK_doADCB()
{
}

void BKEmulCPU::BK_doSBCB()
{
}

void BKEmulCPU::BK_doTSTB()
{
}

void BKEmulCPU::BK_doRORB()
{
}

void BKEmulCPU::BK_doROLB()
{
}

void BKEmulCPU::BK_doASRB()
{
}

void BKEmulCPU::BK_doASLB()
{
}

void BKEmulCPU::BK_doMTPS()
{
}

void BKEmulCPU::BK_doMFPS()
{
}

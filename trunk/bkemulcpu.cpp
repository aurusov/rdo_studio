#include "stdafx.h"
#include "bkemulcpu.h"

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

	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doGroup_1) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doMOV) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doCMP) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doBIT) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doBIC) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doBIS) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doADD) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doGroup_2) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doGroup_3) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doMOVB) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doCMPB) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doBITB) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doBICB) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doBISB) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doSUB) );
	doCommandGroup_0.push_back( static_cast<CPUcommand>(BK_doUnknown) );
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
}

void BKEmulCPU::BK_doGroup_3()
{
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

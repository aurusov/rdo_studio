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
#define BK_3bits_Mask ((WORD)0x0007)  // Маска для выделения трех младших бит,

#define Check_FN_word(A)        (static_cast<WORD>(A) & 0x8000 ? true : false)
#define Check_FZ_word(A)        (static_cast<WORD>(A) == 0     ? true : false)
#define Check_FN_byte(A)        (static_cast<BYTE>(A) & 0x80   ? true : false)
#define Check_FZ_byte(A)        (static_cast<BYTE>(A) == 0     ? true : false)
#define XOR(A, B)               ((A && !B) || (!A && B))

#define R0   regs[0] // Регистр R0
#define R1   regs[1] // Регистр R1
#define R2   regs[2] // Регистр R2
#define R3   regs[3] // Регистр R3
#define R4   regs[4] // Регистр R4
#define R5   regs[5] // Регистр R5
#define R6   regs[6] // Регистр R6 или SP- указатель стека
#define R7   regs[7] // Регистр R7 или PC - счетчик команд

BKEmulCPU::BKEmulCPU()
{
	regs.resize( 8 );

	R0 = R1 = R2 = R3 = R4 = R5 = R6 = R7 = 0;
	FC = FV = FZ = FN = FT = FP1 = FP2 = FP3 = PR_4 = PR_60 = PR_274 = false;

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

	R7 = emul.getMemoryWord( 0177716 ) & 0xFF00;

	BK_doRESET();
}

void BKEmulCPU::nextIteration()
{
	// Прерывание по нажатию на клавишу СТОП или по зависанию канала
	if ( PR_4 ) {
		BK_doHALT();
		return;
	}
	// Прерывание с клавиатуры
	if ( PR_60 ) {
		PR_60 = false;
		interrupt(060);
		return;
	}
	if ( PR_274 ) {
		PR_274 = false;
		interrupt(0274);
		return;
	}

	command = emul.getMemoryWord( R7 );
	R7 += static_cast<WORD>(2);
	WORD commandID = command >> 12;
	commandID &= 0x000F;
	(this->*doCommandGroup_0[commandID])();
}

// Определение приемника-байта перед выполнением команды
void BKEmulCPU::Command_dest_Parser_byte()
{
	// Индекс регистра приемника
	dest_reg_id = command;
	dest_reg_id &= BK_3bits_Mask;
	// Режим адресации приемника
	WORD dest_mode_id = command >> 3;
	dest_mode_id &= BK_3bits_Mask;
	is_dest_reg = false;
	switch ( dest_mode_id ) {
		case 0: is_dest_reg = true;
				break;
		case 1: dest_adr = regs[dest_reg_id];
				break;
		case 2: dest_adr = regs[dest_reg_id];
				regs[dest_reg_id]++;
				if ( dest_reg_id == 7 ) R7++;
				break;
		case 3: dest_adr = regs[dest_reg_id];
				regs[dest_reg_id] += 2;
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				break;
		case 4: regs[dest_reg_id]--;
				if (dest_reg_id == 7) R7--;
				dest_adr = regs[dest_reg_id];
				break;
		case 5: regs[dest_reg_id] -= 2;
				dest_adr = regs[dest_reg_id];
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				break;
		case 6: dest_adr = R7;
				R7 += 2;
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				dest_adr += regs[dest_reg_id];
				break;
		case 7: dest_adr = R7;
				R7 += 2;
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				dest_adr += regs[dest_reg_id];
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				break;
	}
}

// Определение приемника-слова перед выполнением команды
void BKEmulCPU::Command_dest_Parser_word()
{
	// Индекс регистра приемника
	dest_reg_id = command;
	dest_reg_id &= BK_3bits_Mask;
	// Режим адресации приемника
	WORD dest_mode_id = command >> 3;
	dest_mode_id &= BK_3bits_Mask;
	is_dest_reg = false;
	switch ( dest_mode_id ) {
		case 0: is_dest_reg = true;
				break;
		case 1: dest_adr = regs[dest_reg_id];
				break;
		case 2: dest_adr = regs[dest_reg_id];
				regs[dest_reg_id] += 2;
				break;
		case 3: dest_adr = regs[dest_reg_id];
				regs[dest_reg_id] += 2;
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				break;
		case 4: regs[dest_reg_id] -= 2;
				dest_adr = regs[dest_reg_id];
				break;
		case 5: regs[dest_reg_id] -= 2;
				dest_adr = regs[dest_reg_id];
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				break;
		case 6: dest_adr = R7;
				R7 += 2;
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				dest_adr += regs[dest_reg_id];
				break;
		case 7: dest_adr = R7;
				R7 += 2;
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				dest_adr += regs[dest_reg_id];
				dest_adr &= oddWordMask;
				dest_adr = emul.getMemoryWord( dest_adr );
				break;
	}
	// ????
	// Общая часть для режимов адресации  1..7
//	if ( dest_mode_id ) {
//		dest_adr &= oddWordMask;
//	}
}

// Определение источника-байта и приемника-байта перед выполнением команды
void BKEmulCPU::CommandParser_byte()
{
	// Источник
	// Индекс регистра источника - необходим для доступа к регистру через regs[]
	WORD source_reg_id = command >> 6;
	source_reg_id &= BK_3bits_Mask;
	// Режим адресации источника
	WORD source_mode_id = command >> 9;
	source_mode_id &= BK_3bits_Mask;
	switch ( source_mode_id ) {
		case 0: source_data = static_cast<BYTE>(regs[source_reg_id]);
				break;
		case 1: source_data = regs[source_reg_id];
				break;
		case 2: source_data = regs[source_reg_id];
				regs[source_reg_id]++;
				if (source_reg_id == 7) R7++;
				break;
		case 3: source_data = regs[source_reg_id];
				regs[source_reg_id] += 2;
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				break;
		case 4: regs[source_reg_id]--;
				if (source_reg_id == 7) R7--;
				source_data = regs[source_reg_id];
				break;
		case 5: regs[source_reg_id] -= 2;
				source_data = regs[source_reg_id];
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				break;
		case 6: source_data = R7;
				R7 += 2;
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				source_data += regs[source_reg_id];
				break;
		case 7: source_data = R7;
				R7 += 2;
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				source_data += regs[source_reg_id];
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				break;
	}
	// Общая часть для режимов адресации  1..7
	if ( source_mode_id ) {
		source_data = emul.getMemoryByte( source_data );
	}

	source_data &= 0x00FF;
	//Приемник
	Command_dest_Parser_byte();
}

// Определение источника-слова и приемника-слова перед выполнением команды
void BKEmulCPU::CommandParser_word()
{
	// Источник
	// Индекс регистра источника - необходим для доступа к регистру через regs[]
	WORD source_reg_id = command >> 6;
	source_reg_id &= BK_3bits_Mask;
	// Режим адресации источника
	WORD source_mode_id = command >> 9;
	source_mode_id &= BK_3bits_Mask;
	switch ( source_mode_id ) {
		case 0: source_data = regs[source_reg_id];
				break;
		case 1: source_data = regs[source_reg_id];
				break;
		case 2: source_data = regs[source_reg_id];
				regs[source_reg_id] += 2;
				break;
		case 3: source_data = regs[source_reg_id];
				regs[source_reg_id] += 2;
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				break;
		case 4: regs[source_reg_id] -= 2;
				source_data = regs[source_reg_id];
				break;
		case 5: regs[source_reg_id] -= 2;
				source_data = regs[source_reg_id];
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				break;
		case 6: source_data = R7;
				R7 += 2;
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				source_data += regs[source_reg_id];
				break;
		case 7: source_data = R7;
				R7 += 2;
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				source_data += regs[source_reg_id];
				source_data &= oddWordMask;
				source_data = emul.getMemoryWord( source_data );
				break;
	}
	// Общая часть для режимов адресации  1..7
	if ( source_mode_id ) {
		source_data &= oddWordMask;
		source_data = emul.getMemoryWord( source_data );
	}
	// Приемник
	Command_dest_Parser_word();
}

BYTE BKEmulCPU::GetDestData_byte()
{
	return is_dest_reg ? regs[dest_reg_id] : emul.getMemoryByte( dest_adr );
}

void BKEmulCPU::SetDestData_byte( BYTE data )
{
	if ( is_dest_reg ) {
		WORD word = regs[dest_reg_id];
		word &= 0xFF00;
		word |= data;
		regs[dest_reg_id] = word;
	} else {
		emul.setMemoryByte( dest_adr, data );
	}
}

WORD BKEmulCPU::GetDestData_word()
{
	return is_dest_reg ? regs[dest_reg_id] : emul.getMemoryWord( dest_adr & oddWordMask );
}

void BKEmulCPU::SetDestData_word( WORD data )
{
	if ( is_dest_reg ) {
		regs[dest_reg_id] = data;
	} else {
		// ????
//		emul.setMemoryWord( dest_adr, data );
		emul.setMemoryWord( dest_adr & oddWordMask, data );
	}
}

void BKEmulCPU::interrupt( WORD address )
{
	R6 -= 2;
	emul.setMemoryWord( R6, MFPS() );
	R6 -= 2;
	emul.setMemoryWord( R6, R7 );
	R7 = emul.getMemoryWord( address );
	MTPS( emul.getMemoryWord( address + 2 ) );
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
	CommandParser_word();
	SetDestData_word( source_data );
	FN = Check_FN_word( source_data );
	FZ = Check_FZ_word( source_data );
	FV = false;
}

void BKEmulCPU::BK_doCMP()
{
	CommandParser_word();
	WORD data = GetDestData_word();
	WORD word = source_data - data;
	FN = Check_FN_word( word );
	FZ = Check_FZ_word( word );
	FV = ((source_data & 0x8000) != (data & 0x8000)) && ((word & 0x8000) == (data & 0x8000));
	// ????
	DWORD dword = source_data - data;
	FC = (dword > 0x0000FFFF);
//	FC = (dword & 0x10000) ? true : false;
}

void BKEmulCPU::BK_doBIT()
{
	CommandParser_word();
	WORD data = GetDestData_word() & source_data;
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = false;
}

void BKEmulCPU::BK_doBIC()
{
	CommandParser_word();
	WORD data = GetDestData_word() & ~source_data;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = false;
}

void BKEmulCPU::BK_doBIS()
{
	CommandParser_word();
	WORD data = GetDestData_word() | source_data;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = false;
}

void BKEmulCPU::BK_doADD()
{
	CommandParser_word();
	WORD data = GetDestData_word();
	WORD word = data + source_data;
	DWORD dword = data + source_data;
	SetDestData_word( word );
	FN = Check_FN_word( word );
	FZ = Check_FZ_word( word );
	FV = ((source_data & 0x8000) == (data & 0x8000)) && ((word & 0x8000) != (source_data & 0x8000));
	FC = (dword > 0x0000FFFF);
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
	CommandParser_byte();
	if ( is_dest_reg ) {
		if ( source_data & 0x80 ) {
			source_data |= 0xFF00;
		} else {
			source_data &= 0x00FF;
		}
		regs[dest_reg_id] = source_data;
	} else {
		emul.setMemoryByte( dest_adr, source_data );
	}
	FN = Check_FN_byte( source_data );
	FZ = Check_FZ_byte( source_data );
	FV = false;
}

void BKEmulCPU::BK_doCMPB()
{
	CommandParser_byte();
	BYTE data = GetDestData_byte();
	BYTE byte = source_data - data;
	FN = Check_FN_byte( byte );
	FZ = Check_FZ_byte( byte );
	FV = ((source_data & 0x80) != (data & 0x80)) && ((byte & 0x80) == (data & 0x80));
	// ????
	WORD word = source_data - data;
	FC = (word > 0x00FF);
//	FC = (word & 0x0100) ? true : false;
}

void BKEmulCPU::BK_doBITB()
{
	CommandParser_byte();
	BYTE data = GetDestData_byte() & source_data;
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = false;
}

void BKEmulCPU::BK_doBICB()
{
	CommandParser_byte();
	BYTE data = GetDestData_byte() & ~source_data;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = false;
}

void BKEmulCPU::BK_doBISB()
{
	CommandParser_byte();
	BYTE data = GetDestData_byte() | source_data;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = false;
}

void BKEmulCPU::BK_doSUB()
{
	CommandParser_word();
	WORD data = GetDestData_word();
	WORD word = data - source_data;
	SetDestData_word( word );
	FN = Check_FN_word( word );
	FZ = Check_FZ_word( word );
	FV = ((source_data & 0x8000) != (data & 0x8000)) && ((word & 0x8000) == (data & 0x8000));
	DWORD dword = data - source_data;
	FC = (dword > 0x0000FFFF);
}

void BKEmulCPU::BK_doUnknown()
{
	interrupt( 010 );
}

void BKEmulCPU::BK_doJMP()
{
	Command_dest_Parser_word();
	if ( !is_dest_reg ) {
		R7 = dest_adr;
	} else {
		BK_doHALT();
	}
}

void BKEmulCPU::BK_doJSR()
{
	Command_dest_Parser_word();
	if ( !is_dest_reg ) {
		WORD reg_id = command >> 6;
		reg_id &= BK_3bits_Mask;
		WORD value = regs[reg_id];
		R6 -= 2;
		emul.setMemoryWord( R6, value );
		regs[reg_id] = R7;
		R7 = dest_adr;
	} else {
		BK_doHALT();
	}
}

void BKEmulCPU::BK_doRTS()
{
	WORD reg_id = command;
	reg_id &= BK_3bits_Mask;
	R7 = regs[reg_id];
	regs[reg_id] = emul.getMemoryWord( R6 );
	R6 += 2;
}

void BKEmulCPU::BK_doSWAB()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	// ???? FN, FZ проверять по источнику или по результату
	FN = Check_FN_word( data );
	BYTE b = data >> 8;
	data = data << 8;
	data &= 0xFF00;
	data |= b;
	SetDestData_word( data );
	FZ = Check_FZ_byte( data >> 8 );
	FV = false;
	FC = false;
}

void BKEmulCPU::BK_doHALT()
{
//	if ( SearchFileInfo.FindHandle ) {
//		FindClose( SearchFileInfo );
//		SearchFileInfo.FindHandle = 0;
//	}
	PR_4 = false;
	interrupt( 04 );
}

void BKEmulCPU::BK_doWAIT()
{
}

void BKEmulCPU::BK_doRTI()
{
	R7 = emul.getMemoryWord( R6 );
	R6 += 2;
	MTPS( emul.getMemoryWord( R6 ) );
	R6 += 2;
}

void BKEmulCPU::BK_doBPT()
{
	interrupt( 014 );
}

void BKEmulCPU::BK_doIOT()
{
	interrupt( 020 );
}

void BKEmulCPU::BK_doRESET()
{
	// Слово состояние процессора после включения питания должно быть 0340
	FC = FV = FZ = FN = FT = false;
	FP1 = FP2 = FP3 = true;

	PR_4 = PR_60 = PR_274 = false;

	emul.keyboard.reset();
}

void BKEmulCPU::BK_doRTT()
{
	R7 = emul.getMemoryWord( R6 );
	R6 += 2;
	MTPS( emul.getMemoryWord( R6 ) );
	R6 += 2;
}

void BKEmulCPU::BK_doNOP()
{
}

void BKEmulCPU::BK_doCLC()
{
	FC = false;
}

void BKEmulCPU::BK_doCLV()
{
	FV = false;
}

void BKEmulCPU::BK_doCLZ()
{
	FZ = false;
}

void BKEmulCPU::BK_doCLN()
{
	FN = false;
}

void BKEmulCPU::BK_doCCC()
{
	FN = FZ = FV = FC = false;
}

void BKEmulCPU::BK_doSEC()
{
	FC = true;
}

void BKEmulCPU::BK_doSEV()
{
	FV = true;
}

void BKEmulCPU::BK_doSEZ()
{
	FZ = true;
}

void BKEmulCPU::BK_doSEN()
{
	FN = true;
}

void BKEmulCPU::BK_doSCC()
{
	FN = FZ = FV = FC = true;
}

void BKEmulCPU::BK_doBR()
{
	R7 += short_jmp;
}

void BKEmulCPU::BK_doBNE()
{
	if ( FZ == false ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBEQ()
{
	if ( FZ == true ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBGE()
{
	if ( XOR(FN, FV) == false ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBLT()
{
	if ( XOR(FN, FV) == true ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBGT()
{
	if ( (FZ || XOR(FN, FV)) == false ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBLE()
{
	if ( (FZ || XOR(FN, FV)) == true ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doCLR()
{
	Command_dest_Parser_word();
	SetDestData_word( 0x0000 );
	FN = false;
	FZ = true;
	FV = false;
	FC = false;
}

void BKEmulCPU::BK_doCOM()
{
	Command_dest_Parser_word();
	WORD data = ~GetDestData_word();
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = false;
	FC = true;
}

void BKEmulCPU::BK_doINC()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	// ???? FV проверять по источнику или по результату
	FV = data == 0077777;
	data++;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
}

void BKEmulCPU::BK_doDEC()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	// ???? FV проверять по источнику или по результату
	FV = data == 0100000;
	data--;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
}

void BKEmulCPU::BK_doNEG()
{
	Command_dest_Parser_word();
	WORD data = ~GetDestData_word();
	data++;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	// ???? FV проверять по источнику или по результату
	FV = data == 0100000;
	FC = data != 0;
}

void BKEmulCPU::BK_doADC()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	WORD bit_c = FC ? (WORD)0x0001 : (WORD)0x0000;
	FV = (data == 0077777) && FC;
	FC = (data == 0177777) && FC;
	data += bit_c;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
}

void BKEmulCPU::BK_doSBC()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	WORD bit_c = FC ? (WORD)0x0001 : (WORD)0x0000;
	FV = (data == 0100000);
	// ???? FC = !((data == 0000000) && FC);
	FC = (data == 0000000) && FC;
	data -= bit_c;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
}

void BKEmulCPU::BK_doTST()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = false;
	FC = false;
}

void BKEmulCPU::BK_doROR()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	WORD bit_c = FC ? (WORD)0x8000 : (WORD)0x0000;
	FC = data & 0x0001;
	data >>= 1;
	data &= 0x7FFF;
	data |= bit_c;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = XOR( FN, FC );
}

void BKEmulCPU::BK_doROL()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	WORD bit_c = FC ? (WORD)0x0001 : (WORD)0x0000;
	FC = data & 0x8000 ? true : false;
	data <<= 1;
	data &= 0xFFFE;
	data |= bit_c;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = XOR( FN, FC );
}

void BKEmulCPU::BK_doASR()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	WORD bit_15 = data & (WORD)0x8000;
	FC = data & 0x0001;
	data >>= 1;
	data &= 0x7FFF;
	data |= bit_15;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = XOR( FN, FC );
}

void BKEmulCPU::BK_doASL()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	FC = data & 0x8000 ? true : false;
	data <<= 1;
	data &= 0xFFFE;
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = XOR( FN, FC );
}

void BKEmulCPU::BK_doMARK()
{
	WORD n = command & 0000077;
	n <<= 1;
	R6 = (R7 & BK_3bits_Mask) + n;
	R7 = R5;
	R5 = emul.getMemoryWord( R6 );
	R6 += 2;
}

void BKEmulCPU::BK_doSXT()
{
	Command_dest_Parser_word();
	// ????
//	WORD data = FN ? (WORD)0x0001 : (WORD)0x0000;
	WORD data = FN ? (WORD)0xFFFF : (WORD)0x0000;
	SetDestData_word( data );
	FZ = Check_FZ_word( data );
	FV = false;
}

void BKEmulCPU::BK_doXOR()
{
	Command_dest_Parser_word();
	WORD data = GetDestData_word();
	WORD reg_id = command >> 6;
	reg_id &= BK_3bits_Mask;
	WORD data_2 = regs[reg_id];
	WORD mask = 0x0001;
	for ( int i = 0; i < 16; i++ ) {
		if ( XOR((data & mask), (data_2 & mask)) ) {
			data |= mask;
		} else {
			data &= ~mask;
		}
		mask <<= 1;
	}
	SetDestData_word( data );
	FN = Check_FN_word( data );
	FZ = Check_FZ_word( data );
	FV = false;
}

void BKEmulCPU::BK_doSOB()
{
	WORD reg_id = command >> 6;
	reg_id &= BK_3bits_Mask;
	regs[reg_id]--;
	if ( regs[reg_id] != 0 ) {
		WORD sob_jmp = command & 0000077;
		sob_jmp <<= 1;
		R7 -= sob_jmp;
	}
}

void BKEmulCPU::BK_doBPL()
{
	if ( FN == false ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBMI()
{
	if ( FN == true ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBHI()
{
	if ( (FC || FZ) == false ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBLOS()
{
	if ( (FC || FZ) == true ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBVC()
{
	if ( FV == false ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBVS()
{
	if ( FV == true ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBCC()
{
	if ( FC == false ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doBCS()
{
	if ( FC == true ) {
		R7 += short_jmp;
	}
}

void BKEmulCPU::BK_doEMT()
{
	interrupt( 030 );
	if ( (command & 077) == 036 ) {
		emul.taperecorder.doEMT36();
	}
}

void BKEmulCPU::BK_doTRAP()
{
	interrupt( 034 );
}

void BKEmulCPU::BK_doCLRB()
{
	Command_dest_Parser_byte();
	SetDestData_byte( 0x00 );
	FN = false;
	FZ = true;
	FV = false;
	FC = false;
}

void BKEmulCPU::BK_doCOMB()
{
	Command_dest_Parser_byte();
	BYTE data = ~GetDestData_byte();
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = false;
	FC = true;
}

void BKEmulCPU::BK_doINCB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	// ???? FV проверять по источнику или по результату
	FV = data == 0177;
	data++;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
}

void BKEmulCPU::BK_doDECB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	// ???? FV проверять по источнику или по результату
	FV = data == 0200;
	data--;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
}

void BKEmulCPU::BK_doNEGB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	data = (BYTE)~data;
	data++;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	// ???? FV проверять по источнику или по результату
	FV = data == 0200;
	FC = data != 0;
}

void BKEmulCPU::BK_doADCB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	BYTE bit_c = FC ? (BYTE)0x01 : (BYTE)0x00;
	FV = (data == 0177) && FC;
	FC = (data == 0377) && FC;
	data += bit_c;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
}

void BKEmulCPU::BK_doSBCB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	BYTE bit_c = FC ? (BYTE)0x01 : (BYTE)0x00;
	FV = (data == 0200);
	// ???? FC = !((data == 0000) && FC);
	FC = (data == 0000) && FC;
	data -= bit_c;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
}

void BKEmulCPU::BK_doTSTB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = false;
	FC = false;
}

void BKEmulCPU::BK_doRORB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	BYTE bit_c = FC ? (BYTE)0x80 : (BYTE)0x00;
	FC = data & 0x01;
	data >>= 1;
	data &= 0x7F;
	data |= bit_c;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = XOR( FN, FC );
}

void BKEmulCPU::BK_doROLB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	BYTE bit_c = FC ? (BYTE)0x01 : (BYTE)0x00;
	FC = data & 0x80 ? true : false;
	data <<= 1;
	data &= 0xFE;
	data |= bit_c;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = XOR( FN, FC );
}

void BKEmulCPU::BK_doASRB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	BYTE bit_7 = data & (BYTE)0x80;
	FC = data & 0x01;
	data >>= 1;
	data &= 0x7F;
	data |= bit_7;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = XOR(FN, FC);
}

void BKEmulCPU::BK_doASLB()
{
	Command_dest_Parser_byte();
	BYTE data = GetDestData_byte();
	FC = data & 0x80 ? true : false;
	data <<= 1;
	data &= 0xFE;
	SetDestData_byte( data );
	FN = Check_FN_byte( data );
	FZ = Check_FZ_byte( data );
	FV = XOR(FN, FC);
}

void BKEmulCPU::BK_doMTPS()
{
	Command_dest_Parser_word();
	MTPS( GetDestData_byte() );
}

void BKEmulCPU::BK_doMFPS()
{
	Command_dest_Parser_word();
	WORD ps = MFPS();
	SetDestData_byte( ps );
	FN = Check_FN_word( ps );
	FZ = Check_FZ_word( ps );
	FV = false;
}

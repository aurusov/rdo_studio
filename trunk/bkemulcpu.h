#ifndef BKEMULCPU_H
#define BKEMULCPU_H

#if _MSC_VER > 1000
#pragma once
#endif

namespace bkemul {

// --------------------------------------------------------------
// ---------- BKEmulCPU
// --------------------------------------------------------------
class BKEmulCPU;
typedef void (BKEmulCPU::* CPUcommand)();

class BKEmulCPU
{
friend class BKEmulTapeRecorder;
private:
	// Регистры R0..R7
	std::vector< WORD > regs;

	// Слово состояния процессора, PS = 0177776
	bool FC;  // Флаг знака
	bool FV;  // Флаг нуля
	bool FZ;  // Флаг переполнения
	bool FN;  // Флаг переноса
	bool FT;  // Флаг T - разряда
	bool FP1; // Флаг приоритета
	bool FP2; // Флаг приоритета
	bool FP3; // Флаг приоритета (запрещает прерывание с внешних устройств, например, клавиатуры)

	// Прерывания
	bool PR_4;
	bool PR_60;
	bool PR_100;
	bool PR_274;

	WORD command;             // Каманда процессора
	WORD source_data;         // Данные источника
	WORD dest_adr;            // Адресс приемника
	WORD dest_reg_id;         // Индекс регистра приемника. Необходим в том случае, когда приемником является регистр
	bool is_dest_reg;         // Приемником является регистр
	WORD short_jmp;           // Приращение к R7 в команде близкого перехода

	// Разбор метода адресации
	void Command_dest_Parser_byte();
	void Command_dest_Parser_word();
	void CommandParser_byte();
	void CommandParser_word();
	// Установка приемника
	BYTE GetDestData_byte();
	void SetDestData_byte( BYTE data );
	WORD GetDestData_word();
	void SetDestData_word( WORD data );

	void interrupt( WORD address );

	WORD MFPS();
	void MTPS( WORD data );

	// BK_doGroup_0
	std::vector< CPUcommand > doCommandGroup_0;
	void BK_doGroup_1();   // 00xxxx
	void BK_doMOV();       // 01ssdd
	void BK_doCMP();       // 02ssdd
	void BK_doBIT();       // 03ssdd
	void BK_doBIC();       // 04ssdd
	void BK_doBIS();       // 05ssdd
	void BK_doADD();       // 06ssdd
	void BK_doGroup_2();   // 07xxxx
	void BK_doGroup_3();   // 10xxxx
	void BK_doMOVB();      // 11ssdd
	void BK_doCMPB();      // 12ssdd
	void BK_doBITB();      // 13ssdd
	void BK_doBICB();      // 14ssdd
	void BK_doBISB();      // 15ssdd
	void BK_doSUB();       // 16ssdd
	void BK_doUnknown();   // 17xxxx

	// BK_doGroup_1
	void BK_doJMP();       // 0001dd
	void BK_doJSR();       // 004rdd
	void BK_doRTS();       // 00020r
	void BK_doSWAB();      // 0003dd

	std::vector< CPUcommand > doCommandGroup_1_1;
	void BK_doHALT();      // 000000
	void BK_doWAIT();      // 000001
	void BK_doRTI();       // 000002
	void BK_doBPT();       // 000003
	void BK_doIOT();       // 000004
	void BK_doRESET();     // 000005
	void BK_doRTT();       // 000006

	std::vector< CPUcommand > doCommandGroup_1_2;
	void BK_doNOP();       // 000240 000260
	void BK_doCLC();       // 000241
	void BK_doCLV();       // 000242
	void BK_doCLZ();       // 000244
	void BK_doCLN();       // 000250
	void BK_doCCC();       // 000257
	void BK_doSEC();       // 000261
	void BK_doSEV();       // 000262
	void BK_doSEZ();       // 000264
	void BK_doSEN();       // 000270
	void BK_doSCC();       // 000277

	std::vector< CPUcommand > doCommandGroup_1_3;
	void BK_doBR();        // 0004xx (0 000 000 1xx xxx xxx)
	void BK_doBNE();       // 0010xx (0 000 001 0xx xxx xxx)
	void BK_doBEQ();       // 0014xx (0 000 001 1xx xxx xxx)
	void BK_doBGE();       // 0020xx (0 000 010 0xx xxx xxx)
	void BK_doBLT();       // 0024xx (0 000 010 1xx xxx xxx)
	void BK_doBGT();       // 0030xx (0 000 011 0xx xxx xxx)
	void BK_doBLE();       // 0034xx (0 000 011 1xx xxx xxx)

	std::vector< CPUcommand > doCommandGroup_1_4;
	void BK_doCLR();       // 0050dd
	void BK_doCOM();       // 0051dd
	void BK_doINC();       // 0052dd
	void BK_doDEC();       // 0053dd
	void BK_doNEG();       // 0054dd
	void BK_doADC();       // 0055dd
	void BK_doSBC();       // 0056dd
	void BK_doTST();       // 0057dd
	void BK_doROR();       // 0060dd
	void BK_doROL();       // 0061dd
	void BK_doASR();       // 0062dd
	void BK_doASL();       // 0063dd
	void BK_doMARK();      // 0064nn
	void BK_doSXT();       // 0067dd

	// BK_doGroup_2
	void BK_doXOR();       // 074rdd
	void BK_doSOB();       // 077rxx

	// BK_doGroup_3
	std::vector< CPUcommand > doCommandGroup_3_1;
	void BK_doBPL();       // 1000xx (1 000 000 0xx xxx xxx)
	void BK_doBMI();       // 1004xx (1 000 000 1xx xxx xxx)
	void BK_doBHI();       // 1010xx (1 000 001 0xx xxx xxx)
	void BK_doBLOS();      // 1014xx (1 000 001 1xx xxx xxx)
	void BK_doBVC();       // 1020xx (1 000 010 0xx xxx xxx)
	void BK_doBVS();       // 1024xx (1 000 010 1xx xxx xxx)
	void BK_doBCC();       // 1030xx (1 000 011 0xx xxx xxx) == BHIS
	void BK_doBCS();       // 1034xx (1 000 011 1xx xxx xxx) == BLO
	void BK_doEMT();       // 1040nn (1 000 100 0nn nnn nnn)
	void BK_doTRAP();      // 1044nn (1 000 100 1nn nnn nnn)

	std::vector< CPUcommand > doCommandGroup_3_2;
	void BK_doCLRB();      // 1050dd
	void BK_doCOMB();      // 1051dd
	void BK_doINCB();      // 1052dd
	void BK_doDECB();      // 1053dd
	void BK_doNEGB();      // 1054dd
	void BK_doADCB();      // 1055dd
	void BK_doSBCB();      // 1056dd
	void BK_doTSTB();      // 1057dd
	void BK_doRORB();      // 1060dd
	void BK_doROLB();      // 1061dd
	void BK_doASRB();      // 1062dd
	void BK_doASLB();      // 1063dd
	void BK_doMTPS();      // 1064ss
	void BK_doMFPS();      // 1067dd

public:
	BKEmulCPU();
	virtual ~BKEmulCPU();

	void reset();
	void nextIteration();

	WORD R0() const { return regs[0]; }
	WORD R1() const { return regs[1]; }
	WORD R2() const { return regs[2]; }
	WORD R3() const { return regs[3]; }
	WORD R4() const { return regs[4]; }
	WORD R5() const { return regs[5]; }
	WORD SP() const { return regs[6]; }
	WORD PC() const { return regs[7]; }

	BYTE getPrior() const { return FP3 ? 1:0 << 3 | FP2 ? 1:0 << 2 | FP1 ? 1:0; }

	void setPR_4()   { PR_4   = true; }
	void setPR_60()  { PR_60  = true; }
	void setPR_100() { PR_100 = true; }
	void setPR_274() { PR_274 = true; }
};

} // namespace bkemul

#endif // BKEMULCPU_H

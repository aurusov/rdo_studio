/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdoprocess_datablock.h
 * author   : Шувалов Андрей
 * date     : 04.05.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef RDO_PROCESS_DATABLOCK_H
#define RDO_PROCESS_DATABLOCK_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlock
// ----------------------------------------------------------------------------
class RPShapeDataBlock
{
public:

	enum zakonRaspr
	{
		Const,
		Normal,
		Uniform,
		Exp
	};

	RPShapeDataBlock(zakonRaspr zakon, tstring name);

	void setBase  (int base   );
	void setAmount(int amount );
	void setDisp  (double disp);
	void setExp   (double exp );

	zakonRaspr getZakon() {return m_zakon;}
	tstring    getName () {return m_name; }
	int        getBase () {return m_base; }
	double     getDisp () {return m_disp; }
	double     getExp  () {return m_exp;  }

protected:

	zakonRaspr m_zakon;
	tstring    m_name;
	int        m_base;
	double     m_exp;
	double     m_disp;
};

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockCreate
// ----------------------------------------------------------------------------
class RPShapeDataBlockCreate: public RPShapeDataBlock
{
public:

	RPShapeDataBlockCreate(RPShapeDataBlock::zakonRaspr zakon, tstring name);
	~RPShapeDataBlockCreate();

	void setAmount(int amount);

	int getAmount() const {return m_amount;}

private:

	int m_amount;

};

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockTerminate
// ----------------------------------------------------------------------------
class RPShapeDataBlockTerminate
{
public:
	RPShapeDataBlockTerminate(tstring name);
	~RPShapeDataBlockTerminate();

	void setTermInc(int term_inc);

	int     getTermInc(            ) {return m_term_inc;}
	tstring getName   (            ) {return m_name;    }

private:
	int     m_term_inc;
	tstring m_name;
};

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockProcess
// ----------------------------------------------------------------------------
class RPShapeDataBlockProcess: public RPShapeDataBlock
{
public:

	enum resAction
	{
		Advance,
		Seize,
		Release
	};

	RPShapeDataBlockProcess(RPShapeDataBlock::zakonRaspr zakon, tstring name);
	~RPShapeDataBlockProcess();

	void addAction(resAction action);
	void addRes   (tstring res     );
	
	std::list <resAction> getAction();
	std::list <tstring  > getRes   ();

private:
	std::list <resAction>  m_actions;
	std::list <tstring  >  m_res;
};

#endif // RDO_PROCESS_DATABLOCK_H

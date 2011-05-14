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

	RPShapeDataBlock(zakonRaspr zakon, CREF(tstring) name);

	void setBase  (int base   );
	void setAmount(int amount );
	void setDisp  (double disp);
	void setExp   (double exp );

	zakonRaspr    getZakon() const { return m_zakon; }
	CREF(tstring) getName () const { return m_name;  }
	int           getBase () const { return m_base;  }
	double        getDisp () const { return m_disp;  }
	double        getExp  () const { return m_exp;   }

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
	 RPShapeDataBlockCreate(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name);
	~RPShapeDataBlockCreate();

	int  getAmount() const      { return m_amount; }
	void setAmount(int amount);

private:
	int m_amount;
};

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockTerminate
// ----------------------------------------------------------------------------
class RPShapeDataBlockTerminate
{
public:
	 RPShapeDataBlockTerminate(CREF(tstring) name);
	~RPShapeDataBlockTerminate();

	void    setTermInc(int term_inc);
	int     getTermInc() const        { return m_term_inc; }
	tstring getName   () const        { return m_name;     }

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

	typedef std::list<resAction> ActionList;
	typedef std::list<tstring  > ResList;

	RPShapeDataBlockProcess(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name);
	~RPShapeDataBlockProcess();

	void addAction(resAction action);
	void addRes   (CREF(tstring) res);
	
	CREF(ActionList) getAction() const;
	CREF(ResList)    getRes   () const;

private:
	ActionList  m_actions;
	ResList     m_res;
};

#endif //! RDO_PROCESS_DATABLOCK_H

/*!
  \copyright (c) RDO-Team, 2011
  \file      procgui_datablock.h
  \author    Шувалов Андрей
  \date      04.05.2011
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_GUI_DATABLOCK_H_
#define _SIMULATOR_COMPILER_GUI_DATABLOCK_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/compiler/procgui/namespace.h"
#include "simulator/runtime/process/generate.h"
// --------------------------------------------------------------------------------

OPEN_COMPILER_GUI_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlock
// --------------------------------------------------------------------------------
OBJECT(RPShapeDataBlock)
{
DECLARE_FACTORY(RPShapeDataBlock);
public:
	enum zakonRaspr
	{
		Const,
		Normal,
		Uniform,
		Exp,
		Triangular
	};

	void setBase  (int base   );
	void setAmount(int amount );
	void setDisp  (double disp);
	void setExp   (double exp );
	void setMax   (double max );

	zakonRaspr    getZakon() const { return m_zakon; }
	CREF(tstring) getName () const { return m_name;  }
	int           getBase () const { return m_base;  }
	double        getDisp () const { return m_disp;  }
	double        getExp  () const { return m_exp;   }
	double        getMax  () const { return m_max;   }

protected:
	RPShapeDataBlock(zakonRaspr zakon, CREF(tstring) name);
	virtual ~RPShapeDataBlock() {}

	zakonRaspr m_zakon;
	tstring    m_name;
	int        m_base;
	double     m_exp;
	double     m_disp;
	double     m_max;
};

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockCreate
// --------------------------------------------------------------------------------
class RPShapeDataBlockCreate: public RPShapeDataBlock
{
DECLARE_FACTORY(RPShapeDataBlockCreate)
public:
	int  getAmount() const      { return m_amount; }
	void setAmount(int amount);

	CREF(rdo::runtime::LPIInternalStatistics) getStatistics() const      { return m_pStatistics; }
	void setStatistics(CREF(rdo::runtime::LPIInternalStatistics) pStatistics);

private:
	RPShapeDataBlockCreate(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name);
	virtual ~RPShapeDataBlockCreate();

	int                                  m_amount;
	rdo::runtime::LPIInternalStatistics  m_pStatistics;
};
DECLARE_POINTER(RPShapeDataBlockCreate);

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockTerminate
// --------------------------------------------------------------------------------
OBJECT(RPShapeDataBlockTerminate)
{
DECLARE_FACTORY(RPShapeDataBlockTerminate)
public:
	void    setTermInc(int term_inc);
	int     getTermInc() const        { return m_term_inc; }
	tstring getName   () const        { return m_name;     }

	CREF(rdo::runtime::LPIInternalStatistics) getStatistics() const      { return m_pStatistics; }
	void setStatistics(CREF(rdo::runtime::LPIInternalStatistics) pStatistics);

private:
	RPShapeDataBlockTerminate(CREF(tstring) name);
	virtual ~RPShapeDataBlockTerminate();

	int                                 m_term_inc;
	tstring                             m_name;
	rdo::runtime::LPIInternalStatistics m_pStatistics;
};

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockProcess
// --------------------------------------------------------------------------------
class RPShapeDataBlockProcess: public RPShapeDataBlock
{
DECLARE_FACTORY(RPShapeDataBlockProcess)
public:
	enum Action
	{
		A_ADVANCE,
		A_SEIZE,
		A_RELEASE
	};

	typedef  std::list<Action>   ActionList;
	typedef  std::list<tstring>  ResNameList;

	void addAction(Action action);
	void addRes   (CREF(tstring) res);
	
	CREF(ActionList)  getActionList () const;
	CREF(ResNameList) getResNameList() const;

private:
	RPShapeDataBlockProcess(RPShapeDataBlock::zakonRaspr zakon, CREF(tstring) name);
	virtual ~RPShapeDataBlockProcess();

	ActionList  m_actionList;
	ResNameList m_resNameList;
};
DECLARE_POINTER(RPShapeDataBlockProcess);

CLOSE_COMPILER_GUI_NAMESPACE

#endif // _SIMULATOR_COMPILER_GUI_DATABLOCK_H_

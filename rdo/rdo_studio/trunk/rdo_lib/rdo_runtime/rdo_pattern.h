/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_pattern.h
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 13.04.2008
 * bref     : Описание базового класса для образцов всех типов активностей и событий
 * indent   : 4T
 */

#ifndef _RDO_PATTERN_H_
#define _RDO_PATTERN_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDOEvent;
class RDORule;
class RDOKeyboard;

// ----------------------------------------------------------------------------
// ---------- RDOPattern
// ----------------------------------------------------------------------------
OBJECT(RDOPattern)
	IS  INSTANCE_OF(RDORuntimeObject  )
	AND INSTANCE_OF(RDOTraceableObject)
{
DECLARE_FACTORY(RDOPattern);

public:
	void addPreSelectRelRes(CREF(LPRDOCalc) pCalc)
	{
		CalcList::iterator it = m_preSelectRelRes.begin();
		while ( it != m_preSelectRelRes.end() )
		{
			if ((*it)->compare(pCalc))
			{
				return;
			}
			it++;
		}
		m_preSelectRelRes.push_back(pCalc);
	}

protected:
	RDOPattern(rbool trace);
	virtual ~RDOPattern() {}

	typedef  std::vector<LPRDOCalc>                   CalcList;
	typedef  std::vector<RDOResource::ConvertStatus>  ConvertStatusList;

	CalcList m_preSelectRelRes;

	void preSelectRelRes(CREF(LPRDORuntime) pRuntime)
	{
		runCalcs(m_preSelectRelRes, pRuntime);
	}

	void runCalcs(REF(CalcList) calcList, CREF(LPRDORuntime) pRuntime)
	{
		LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
		pRuntime->getMemoryStack()->push(pLocalMemory);
		STL_FOR_ALL(calcList, calcIt)
			(*calcIt)->calcValue(pRuntime);
		pRuntime->getMemoryStack()->pop();
	}
	bool runCalcsBool(REF(CalcList) calcList, CREF(LPRDORuntime) pRuntime)
	{
		STL_FOR_ALL(calcList, calcIt)
		{
			if ( !(*calcIt)->calcValue( pRuntime ).getAsBool() ) return false;
		}
		return true;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternEvent
// ----------------------------------------------------------------------------
CLASS(RDOPatternEvent): INSTANCE_OF(RDOPattern)
{
DECLARE_FACTORY(RDOPatternEvent);
friend class RDOEvent;

public:
	void addConvertorCalc  ( CREF(LPRDOCalc) pCalc             ) { m_convertor.push_back( pCalc );        }
	void addConvertorStatus( RDOResource::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( CREF(LPRDOCalc) pCalc             ) { m_erase.push_back( pCalc );            }

	void convertEvent( CREF(LPRDORuntime) pRuntime )
	{
		preSelectRelRes( pRuntime );
		runCalcs( m_convertor, pRuntime );
	}
	void convertErase( CREF(LPRDORuntime) pRuntime )
	{
		runCalcs( m_erase, pRuntime );
	}

	double getNextTimeInterval( CREF(LPRDORuntime) pRuntime );

	LPIEvent createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) oprName);

private:
	RDOPatternEvent(rbool trace);
	virtual ~RDOPatternEvent();

	LPRDOCalc         m_timeCalc;
	CalcList          m_convertor;
	ConvertStatusList m_convertorStatus;
	CalcList          m_erase;
};
DECLARE_POINTER(RDOPatternEvent);

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
CLASS(RDOPatternRule): INSTANCE_OF(RDOPattern)
{
DECLARE_FACTORY(RDOPatternRule);
friend class RDORule;

public:
	void addChoiceFromCalc ( CREF(LPRDOCalc) pCalc             ) { m_choiceFrom.push_back( pCalc );       }
	void addConvertorCalc  ( CREF(LPRDOCalc) pCalc             ) { m_convertor.push_back( pCalc );        }
	void addConvertorStatus( RDOResource::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( CREF(LPRDOCalc) pCalc             ) { m_erase.push_back( pCalc );            }

	bool choiceFrom( CREF(LPRDORuntime) pRuntime )
	{
		preSelectRelRes( pRuntime );
		return runCalcsBool( m_choiceFrom, pRuntime );
	}
	void convertRule(CREF(LPRDORuntime) pRuntime)
	{
		runCalcs(m_convertor, pRuntime);
	}
	void convertErase(CREF(LPRDORuntime) pRuntime)
	{
		runCalcs(m_erase, pRuntime);
	}

	LPIRule createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName);
	LPIRule createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) condition, CREF(tstring) _oprName);

private:
	RDOPatternRule(rbool trace);
	virtual ~RDOPatternRule();

	CalcList          m_choiceFrom;
	CalcList          m_convertor;
	ConvertStatusList m_convertorStatus;
	CalcList          m_erase;
};
DECLARE_POINTER(RDOPatternRule);

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
CLASS(RDOPatternOperation): INSTANCE_OF(RDOPattern)
{
DECLARE_FACTORY(RDOPatternOperation);
friend class RDOOperation;

public:
	void addChoiceFromCalc      ( CREF(LPRDOCalc) pCalc             ) { m_choiceFrom.push_back( pCalc );            }

	void addConvertorBeginCalc  ( CREF(LPRDOCalc) pCalc             ) { m_convertorBegin.push_back( pCalc );        }
	void addConvertorBeginStatus( RDOResource::ConvertStatus status ) { m_convertorBeginStatus.push_back( status ); }
	void addEraseBeginCalc      ( CREF(LPRDOCalc) pCalc             ) { m_eraseBegin.push_back( pCalc );            }

	void addConvertorEndCalc    ( CREF(LPRDOCalc) pCalc             ) { m_convertorEnd.push_back( pCalc );          }
	void addConvertorEndStatus  ( RDOResource::ConvertStatus status ) { m_convertorEndStatus.push_back( status );   }
	void addEraseEndCalc        ( CREF(LPRDOCalc) pCalc             ) { m_eraseEnd.push_back( pCalc );              }

	void setTime                ( CREF(LPRDOCalc) pCalc             ) { m_timeCalc = pCalc;                         }

	bool choiceFrom( CREF(LPRDORuntime) pRuntime )
	{
		preSelectRelRes( pRuntime );
		return runCalcsBool( m_choiceFrom, pRuntime );
	}
	void convertBegin( CREF(LPRDORuntime) pRuntime )
	{
		runCalcs( m_convertorBegin, pRuntime );
	}
	void convertBeginErase( CREF(LPRDORuntime) pRuntime )
	{
		runCalcs( m_eraseBegin, pRuntime );
	}
	void convertEnd( CREF(LPRDORuntime) pRuntime )
	{
		runCalcs( m_convertorEnd, pRuntime );
	}
	void convertEndErase( CREF(LPRDORuntime) pRuntime )
	{
		runCalcs( m_eraseEnd, pRuntime );
	}

	double getNextTimeInterval( CREF(LPRDORuntime) pRuntime );

	LPIOperation createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName);
	LPIOperation createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) condition, CREF(tstring) _oprName);

protected:
	RDOPatternOperation(rbool trace);
	virtual ~RDOPatternOperation();

private:
	LPRDOCalc         m_timeCalc;
	CalcList          m_choiceFrom;

	CalcList          m_convertorBegin;
	ConvertStatusList m_convertorBeginStatus;
	CalcList          m_eraseBegin;

	CalcList          m_convertorEnd;
	ConvertStatusList m_convertorEndStatus;
	CalcList          m_eraseEnd;
};
DECLARE_POINTER(RDOPatternOperation);

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
CLASS(RDOPatternKeyboard): INSTANCE_OF(RDOPatternOperation)
{
DECLARE_FACTORY(RDOPatternKeyboard);
public:
	LPIKeyboard createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName);
	LPIKeyboard createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) condition, CREF(tstring) _oprName);

private:
	RDOPatternKeyboard(rbool trace);
	virtual ~RDOPatternKeyboard();
};
DECLARE_POINTER(RDOPatternKeyboard);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_PATTERN_H_

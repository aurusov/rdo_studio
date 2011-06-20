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
class RDOPattern: public RDORuntimeObject, public RDOTraceableObject
{
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

	void preSelectRelRes(PTR(RDORuntime) runtime)
	{
		runCalcs(m_preSelectRelRes, runtime);
	}

	void runCalcs(REF(CalcList) calcList, PTR(RDORuntime) runtime)
	{
		LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
		runtime->getMemoryStack()->push(pLocalMemory);
		STL_FOR_ALL(calcList, calcIt)
			(*calcIt)->calcValue(runtime);
		runtime->getMemoryStack()->pop();
	}
	bool runCalcsBool(REF(CalcList) calcList, PTR(RDORuntime) runtime)
	{
		STL_FOR_ALL(calcList, calcIt)
		{
			if ( !(*calcIt)->calcValue( runtime ).getAsBool() ) return false;
		}
		return true;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternEvent
// ----------------------------------------------------------------------------
class RDOPatternEvent: public RDOPattern
{
friend class RDOEvent;

public:
	RDOPatternEvent(rbool trace);

	void addConvertorCalc  ( CREF(LPRDOCalc) pCalc             ) { m_convertor.push_back( pCalc );        }
	void addConvertorStatus( RDOResource::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( CREF(LPRDOCalc) pCalc             ) { m_erase.push_back( pCalc );            }

	void convertEvent( PTR(RDORuntime) runtime )
	{
		preSelectRelRes( runtime );
		runCalcs( m_convertor, runtime );
	}
	void convertErase( PTR(RDORuntime) runtime )
	{
		runCalcs( m_erase, runtime );
	}

	double getNextTimeInterval( PTR(RDORuntime) runtime );

	LPIEvent createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) oprName);

private:
	LPRDOCalc         m_timeCalc;
	CalcList          m_convertor;
	ConvertStatusList m_convertorStatus;
	CalcList          m_erase;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
class RDOPatternRule: public RDOPattern
{
friend class RDORule;

public:
	RDOPatternRule(rbool trace);

	void addChoiceFromCalc ( CREF(LPRDOCalc) pCalc             ) { m_choiceFrom.push_back( pCalc );       }
	void addConvertorCalc  ( CREF(LPRDOCalc) pCalc             ) { m_convertor.push_back( pCalc );        }
	void addConvertorStatus( RDOResource::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( CREF(LPRDOCalc) pCalc             ) { m_erase.push_back( pCalc );            }

	bool choiceFrom( PTR(RDORuntime) runtime )
	{
		preSelectRelRes( runtime );
		return runCalcsBool( m_choiceFrom, runtime );
	}
	void convertRule(PTR(RDORuntime) runtime)
	{
		runCalcs(m_convertor, runtime);
	}
	void convertErase(PTR(RDORuntime) runtime)
	{
		runCalcs(m_erase, runtime);
	}

	LPIRule createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) _oprName);
	LPIRule createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(LPRDOCalc) condition, CREF(tstring) _oprName);

private:
	CalcList          m_choiceFrom;
	CalcList          m_convertor;
	ConvertStatusList m_convertorStatus;
	CalcList          m_erase;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
class RDOPatternOperation: public RDOPattern
{
friend class RDOOperation;

public:
	RDOPatternOperation(rbool trace);

	void addChoiceFromCalc      ( CREF(LPRDOCalc) pCalc             ) { m_choiceFrom.push_back( pCalc );            }

	void addConvertorBeginCalc  ( CREF(LPRDOCalc) pCalc             ) { m_convertorBegin.push_back( pCalc );        }
	void addConvertorBeginStatus( RDOResource::ConvertStatus status ) { m_convertorBeginStatus.push_back( status ); }
	void addEraseBeginCalc      ( CREF(LPRDOCalc) pCalc             ) { m_eraseBegin.push_back( pCalc );            }

	void addConvertorEndCalc    ( CREF(LPRDOCalc) pCalc             ) { m_convertorEnd.push_back( pCalc );          }
	void addConvertorEndStatus  ( RDOResource::ConvertStatus status ) { m_convertorEndStatus.push_back( status );   }
	void addEraseEndCalc        ( CREF(LPRDOCalc) pCalc             ) { m_eraseEnd.push_back( pCalc );              }

	void setTime                ( CREF(LPRDOCalc) pCalc             ) { m_timeCalc = pCalc;                         }

	bool choiceFrom( PTR(RDORuntime) runtime )
	{
		preSelectRelRes( runtime );
		return runCalcsBool( m_choiceFrom, runtime );
	}
	void convertBegin( PTR(RDORuntime) runtime )
	{
		runCalcs( m_convertorBegin, runtime );
	}
	void convertBeginErase( PTR(RDORuntime) runtime )
	{
		runCalcs( m_eraseBegin, runtime );
	}
	void convertEnd( PTR(RDORuntime) runtime )
	{
		runCalcs( m_convertorEnd, runtime );
	}
	void convertEndErase( PTR(RDORuntime) runtime )
	{
		runCalcs( m_eraseEnd, runtime );
	}

	double getNextTimeInterval( PTR(RDORuntime) runtime );

	LPIOperation createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) _oprName);
	LPIOperation createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(LPRDOCalc) condition, CREF(tstring) _oprName);

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

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
class RDOPatternKeyboard: public RDOPatternOperation
{
public:
	RDOPatternKeyboard(rbool trace);

	LPIKeyboard createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) _oprName);
	LPIKeyboard createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(LPRDOCalc) condition, CREF(tstring) _oprName);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_PATTERN_H_

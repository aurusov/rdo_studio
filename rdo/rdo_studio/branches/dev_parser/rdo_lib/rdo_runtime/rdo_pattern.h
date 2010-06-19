#ifndef RDO_PATTERN_H
#define RDO_PATTERN_H

#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"

namespace rdoRuntime {

class RDORuntime;
class RDOIrregEvent;
class RDORule;
class RDOKeyboard;

// ----------------------------------------------------------------------------
// ---------- RDOPattern
// ----------------------------------------------------------------------------
class RDOPattern: public RDORuntimeParent, public RDOTraceableObject
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
	RDOPattern( PTR(RDORuntime) runtime, bool trace );
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
		STL_FOR_ALL(CalcList, calcList, calcIt)
			(*calcIt)->calcValue(runtime);
	}
	bool runCalcsBool(REF(CalcList) calcList, PTR(RDORuntime) runtime)
	{
		STL_FOR_ALL(CalcList, calcList, calcIt)
		{
			if ( !(*calcIt)->calcValue( runtime ).getAsBool() ) return false;
		}
		return true;
	}
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
class RDOPatternIrregEvent: public RDOPattern
{
friend class RDOIrregEvent;

public:
	RDOPatternIrregEvent( PTR(RDORuntime) rTime, bool trace );

	void addConvertorCalc  ( CREF(LPRDOCalc) pCalc             ) { m_convertor.push_back( pCalc );        }
	void addConvertorStatus( RDOResource::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( CREF(LPRDOCalc) pCalc             ) { m_erase.push_back( pCalc );            }
	void setTime           ( CREF(LPRDOCalc) timeCalc          ) { m_timeCalc = timeCalc;	              }

	void convertEvent(PTR(RDORuntime) runtime)
	{
		preSelectRelRes(runtime);
		runCalcs(m_convertor, runtime);
	}
	void convertErase(PTR(RDORuntime) runtime)
	{
		runCalcs(m_erase, runtime);
	}

	double getNextTimeInterval( PTR(RDORuntime) runtime );

	LPIIrregEvent createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) oprName);

private:
	LPRDOCalc         m_timeCalc;
	CalcList          m_convertor;
	ConvertStatusList m_convertorStatus;
	CalcList          m_erase;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternEvent
// ----------------------------------------------------------------------------
class RDOPatternEvent: public RDOPattern
{
friend class RDOEvent;

public:
	RDOPatternEvent( PTR(RDORuntime) rTime, bool trace );

	void addConvertorCalc  ( CREF(LPRDOCalc) pCalc                 ) { m_convertor.push_back( pCalc );        }
	void addConvertorStatus( RDOResource::ConvertStatus status     ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( CREF(LPRDOCalc) pCalc                 ) { m_erase.push_back( pCalc );            }
	void setTime           ( CREF(LPRDOCalc) timeCalc              ) { m_timeCalc = timeCalc;	              }

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
	RDOPatternRule( PTR(RDORuntime) rTime, bool trace );

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
	RDOPatternOperation( PTR(RDORuntime) rTime, bool trace );

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
	RDOPatternKeyboard( PTR(RDORuntime) rTime, bool trace );

	LPIKeyboard createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) _oprName);
	LPIKeyboard createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(LPRDOCalc) condition, CREF(tstring) _oprName);
};

} // namespace rdoRuntime

#endif // RDO_PATTERN_H

#ifndef RDO_PATTERN_H
#define RDO_PATTERN_H

#include "rdotrace.h"
#include "rdo_resource.h"

namespace rdoRuntime {

class RDOCalc;
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
	void addPreSelectRelRes( PTR(RDOCalc) calc )
	{
		std::vector< PTR(RDOCalc) >::iterator it = m_preSelectRelRes.begin();
		while ( it != m_preSelectRelRes.end() )
		{
			if ( *(*it) == *calc )
			{
				return;
			}
			it++;
		}
		m_preSelectRelRes.push_back( calc );
	}

protected:
	RDOPattern( PTR(RDORuntime) runtime, bool trace );
	virtual ~RDOPattern() {}

	std::vector< PTR(RDOCalc) > m_preSelectRelRes;
	void preSelectRelRes( PTR(RDORuntime) runtime )
	{
		runCalcs( m_preSelectRelRes, runtime );
	}

	void runCalcs( std::vector< PTR(RDOCalc) >& calcs, PTR(RDORuntime) runtime )
	{
		std::vector< PTR(RDOCalc) >::iterator it = calcs.begin();
		while ( it != calcs.end() )
		{
			(*it)->calcValue( runtime );
			it++;
		}
	}
	bool runCalcsBool( std::vector< PTR(RDOCalc) >& calcs, PTR(RDORuntime) runtime )
	{
		std::vector< PTR(RDOCalc) >::iterator it = calcs.begin();
		while ( it != calcs.end() )
		{
			if ( !(*it)->calcValue( runtime ).getAsBool() ) return false;
			it++;
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

	void addConvertorCalc  ( PTR(RDOCalc) calc                     ) { m_convertor.push_back( calc );         }
	void addConvertorStatus( RDOResource::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( PTR(RDOCalc) calc                     ) { m_erase.push_back( calc );             }
	void setTime           ( PTR(RDOCalc) timeCalc                 ) { m_timeCalc = timeCalc;	               }

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

	LPIIrregEvent createActivity(PTR(RDORuntime) runtime, CREF(tstring) oprName);

private:
	PTR(RDOCalc)                                  m_timeCalc;
	std::vector< PTR(RDOCalc) >                   m_convertor;
	std::vector< RDOResource::ConvertStatus > m_convertorStatus;
	std::vector< PTR(RDOCalc) >                   m_erase;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
class RDOPatternRule: public RDOPattern
{
friend class RDORule;

public:
	RDOPatternRule( PTR(RDORuntime) rTime, bool trace );

	void addChoiceFromCalc ( PTR(RDOCalc) calc                     ) { m_choiceFrom.push_back( calc );        }
	void addConvertorCalc  ( PTR(RDOCalc) calc                     ) { m_convertor.push_back( calc );         }
	void addConvertorStatus( RDOResource::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( PTR(RDOCalc) calc                     ) { m_erase.push_back( calc );             }

	bool choiceFrom( PTR(RDORuntime) runtime )
	{
		preSelectRelRes( runtime );
		return runCalcsBool( m_choiceFrom, runtime );
	}
	void convertRule( PTR(RDORuntime) runtime )
	{
		runCalcs( m_convertor, runtime );
	}
	void convertErase( PTR(RDORuntime) runtime )
	{
		runCalcs( m_erase, runtime );
	}

	LPIRule createActivity(PTR(RDORuntime) runtime, CREF(tstring) _oprName);
	LPIRule createActivity(PTR(RDORuntime) runtime, PTR(RDOCalc) condition, CREF(tstring) _oprName);

private:
	std::vector< PTR(RDOCalc) >                   m_choiceFrom;
	std::vector< PTR(RDOCalc) >                   m_convertor;
	std::vector< RDOResource::ConvertStatus > m_convertorStatus;
	std::vector< PTR(RDOCalc) >                   m_erase;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
class RDOPatternOperation: public RDOPattern
{
friend class RDOOperation;

public:
	RDOPatternOperation( PTR(RDORuntime) rTime, bool trace );

	void addChoiceFromCalc      ( PTR(RDOCalc) calc                     ) { m_choiceFrom.push_back( calc );             }

	void addConvertorBeginCalc  ( PTR(RDOCalc) calc                     ) { m_convertorBegin.push_back( calc );         }
	void addConvertorBeginStatus( RDOResource::ConvertStatus status ) { m_convertorBeginStatus.push_back( status ); }
	void addEraseBeginCalc      ( PTR(RDOCalc) calc                     ) { m_eraseBegin.push_back( calc );             }

	void addConvertorEndCalc    ( PTR(RDOCalc) calc                     ) { m_convertorEnd.push_back( calc );           }
	void addConvertorEndStatus  ( RDOResource::ConvertStatus status ) { m_convertorEndStatus.push_back( status );   }
	void addEraseEndCalc        ( PTR(RDOCalc) calc                     ) { m_eraseEnd.push_back( calc );               }

	void setTime                ( PTR(RDOCalc) calc                     ) { m_timeCalc = calc;                          }

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

	LPIOperation createActivity(PTR(RDORuntime) runtime, CREF(tstring) _oprName);
	LPIOperation createActivity(PTR(RDORuntime) runtime, PTR(RDOCalc) condition, CREF(tstring) _oprName);

private:
	PTR(RDOCalc)                                  m_timeCalc;
	std::vector< PTR(RDOCalc) >                   m_choiceFrom;

	std::vector< PTR(RDOCalc) >                   m_convertorBegin;
	std::vector< RDOResource::ConvertStatus > m_convertorBeginStatus;
	std::vector< PTR(RDOCalc) >                   m_eraseBegin;

	std::vector< PTR(RDOCalc) >                   m_convertorEnd;
	std::vector< RDOResource::ConvertStatus > m_convertorEndStatus;
	std::vector< PTR(RDOCalc) >                   m_eraseEnd;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
class RDOPatternKeyboard: public RDOPatternOperation
{
public:
	RDOPatternKeyboard( PTR(RDORuntime) rTime, bool trace );

	LPIKeyboard createActivity(PTR(RDORuntime) runtime, CREF(tstring) _oprName);
	LPIKeyboard createActivity(PTR(RDORuntime) runtime, PTR(RDOCalc) condition, CREF(tstring) _oprName);
};

} // namespace rdoRuntime

#endif // RDO_PATTERN_H

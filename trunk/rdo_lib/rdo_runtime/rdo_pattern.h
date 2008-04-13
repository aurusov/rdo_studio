#ifndef RDO_PATTERN_H
#define RDO_PATTERN_H

#include "rdotrace.h"

namespace rdoRuntime {

class RDOCalc;
class RDORuntime;
class RDOActivityRule;
class RDOActivityIrregEvent;
class RDOActivityKeyboard;


// ----------------------------------------------------------------------------
// ---------- RDOPattern
// ----------------------------------------------------------------------------
class RDOPattern: public RDORuntimeParent, public RDOTraceableObject
{
friend class RDOActivityRule;
friend class RDOActivityIrregEvent;
friend class RDOActivityOperation;

public:
	virtual ~RDOPattern() {}

	void addPreSelectRelRes( RDOCalc* calc )
	{
		std::vector< RDOCalc* >::iterator it = m_preSelectRelRes.begin();
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
	RDOPattern( RDORuntime* runtime, bool trace );

	std::vector< RDOCalc* > m_preSelectRelRes;
	void preSelectRelRes( RDORuntime* runtime )
	{
		runCalcs( m_preSelectRelRes, runtime );
	}

	void runCalcs( std::vector< RDOCalc* >& calcs, RDORuntime* runtime )
	{
		std::vector< RDOCalc* >::iterator it = calcs.begin();
		while ( it != calcs.end() )
		{
			(*it)->calcValueBase( runtime );
			it++;
		}
	}
	bool runCalcsBool( std::vector< RDOCalc* >& calcs, RDORuntime* runtime )
	{
		std::vector< RDOCalc* >::iterator it = calcs.begin();
		while ( it != calcs.end() )
		{
			if ( !(*it)->calcValueBase( runtime ).getBool() ) return false;
			it++;
		}
		return true;
	}

//	std::vector< RDOCalc* > m_choiceFromCalcs;
//	std::vector< RDOCalc* > m_beginCalcs;
//	std::vector< RDOCalc* > m_beginEraseCalcs;
//	std::vector< RDOResourceTrace::ConvertStatus > m_beginConvertStatus;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
class RDOPatternIrregEvent: public RDOPattern
{
friend class RDOActivityIrregEvent;

public:
	RDOPatternIrregEvent( RDORuntime* rTime, bool trace );

	void addConvertorCalc  ( RDOCalc* calc                          ) { m_convertor.push_back( calc );         }
	void addConvertorStatus( RDOResourceTrace::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( RDOCalc* calc                          ) { m_erase.push_back( calc );             }
	void setTime           ( RDOCalc* timeCalc                      ) { m_timeCalc = timeCalc;	               }

	void convertEvent( RDORuntime* runtime )
	{
		preSelectRelRes( runtime );
		runCalcs( m_convertor, runtime );
	}
	void convertErase( RDORuntime* runtime )
	{
		runCalcs( m_erase, runtime );
	}

	double getNextTimeInterval( RDORuntime* runtime );

	RDOActivityIrregEvent* createActivity( RDORuntime* runtime, const std::string& oprName );

private:
	RDOCalc*                                       m_timeCalc;
	std::vector< RDOCalc* >                        m_convertor;
	std::vector< RDOResourceTrace::ConvertStatus > m_convertorStatus;
	std::vector< RDOCalc* >                        m_erase;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
class RDOPatternRule: public RDOPattern
{
friend class RDOActivityRule;

public:
	RDOPatternRule( RDORuntime* rTime, bool trace );

	void addChoiceFromCalc ( RDOCalc* calc                          ) { m_choiceFrom.push_back( calc );        }
	void addConvertorCalc  ( RDOCalc* calc                          ) { m_convertor.push_back( calc );         }
	void addConvertorStatus( RDOResourceTrace::ConvertStatus status ) { m_convertorStatus.push_back( status ); }
	void addEraseCalc      ( RDOCalc* calc                          ) { m_erase.push_back( calc );             }

	bool choiceFrom( RDORuntime* runtime )
	{
		preSelectRelRes( runtime );
		return runCalcsBool( m_choiceFrom, runtime );
	}
	void convertRule( RDORuntime* runtime )
	{
		runCalcs( m_convertor, runtime );
	}
	void convertErase( RDORuntime* runtime )
	{
		runCalcs( m_erase, runtime );
	}

	RDOActivityRule* createActivity( RDORuntime* runtime, const std::string& _oprName );
	RDOActivityRule* createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName );

private:
	std::vector< RDOCalc* >                        m_choiceFrom;
	std::vector< RDOCalc* >                        m_convertor;
	std::vector< RDOResourceTrace::ConvertStatus > m_convertorStatus;
	std::vector< RDOCalc* >                        m_erase;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
class RDOPatternOperation: public RDOPattern
{
friend class RDOActivityOperation;

public:
	RDOPatternOperation( RDORuntime* rTime, bool trace );

	void addChoiceFromCalc      ( RDOCalc* calc                          ) { m_choiceFrom.push_back( calc );             }

	void addConvertorBeginCalc  ( RDOCalc* calc                          ) { m_convertorBegin.push_back( calc );         }
	void addConvertorBeginStatus( RDOResourceTrace::ConvertStatus status ) { m_convertorBeginStatus.push_back( status ); }
	void addEraseBeginCalc      ( RDOCalc* calc                          ) { m_eraseBegin.push_back( calc );             }

	void addConvertorEndCalc    ( RDOCalc* calc                          ) { m_convertorEnd.push_back( calc );           }
	void addConvertorEndStatus  ( RDOResourceTrace::ConvertStatus status ) { m_convertorEndStatus.push_back( status );   }
	void addEraseEndCalc        ( RDOCalc* calc                          ) { m_eraseEnd.push_back( calc );               }

	void setTime                ( RDOCalc* calc                          ) { m_timeCalc = calc;                          }

	bool choiceFrom( RDORuntime* runtime )
	{
		preSelectRelRes( runtime );
		return runCalcsBool( m_choiceFrom, runtime );
	}
	void convertBegin( RDORuntime* runtime )
	{
		runCalcs( m_convertorBegin, runtime );
	}
	void convertBeginErase( RDORuntime* runtime )
	{
		runCalcs( m_eraseBegin, runtime );
	}
	void convertEnd( RDORuntime* runtime )
	{
		runCalcs( m_convertorEnd, runtime );
	}
	void convertEndErase( RDORuntime* runtime )
	{
		runCalcs( m_eraseEnd, runtime );
	}

	double getNextTimeInterval( RDORuntime* runtime );

	RDOActivityOperation* createActivity( RDORuntime* runtime, const std::string& _oprName );
	RDOActivityOperation* createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName );

private:
	RDOCalc*                                       m_timeCalc;
	std::vector< RDOCalc* >                        m_choiceFrom;

	std::vector< RDOCalc* >                        m_convertorBegin;
	std::vector< RDOResourceTrace::ConvertStatus > m_convertorBeginStatus;
	std::vector< RDOCalc* >                        m_eraseBegin;

	std::vector< RDOCalc* >                        m_convertorEnd;
	std::vector< RDOResourceTrace::ConvertStatus > m_convertorEndStatus;
	std::vector< RDOCalc* >                        m_eraseEnd;
};

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
class RDOPatternKeyboard: public RDOPatternOperation
{
public:
	RDOPatternKeyboard( RDORuntime* rTime, bool trace );

	RDOActivityKeyboard* createActivity( RDORuntime* runtime, const std::string& _oprName );
	RDOActivityKeyboard* createActivity( RDORuntime* runtime, RDOCalc* condition, const std::string& _oprName );
};

} // namespace rdoRuntime

#endif // RDO_PATTERN_H

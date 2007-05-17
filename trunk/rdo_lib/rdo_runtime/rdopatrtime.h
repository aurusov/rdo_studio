#ifndef RDOPATRTIME_H
#define RDOPATRTIME_H

#include "rdotrace.h"
#include "ruletrace.h"
#include "ietrace.h"
#include "operationtrace.h"

//class RDOSimulator;
//class RDOResourceTrace;

namespace rdoRuntime {

class RDOCalc;
class RDORuntime;
class RDOActivityRuntime;

// ----------------------------------------------------------------------------
// ---------- RDOPatternRuntime
// ----------------------------------------------------------------------------
class RDOPatternRuntime: public RDORuntimeParent
{
friend class RDOActivityRuleRuntime;
friend class RDOActivityIERuntime;
friend class RDOActivityOperationRuntime;

protected:
	RDOPatternRuntime( RDORuntime* _runtime, bool _trace );

	std::vector< RDOCalc* > choiceFromCalcs;
	std::vector< RDOCalc* > beginCalcs;
	std::vector< RDOCalc* > beginEraseCalcs;
	std::string patternId;
	bool        trace;
	std::vector< RDOResourceTrace::ConvertStatus > beginConvertStatus;

public:
	virtual ~RDOPatternRuntime() {}

	bool choiceFrom( RDOSimulator* sim );
	void convertBegin( RDORuntime* _runtime );
	void convertBeginErase( RDORuntime* _runtime );

	void addChoiceFromCalc( RDOCalc* calc )         { choiceFromCalcs.push_back( calc ); }
	void addBeginCalc     ( RDOCalc* calc  )        { beginCalcs.push_back( calc );      }
	void addBeginEraseCalc( RDOCalc* calc  )        { beginEraseCalcs.push_back( calc ); }
	void addBeginConvertStatus( RDOResourceTrace::ConvertStatus status ) {
		beginConvertStatus.push_back( status );
	}

	const std::string& getPatternId() const         { return patternId;                  }
	const std::string& tracePatternId() const       { return patternId;                  }
	void setPatternId( int _id )                    { patternId = toString( _id );       }

	virtual void setTime( RDOCalc* _timeCalc )      {};

	virtual RDOActivityRuntime* createActivity( const std::string& _oprName ) = 0;
	virtual RDOActivityRuntime* createActivity( RDOCalc* condition, const std::string& _oprName );
};

// ----------------------------------------------------------------------------
// ---------- RDORuleRuntime
// ----------------------------------------------------------------------------
class RDORuleRuntime: public RDOPatternRuntime
{
public:
	RDORuleRuntime( RDORuntime* rTime, bool _trace );

	void convertRule( RDOSimulator* sim );

	virtual RDOActivityRuntime* createActivity( const std::string& _oprName );
	virtual RDOActivityRuntime* createActivity( RDOCalc* condition, const std::string& _oprName );
};

// ----------------------------------------------------------------------------
// ---------- RDOIERuntime
// ----------------------------------------------------------------------------
class RDOIERuntime: public RDOPatternRuntime
{
protected:
	RDOCalc* timeCalc;

public:
	RDOIERuntime( RDORuntime* rTime, bool _trace );

	void convertEvent( RDOSimulator* sim );

	double getNextTimeInterval( RDOSimulator* sim );
	virtual void setTime( RDOCalc* _timeCalc )        { timeCalc = _timeCalc; }

	virtual RDOActivityRuntime* createActivity( const std::string& _oprName );
};

// ----------------------------------------------------------------------------
// ---------- RDOOperationRuntime
// ----------------------------------------------------------------------------
class RDOOperationRuntime: public RDOPatternRuntime
{
friend class RDOActivityOperationRuntime;

private:
	std::vector< RDOCalc* >	endCalcs;
	std::vector< RDOCalc* >	endEraseCalcs;

protected:
	RDOCalc* timeCalc;
	std::vector< RDOResourceTrace::ConvertStatus > endConvertStatus;

public:
	RDOOperationRuntime( RDORuntime* rTime, bool _trace );

	void convertBegin( RDOSimulator* sim );
	void convertEnd( RDOSimulator* sim );
	void convertEndErase( RDORuntime* _runtime );

	void addEndCalc     ( RDOCalc* calc )             { endCalcs.push_back( calc );      }
	void addEndEraseCalc( RDOCalc* calc )             { endEraseCalcs.push_back( calc ); }
	void addEndConvertStatus( RDOResourceTrace::ConvertStatus status ) {
		endConvertStatus.push_back( status );
	}

	double getNextTimeInterval( RDOSimulator* sim );
	virtual void setTime( RDOCalc* _timeCalc )        { timeCalc = _timeCalc;            }

	virtual RDOActivityRuntime* createActivity( const std::string& _oprName );
	virtual RDOActivityRuntime* createActivity( RDOCalc* condition, const std::string& _oprName );
};

// ----------------------------------------------------------------------------
// ---------- RDOKeyboardRuntime
// ----------------------------------------------------------------------------
class RDOKeyboardRuntime: public RDOOperationRuntime
{
public:
	RDOKeyboardRuntime( RDORuntime* rTime, bool _trace );

	virtual RDOActivityRuntime* createActivity( const std::string& _oprName );
	virtual RDOActivityRuntime* createActivity( RDOCalc* condition, const std::string& _oprName );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityRuntime
// ----------------------------------------------------------------------------
class RDOActivityRuntime
{
protected:
	RDOActivityRuntime( RDOPatternRuntime* _pattern, const std::string& _oprName ):
		pattern( _pattern ),
		oprName( _oprName )
#ifdef RDOSIM_COMPATIBLE
		,
		show_create_index( true )
#endif
	{
	}
	virtual ~RDOActivityRuntime() {}

#ifdef RDOSIM_COMPATIBLE
	bool                           show_create_index;
#endif
	std::string                    oprName;
	std::list< RDOResourceTrace* > relevantResources; // Список релевантных ресурсов
	std::vector< int >             relResID;  // Содержит список id ресурсов, которые стали релевантными образцу
	std::vector< RDOCalc* >        paramsCalcs;
	RDOPatternRuntime*             pattern;
	void setPatternParameters( RDOSimulator* sim );
	void getRelevantResources( RDOSimulator* sim, std::list< RDOResourceTrace* >& rel_res_list );
	void incrementRelevantResourceReference( RDOSimulator* sim );
	void decrementRelevantResourceReference( RDOSimulator* sim );

	void updateRelRes( RDOSimulator* sim ) {
		getRelevantResources( sim, relevantResources );
	}
	void updateConvertStatus( RDOSimulator* sim, const std::vector< RDOResourceTrace::ConvertStatus >& status_list );

	std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim );
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim );

public:
	void addParamCalc( RDOCalc* calc ) { paramsCalcs.push_back( calc ); }
	int getResByRelRes( const int rel_res_id ) const
	{
		if ( relResID.size() <= rel_res_id ) {
			return 0;
		}
		return relResID.at( rel_res_id ); 
	} 
	void setRelRes( int rel_res_id, int res_id )
	{
		if ( relResID.size() <= rel_res_id ) {
			relResID.resize( rel_res_id + 1 );
		}
		relResID[rel_res_id] = res_id; 
	}
	virtual void addHotKey( std::string* hotKey ) {}
	void writeModelStructure( std::stringstream& stream );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityRuleRuntime
// ----------------------------------------------------------------------------
class RDOActivityRuleRuntime: public RDORuleTrace, public RDOActivityRuntime
{
private:
	bool choiceFrom( RDOSimulator* sim );
	void convertRule( RDOSimulator* sim );

	bool     haveAdditionalCondition;
	RDOCalc* additionalCondition;

	virtual const std::string& tracePatternId() const                                  { return pattern->tracePatternId();                     }
	virtual void onBeforeChoiceFrom( RDOSimulator* sim )                               { setPatternParameters(sim);                            }
	virtual void onAfterRule( RDOSimulator* sim, bool inSearch = false );
	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim ) {
		return RDOActivityRuntime::traceResourcesList( prefix, sim );
	}
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim ) {
		return RDOActivityRuntime::traceResourcesListNumbers( sim );
	}

public:
	RDOActivityRuleRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, const std::string& _oprName );
	RDOActivityRuleRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityIERuntime
// ----------------------------------------------------------------------------
class RDOActivityIERuntime: public RDOIETrace, public RDOActivityRuntime
{
private:
	bool choiceFrom( RDOSimulator* sim );
	void convertEvent( RDOSimulator* sim );

	double getNextTimeInterval(RDOSimulator *sim);
	virtual const std::string& tracePatternId() const                                  { return pattern->tracePatternId();                                   }
	virtual void onBeforeIrregularEvent( RDOSimulator* sim );
	virtual void onAfterIrregularEvent( RDOSimulator* sim );
	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim ) {
		return RDOActivityRuntime::traceResourcesList( prefix, sim );
	}
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim ) {
		return RDOActivityRuntime::traceResourcesListNumbers( sim );
	}

public:
	RDOActivityIERuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, const std::string& _oprName );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityOperationRuntime
// ----------------------------------------------------------------------------
class RDOActivityOperationRuntime: public RDOOperationTrace, public RDOActivityRuntime
{
private:
	void convertBegin(RDOSimulator *sim);
	void convertEnd(RDOSimulator *sim);

	bool     haveAdditionalCondition;
	RDOCalc* additionalCondition;

	double getNextTimeInterval(RDOSimulator *sim);
	virtual const std::string& tracePatternId() const                               { return pattern->tracePatternId();                     }
	RDOOperationTrace* clone2( RDOSimulator* sim );

	virtual void onBeforeChoiceFrom( RDOSimulator* sim);
	virtual void onBeforeOperationEnd( RDOSimulator* sim);
	virtual void onAfterOperationBegin( RDOSimulator* sim );
	virtual void onAfterOperationEnd( RDOSimulator* sim );
	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim ) {
		return RDOActivityRuntime::traceResourcesList( prefix, sim );
	}
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim ) {
		return RDOActivityRuntime::traceResourcesListNumbers( sim );
	}

protected:
	virtual bool choiceFrom( RDOSimulator* sim );

public:
	RDOActivityOperationRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, const std::string& _oprName );
	RDOActivityOperationRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityKeyboardRuntime
// ----------------------------------------------------------------------------
class RDOActivityKeyboardRuntime: public RDOActivityOperationRuntime
{
private:
	bool shift;
	bool control;
	unsigned int scan_code;

public:
	RDOActivityKeyboardRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, const std::string& _oprName );
	RDOActivityKeyboardRuntime( RDORuntime* rTime, RDOPatternRuntime* _pattern, bool _trace, RDOCalc* condition, const std::string& _oprName );

	void addHotKey( std::string* hotKey );
	bool choiceFrom( RDOSimulator* sim );
};

} // namespace rdoRuntime

#endif // RDOPATRTIME_H

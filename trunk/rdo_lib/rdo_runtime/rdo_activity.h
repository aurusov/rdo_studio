#ifndef RDO_ACTIVITY_H
#define RDO_ACTIVITY_H

#include "rdo_pattern.h"
#include "rdotrace.h"
#include "ruletrace.h"
#include "ietrace.h"
#include "operationtrace.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
class RDOActivity
{
public:
	void addParamCalc( RDOCalc* calc )
	{
		m_paramsCalcs.push_back( calc );
	}
	int getResByRelRes( const int rel_res_id ) const
	{
		if ( m_relResID.size() <= rel_res_id ) {
			return 0;
		}
		return m_relResID.at( rel_res_id ); 
	}
	void setRelRes( int rel_res_id, int res_id )
	{
		if ( m_relResID.size() <= rel_res_id ) {
			m_relResID.resize( rel_res_id + 1 );
		}
		m_relResID[rel_res_id] = res_id; 
	}

	enum AddHotKey {
		addhk_ok,
		addhk_already,
		addhk_notfound,
		addhk_dont
	};
	virtual AddHotKey addHotKey( RDORuntime* runtime, const std::string& hotKey )
	{
		return addhk_dont;
	}
	virtual bool hasHotKey() const { return false;      }

protected:
	RDOActivity( const std::string& name ):
		m_oprName( name )
	{
	}
	virtual ~RDOActivity() {}

	std::string                    m_oprName;
	std::list< RDOResourceTrace* > m_relevantResources; // Список релевантных ресурсов
	std::vector< int >             m_relResID;  // Содержит список id ресурсов, которые стали релевантными образцу
	std::vector< RDOCalc* >        m_paramsCalcs;

	void setPatternParameters( RDOSimulator* sim );
	void getRelevantResources( RDOSimulator* sim, std::list< RDOResourceTrace* >& rel_res_list );
	void incrementRelevantResourceReference( RDOSimulator* sim );
	void decrementRelevantResourceReference( RDOSimulator* sim );

	void updateRelRes( RDOSimulator* sim )
	{
		getRelevantResources( sim, m_relevantResources );
	}
	void updateConvertStatus( RDOSimulator* sim, const std::vector< RDOResourceTrace::ConvertStatus >& status_list );

	std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim );
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index = true );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityPattern
// ----------------------------------------------------------------------------
template< class T >
class RDOActivityPattern: public RDOActivity
{
public:
	void writeModelStructure( std::stringstream& stream )
	{
		stream << m_oprName << " " << m_pattern->traceId() << std::endl;
	}

protected:
	RDOActivityPattern( T* pattern, const std::string& name ):
		RDOActivity( name ),
		m_pattern( pattern )
	{
	}
	virtual ~RDOActivityPattern() {}

	T* m_pattern;
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityIrregEvent
// ----------------------------------------------------------------------------
class RDOActivityIrregEvent: public RDOIETrace, public RDOActivityPattern<RDOPatternIrregEvent>
{
private:
	void convertEvent( RDOSimulator* sim );

	double getNextTimeInterval(RDOSimulator *sim);
	virtual const std::string& tracePatternId() const                                  { return m_pattern->traceId(); }
	virtual void onBeforeIrregularEvent( RDOSimulator* sim );
	virtual void onAfterIrregularEvent( RDOSimulator* sim );
	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim ) {
		return RDOActivity::traceResourcesList( prefix, sim );
	}
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index = true ) {
		return RDOActivity::traceResourcesListNumbers( sim, show_create_index );
	}

public:
	RDOActivityIrregEvent( RDORuntime* rTime, RDOPatternIrregEvent* pattern, bool trace, const std::string& name );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityRule
// ----------------------------------------------------------------------------
class RDOActivityRule: public RDORuleTrace, public RDOActivityPattern<RDOPatternRule>
{
private:
	bool choiceFrom( RDORuntime* runtime );
	void convertRule( RDORuntime* runtime );

	bool     haveAdditionalCondition;
	RDOCalc* additionalCondition;

	virtual const std::string& tracePatternId() const                                  { return m_pattern->traceId();                     }
	virtual void onBeforeChoiceFrom( RDOSimulator* sim )                               { setPatternParameters(sim);                     }
	virtual void onAfterRule( RDOSimulator* sim, bool inSearch = false );
	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim ) {
		return RDOActivity::traceResourcesList( prefix, sim );
	}
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index = true ) {
		return RDOActivity::traceResourcesListNumbers( sim, show_create_index );
	}

public:
	RDOActivityRule( RDORuntime* rTime, RDOPatternRule* pattern, bool trace, const std::string& name );
	RDOActivityRule( RDORuntime* rTime, RDOPatternRule* pattern, bool trace, RDOCalc* condition, const std::string& name );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityOperation
// ----------------------------------------------------------------------------
class RDOActivityOperation: public RDOOperationTrace, public RDOActivityPattern<RDOPatternOperation>
{
private:
	void convertBegin(RDOSimulator *sim);
	void convertEnd(RDOSimulator *sim);

	bool     haveAdditionalCondition;
	RDOCalc* additionalCondition;

	double getNextTimeInterval(RDOSimulator *sim);
	virtual const std::string& tracePatternId() const    { return m_pattern->traceId(); }
	RDOOperationTrace* clone2( RDOSimulator* sim );

	virtual void onBeforeChoiceFrom( RDOSimulator* sim);
	virtual void onBeforeOperationEnd( RDOSimulator* sim);
	virtual void onAfterOperationBegin( RDOSimulator* sim );
	virtual void onAfterOperationEnd( RDOSimulator* sim );
	virtual std::string traceResourcesList( char prefix, RDOSimulatorTrace* sim ) {
		return RDOActivity::traceResourcesList( prefix, sim );
	}
	virtual std::string traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index = true ) {
		return RDOActivity::traceResourcesListNumbers( sim, show_create_index );
	}

protected:
	virtual bool choiceFrom( RDOSimulator* sim );

public:
	RDOActivityOperation( RDORuntime* rTime, RDOPatternOperation* pattern, bool trace, const std::string& name );
	RDOActivityOperation( RDORuntime* rTime, RDOPatternOperation* pattern, bool trace, RDOCalc* condition, const std::string& name );
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityKeyboard
// ----------------------------------------------------------------------------
class RDOActivityKeyboard: public RDOActivityOperation
{
public:
	RDOActivityKeyboard( RDORuntime* rTime, RDOPatternOperation* pattern, bool _trace, const std::string& name );
	RDOActivityKeyboard( RDORuntime* rTime, RDOPatternOperation* pattern, bool _trace, RDOCalc* condition, const std::string& name );

private:
	bool shift;
	bool control;
	int scan_code;

	virtual bool choiceFrom( RDOSimulator* sim );
	virtual AddHotKey addHotKey( RDORuntime* runtime, const std::string& hotKey );
	virtual bool hasHotKey() const { return scan_code != -1 ? true : false; }
};

} // namespace rdoRuntime

#endif // RDO_ACTIVITY_H

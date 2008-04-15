#include "pch.h"
#include "rdo_activity.h"
#include "rdo_runtime.h"
#include "rdocalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
void RDOActivity::setPatternParameters(RDOSimulator *sim) 
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	int size = m_paramsCalcs.size();
	for ( int i = 0; i < size; i++ )
	{
		m_paramsCalcs.at(i)->calcValue( runtime );
	}
}

void RDOActivity::getRelevantResources( RDOSimulator* sim, std::list< RDOResource* >& rel_res_list )
{
	rel_res_list.clear();
	int size = m_relResID.size();
	for ( int i = 0; i < size; i++ )
	{
		rel_res_list.push_back( static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID[i] ) );
	}
}

void RDOActivity::updateConvertStatus( RDOSimulator* sim, const std::vector< RDOResource::ConvertStatus >& status_list )
{
	updateRelRes( sim );
	int i = 0;
	std::list< RDOResource* >::iterator it = m_relevantResources.begin();
	while ( it != m_relevantResources.end() ) {
		RDOResource* res = *it;
		if ( res ) {
			res->setState( status_list[i] );
			switch ( status_list[i] ) {
				case RDOResource::CS_Create: {
					res->makeTemporary( true );
					break;
				}
			}
		}
		i++;
		it++;
	}
}

std::string RDOActivity::traceResourcesList( char prefix, RDOSimulatorTrace* sim )
{
	std::string res;
	for ( std::list< RDOResource* >::const_iterator i = m_relevantResources.begin(); i != m_relevantResources.end(); i++ ) {
		if ( *i ) {
			res += (*i)->traceResourceState( prefix, sim );
		}
	}
	return res;
}

std::string RDOActivity::traceResourcesListNumbers( RDOSimulatorTrace* sim, bool show_create_index )
{
	std::ostringstream res;
	res << m_relevantResources.size() << " ";
	for ( std::list< RDOResource* >::const_iterator i = m_relevantResources.begin(); i != m_relevantResources.end(); i++ ) {
#ifdef RDOSIM_COMPATIBLE
		if ( *i && (show_create_index || (!show_create_index && (*i)->getState() != RDOResource::CS_Create)) ) {
#else
		if ( *i ) {
#endif
			res << " " << (*i)->traceId();
		} else {
			res << " 0";
		}
	}
	return res.str();
}

RDOOperationTrace *RDOActivityOperation::clone2(RDOSimulator *sim) 
{ 
	RDORuntime *runtime = (RDORuntime *)sim;
	RDOActivityOperation *newActivity = new RDOActivityOperation( runtime, m_pattern, traceable(), m_oprName );
	newActivity->m_paramsCalcs.insert(newActivity->m_paramsCalcs.begin(), m_paramsCalcs.begin(), m_paramsCalcs.end());
	newActivity->m_relResID.insert(newActivity->m_relResID.begin(), m_relResID.begin(), m_relResID.end());

	return newActivity;
}

void RDOActivity::incrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( int i = 0; i < m_relResID.size(); i++ )
	{
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID.at(i) );
		if ( res && (res->getState() == RDOResource::CS_Keep || res->getState() == RDOResource::CS_Create || res->getState() == RDOResource::CS_Erase ) ) res->incRef();
	}
}

void RDOActivity::decrementRelevantResourceReference( RDOSimulator* sim )
{
	for ( int i = 0; i < m_relResID.size(); i++ )
	{
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( m_relResID.at(i) );
		if ( res && (res->getState() == RDOResource::CS_Keep || res->getState() == RDOResource::CS_Create || res->getState() == RDOResource::CS_Erase ) ) res->decRef();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityIrregEvent
// ----------------------------------------------------------------------------
RDOActivityIrregEvent::RDOActivityIrregEvent( RDORuntime* rTime, RDOPatternIrregEvent* pattern, bool trace, const std::string& name ):
	RDOIETrace( rTime, trace ),
	RDOActivityPattern<RDOPatternIrregEvent>( pattern, name )
{
	setTrace( trace );
}

void RDOActivityIrregEvent::convertEvent( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertEvent( static_cast<RDORuntime*>(sim) ); 
}

void RDOActivityIrregEvent::onBeforeIrregularEvent( RDOSimulator* sim )
{
	setPatternParameters( sim );
	RDOIETrace::onBeforeIrregularEvent( sim );
}

void RDOActivityIrregEvent::onAfterIrregularEvent( RDOSimulator* sim )
{
	updateConvertStatus( sim, m_pattern->m_convertorStatus );
	RDOIETrace::onAfterIrregularEvent( sim );
	m_pattern->convertErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOActivityIrregEvent::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return m_pattern->getNextTimeInterval( static_cast<RDORuntime*>(sim) ); 
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityRule
// ----------------------------------------------------------------------------
RDOActivityRule::RDOActivityRule( RDORuntime* rTime, RDOPatternRule* pattern, bool trace, const std::string& name ):
	RDORuleTrace( rTime, trace ),
	RDOActivityPattern<RDOPatternRule>( pattern, name ),
	additionalCondition( NULL )
{
	haveAdditionalCondition = false;
}

RDOActivityRule::RDOActivityRule( RDORuntime* rTime, RDOPatternRule* pattern, bool trace, RDOCalc* condition, const std::string& name ):
	RDORuleTrace( rTime, trace ),
	RDOActivityPattern<RDOPatternRule>( pattern, name ),
	additionalCondition( condition )
{
	haveAdditionalCondition = true;
}

bool RDOActivityRule::choiceFrom( RDORuntime* runtime )
{ 
	runtime->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValue( runtime ).getBool() ) {
			return false;
		}
	}
	return m_pattern->choiceFrom( runtime ); 
}

void RDOActivityRule::convertRule( RDORuntime* runtime ) 
{ 
	runtime->setCurrentActivity( this );
	m_pattern->convertRule( runtime ); 
}

void RDOActivityRule::onAfterRule( RDOSimulator* sim, bool inSearch )
{
	updateConvertStatus( sim, m_pattern->m_convertorStatus );
	RDORuleTrace::onAfterRule( sim, inSearch );
	m_pattern->convertErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityOperation
// ----------------------------------------------------------------------------
RDOActivityOperation::RDOActivityOperation( RDORuntime* rTime, RDOPatternOperation* pattern, bool trace, const std::string& name ):
	RDOOperationTrace( rTime, trace ),
	RDOActivityPattern<RDOPatternOperation>( pattern, name ),
	additionalCondition( NULL )
{
	setTrace( trace );
	haveAdditionalCondition = false;
}

RDOActivityOperation::RDOActivityOperation( RDORuntime* rTime, RDOPatternOperation* pattern, bool trace, RDOCalc* condition, const std::string& name ):
	RDOOperationTrace( rTime, trace ),
	RDOActivityPattern<RDOPatternOperation>( pattern, name ),
	additionalCondition( condition )
{
	setTrace( trace );
	haveAdditionalCondition = true;
}

bool RDOActivityOperation::choiceFrom( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	if ( haveAdditionalCondition ) {
		if ( !additionalCondition->calcValue( static_cast<RDORuntime*>(sim) ).getBool() ) {
			return false;
		}
	}
	return m_pattern->choiceFrom( static_cast<RDORuntime*>(sim) ); 
}

void RDOActivityOperation::convertBegin( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertBegin( static_cast<RDORuntime*>(sim) );
}

void RDOActivityOperation::convertEnd( RDOSimulator* sim )
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	m_pattern->convertEnd( static_cast<RDORuntime*>(sim) );
}

void RDOActivityOperation::onBeforeChoiceFrom( RDOSimulator* sim)
{
	RDOOperationTrace::onBeforeChoiceFrom( sim );
	setPatternParameters( sim );
}

void RDOActivityOperation::onBeforeOperationEnd( RDOSimulator* sim)
{
	setPatternParameters( sim );
	RDOOperationTrace::onBeforeOperationEnd( sim );
}

void RDOActivityOperation::onAfterOperationBegin( RDOSimulator* sim )
{
	updateConvertStatus( sim, m_pattern->m_convertorBeginStatus );
	RDOOperationTrace::onAfterOperationBegin( sim );
	m_pattern->convertBeginErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
	updateConvertStatus( sim, m_pattern->m_convertorEndStatus );
	incrementRelevantResourceReference( sim );
	updateConvertStatus( sim, m_pattern->m_convertorBeginStatus );
}

void RDOActivityOperation::onAfterOperationEnd( RDOSimulator* sim )
{
	updateConvertStatus( sim, m_pattern->m_convertorEndStatus );
	decrementRelevantResourceReference( sim );
	RDOOperationTrace::onAfterOperationEnd( sim );
	m_pattern->convertEndErase( static_cast<RDORuntime*>(sim) );
	updateRelRes( sim );
}

double RDOActivityOperation::getNextTimeInterval( RDOSimulator* sim ) 
{ 
	static_cast<RDORuntime*>(sim)->setCurrentActivity( this );
	return m_pattern->getNextTimeInterval( static_cast<RDORuntime*>(sim) ); 
}

// ----------------------------------------------------------------------------
// ---------- RDOActivityKeyboard
// ----------------------------------------------------------------------------
RDOActivityKeyboard::RDOActivityKeyboard( RDORuntime* rTime, RDOPatternOperation* pattern, bool _trace, const std::string& name ):
	RDOActivityOperation( rTime, pattern, _trace, name ),
	shift( false ),
	control( false ),
	scan_code( -1 )
{
}

RDOActivityKeyboard::RDOActivityKeyboard( RDORuntime* rTime, RDOPatternOperation* pattern, bool _trace, RDOCalc* condition, const std::string& name ):
	RDOActivityOperation( rTime, pattern, _trace, condition, name ),
	shift( false ),
	control( false ),
	scan_code( -1 )
{
}

RDOActivity::AddHotKey RDOActivityKeyboard::addHotKey( RDORuntime* runtime, const std::string& hotKey )
{
	unsigned int _scan_code = runtime->rdoHotKeyToolkit.codeFromString( hotKey );
	if ( _scan_code == -1 ) return RDOActivity::addhk_notfound;
	if ( scan_code != -1 && _scan_code != VK_SHIFT && _scan_code != VK_CONTROL ) return RDOActivity::addhk_already;
	switch ( _scan_code ) {
		case VK_SHIFT  : shift     = true; runtime->using_scan_codes.push_back( VK_SHIFT   ); break;
		case VK_CONTROL: control   = true; runtime->using_scan_codes.push_back( VK_CONTROL ); break;
		default        : scan_code = _scan_code; if ( scan_code ) runtime->using_scan_codes.push_back( _scan_code ); break;
	}
	return RDOActivity::addhk_ok;
}

bool RDOActivityKeyboard::choiceFrom( RDOSimulator *sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	runtime->setCurrentActivity( this );

	if ( !runtime->checkAreaActivated( m_oprName ) ) {
		if ( !runtime->checkKeyPressed( scan_code, shift, control ) ) return false;
	}
	return RDOActivityOperation::choiceFrom( sim ); 
}

} // namespace rdoRuntime

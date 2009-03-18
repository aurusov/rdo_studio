#include "pch.h"
#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"
#include "rdofrm.h"
#include "rdofun.h"
#include "rdoopr.h"
#include "rdorss.h"
#include "rdodpt.h"
#include "rdopmd.h"
#include <rdocommon.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDOParser
// ----------------------------------------------------------------------------
std::list< RDOParser* > RDOParser::s_parserStack;

#define DECLARE_SIMPLE_OBJECT_CONTAINER_NONAME( Name ) \
void RDOParser::insert##Name( RDO##Name* value ) \
{ \
	m_all##Name.push_back( value ); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER_NONAME( Name ) \
void RDOParser::insert##Name( RDO##Name* value ) \
{ \
	m_parsing_object = (RDOParserObject*)value; \
	m_all##Name.push_back( value ); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER( Name ) \
DECLARE_PARSER_OBJECT_CONTAINER_NONAME( Name ) \
const RDO##Name* RDOParser::find##Name( const std::string& name ) const \
{ \
	std::vector< RDO##Name* >::const_iterator it = std::find_if( m_all##Name.begin(), m_all##Name.end(), compareName<RDO##Name>(name) ); \
	return it != m_all##Name.end() ? *it : NULL; \
}

DECLARE_PARSER_OBJECT_CONTAINER( PATPattern      );
DECLARE_PARSER_OBJECT_CONTAINER( RTPResType      );
DECLARE_PARSER_OBJECT_CONTAINER( RSSResource     );
DECLARE_PARSER_OBJECT_CONTAINER( OPROperation    );
DECLARE_PARSER_OBJECT_CONTAINER( FRMFrame        );
DECLARE_PARSER_OBJECT_CONTAINER( FUNConstant     );
DECLARE_PARSER_OBJECT_CONTAINER( FUNFunction     );
DECLARE_PARSER_OBJECT_CONTAINER( FUNSequence     );
DECLARE_PARSER_OBJECT_CONTAINER( DPTSearch       );
DECLARE_PARSER_OBJECT_CONTAINER( DPTSome         );
DECLARE_PARSER_OBJECT_CONTAINER( DPTFreeActivity );
DECLARE_PARSER_OBJECT_CONTAINER( PMDPokaz        );

DECLARE_PARSER_OBJECT_CONTAINER_NONAME( FUNGroup    );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME( DPTFree     );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME( PROCProcess );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME( Operations  );

DECLARE_SIMPLE_OBJECT_CONTAINER_NONAME( RTPEnumParamType );

rdoModelObjects::RDOFileType RDOParser::getFileToParse()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->m_type : rdoModelObjects::PAT;
}

int RDOParser::lexer_loc_line()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_parser_item ? s_parserStack.back()->m_parser_item->lexer_loc_line() : -1;
}

int RDOParser::lexer_loc_pos()
{
	return m_parser_item ? m_parser_item->lexer_loc_pos() : 0;
}

RDOParser::RDOParser():
	m_parsing_object( NULL ),
	m_parser_item( NULL ),
	m_have_kw_Resources( false ),
	m_have_kw_ResourcesEnd( false ),
	m_smr( NULL )
{
	s_parserStack.push_back( this );
	m_runtime.memory_insert( sizeof(RDOParser) );
	m_runtime.memory_insert( sizeof(rdoRuntime::RDORuntime) );
}

RDOParser::~RDOParser()
{
	rdo::deleteAllObjects( m_allValues );
	std::vector< RDODeletable* >::reverse_iterator it = m_allDeletables.rbegin();
	while ( it != m_allDeletables.rend() ) {
		delete *it;
		it = m_allDeletables.rbegin();
	}
	TRACE( "PARSER : m_allDeletables.size() = %d\n",m_allDeletables.size() );
	s_parserStack.remove( this );
}

bool RDOParser::isCurrentDPTSearch()
{
	return getLastDPTSearch() && !getLastDPTSearch()->closed() ? true : false;
}

void RDOParser::insertChanges( const std::string& name, const std::string& value )
{
	m_changes.push_back( Changes( name, value ) );
}

std::string RDOParser::getChanges() const
{
	std::stringstream stream;
	stream << "$Changes" << std::endl;
	unsigned int changes_max_length = 0;
	std::vector< Changes >::const_iterator change_it = m_changes.begin();
	while ( change_it != m_changes.end() ) {
		if ( change_it->m_name.length() > changes_max_length ) {
			changes_max_length = change_it->m_name.length();
		}
		change_it++;
	}
	change_it = m_changes.begin();
	while ( change_it != m_changes.end() ) {
		stream << "  " << change_it->m_name;
		for ( unsigned int i = change_it->m_name.length(); i < changes_max_length; i++ ) {
			stream << " ";
		}
		stream << "  = " << change_it->m_value << std::endl;
		change_it++;
	}
	return stream.str();
}

std::string RDOParser::getModelStructure()
{
	std::stringstream modelStructure;

	// $Changes
	modelStructure << getChanges();

	// RTP
	modelStructure << std::endl << std::endl << "$Resource_type" << std::endl;
	std::vector< RDORTPResType* >::const_iterator rtp_it = m_allRTPResType.begin();
	while ( rtp_it != m_allRTPResType.end() )
	{
		(*rtp_it)->writeModelStructure( modelStructure );
		rtp_it++;
	}

	// RSS
	modelStructure << std::endl << "$Resources" << std::endl;
	std::vector< RDORSSResource* >::const_iterator rss_it = m_allRSSResource.begin();
	while ( rss_it != m_allRSSResource.end() )
	{
		(*rss_it)->writeModelStructure( modelStructure );
		rss_it++;
	}

	// PAT
	modelStructure << std::endl << "$Pattern" << std::endl;
	std::vector< RDOPATPattern* >::const_iterator pat_it = m_allPATPattern.begin();
	while ( pat_it != m_allPATPattern.end() )
	{
		(*pat_it)->writeModelStructure( modelStructure );
		pat_it++;
	}

	// OPR/DPT
	int counter = 1;
	modelStructure << std::endl << "$Activities" << std::endl;
	modelStructure << m_runtime.writeActivitiesStructure( counter );

	// DPT only
	for( unsigned int i = 0; i < m_allDPTSearch.size(); i++ ) {
		for( unsigned int j = 0; j < m_allDPTSearch.at(i)->getActivities().size(); j++ ) {
			RDODPTSearchActivity* curr = m_allDPTSearch.at(i)->getActivities().at(j);
			modelStructure << counter++ << " " << curr->name() << " " << curr->pattern()->getPatternId() << std::endl;
		}
	}

	// PMD
	modelStructure << std::endl << "$Watching" << std::endl;
	unsigned int watching_max_length = 0;
	std::vector< rdoRuntime::RDOPMDPokaz* >::const_iterator watching_it = m_runtime.getPokaz().begin();
	while ( watching_it != m_runtime.getPokaz().end() ) {
		if ( (*watching_it)->traceable() && (*watching_it)->name().length() > watching_max_length ) {
			watching_max_length = (*watching_it)->name().length();
		}
		watching_it++;
	}
	watching_it = m_runtime.getPokaz().begin();
	while ( watching_it != m_runtime.getPokaz().end() ) {
		if ( (*watching_it)->traceable() ) {
			modelStructure << "  " << (*watching_it)->name();
			for ( unsigned int i = (*watching_it)->name().length(); i < watching_max_length + 2; i++ ) {
				modelStructure << " ";
			}
			(*watching_it)->writePokazStructure( modelStructure );
		}
		watching_it++;
	}

	return modelStructure.str();
}

void RDOParser::parse()
{
	parse( rdoModelObjects::obPRE );

	RDOParserContainer::CIterator it = begin();
	while ( it != end() ) {
		m_parser_item = it->second;
		it->second->parse();
		m_parser_item = NULL;
		it++;
	}

	parse( rdoModelObjects::obPOST );
}

void RDOParser::parse( rdoModelObjects::RDOParseType file )
{
	int min, max;
	RDOParserContainer::getMinMax( file, min, max );
	if ( min == -1 || max == -1 ) return;
	RDOParserContainer::CIterator it = find( min );
	while ( it != end() ) {
		if ( it->first <= max ) {
			m_parser_item = it->second;
			it->second->parse();
			m_parser_item = NULL;
		} else {
			break;
		}
		it++;
	}
}

void RDOParser::parse( std::istream& stream )
{
	RDOParserContainer::CIterator it = begin();
	while ( it != end() ) {
		m_parser_item = it->second;
		it->second->parse( stream );
		m_parser_item = NULL;
		it++;
	}
}

void RDOParser::error( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... )
{
	va_list params;
	va_start( params, _error_code );
	std::string str = rdoSimulator::RDOSyntaxError::getMessage( _error_code, params );
	va_end( params );
	error( str, _error_code );
}

void RDOParser::error_push_only( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... )
{
	va_list params;
	va_start( params, _error_code );
	std::string str = rdoSimulator::RDOSyntaxError::getMessage( _error_code, params );
	va_end( params );
	error_push_only( str, _error_code );
}

void RDOParser::error( const RDOParserSrcInfo& _src_info, rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... )
{
	va_list params;
	va_start( params, _error_code );
	std::string str = rdoSimulator::RDOSyntaxError::getMessage( _error_code, params );
	va_end( params );
	error( _src_info, str, _error_code );
}

void RDOParser::error_push_only( const RDOParserSrcInfo& _src_info, rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... )
{
	va_list params;
	va_start( params, _error_code );
	std::string str = rdoSimulator::RDOSyntaxError::getMessage( _error_code, params );
	va_end( params );
	error_push_only( _src_info, str, _error_code );
}

void RDOParser::error( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	error_push_only( _message, _error_code );
	throw rdoParse::RDOSyntaxException( m_errors.back().message );
}

void RDOParser::error_push_only( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	m_errors.push_back( rdoSimulator::RDOSyntaxError( _error_code, _message, lexer_loc_line(), lexer_loc_pos(), getFileToParse() ) );
}

void RDOParser::error( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	error_push_only( _src_info, _message, _error_code );
	throw rdoParse::RDOSyntaxException( m_errors.back().message );
}

void RDOParser::error( const RDOParserSrcInfo& _src_info1, const RDOParserSrcInfo& _src_info2, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	error_push_only( _src_info1.src_pos().m_last_line != _src_info2.src_pos().m_last_line ? _src_info1 : _src_info2, _message, _error_code );
	throw rdoParse::RDOSyntaxException( m_errors.back().message );
}

void RDOParser::error_push_only( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	if ( _src_info.src_pos().m_last_line != rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE && _src_info.src_pos().m_last_pos != rdoRuntime::RDOSrcInfo::Position::UNDEFINE_POS )
	{
		m_errors.push_back( rdoSimulator::RDOSyntaxError( _error_code, _message, _src_info.src_pos().m_last_line, _src_info.src_pos().m_last_pos, _src_info.src_filetype() ) );
	}
}

void RDOParser::error_push_done()
{
	if ( !m_errors.empty() )
	{
		throw rdoParse::RDOSyntaxException( m_errors.back().message );
	}
}

void RDOParser::error_modify( const std::string& _message )
{
	if ( !m_errors.empty() )
	{
		m_errors.front().message = _message + m_errors.front().message;
		throw rdoParse::RDOSyntaxException( "" );
	}
}

void RDOParser::warning( rdoSimulator::RDOSyntaxError::ErrorCode _error_code, ... )
{
	va_list params;
	va_start( params, _error_code );
	std::string str = rdoSimulator::RDOSyntaxError::getMessage( _error_code, params );
	va_end( params );
	warning( str, _error_code );
}

void RDOParser::warning( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code ) 
{
	m_errors.push_back( rdoSimulator::RDOSyntaxError( _error_code, _message, lexer_loc_line(), lexer_loc_pos(), getFileToParse(), true ) );
}

void RDOParser::warning( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code ) 
{
	m_errors.push_back( rdoSimulator::RDOSyntaxError( _error_code, _message, _src_info.src_pos().m_last_line, _src_info.src_pos().m_last_pos, _src_info.src_filetype(), true ) );
}

void RDOParser::checkFunctionName( const RDOParserSrcInfo& _src_info )
{
	const RDOFUNConstant* _const = findFUNConstant( _src_info.src_text() );
	if ( _const ) {
		error_push_only( _src_info, rdo::format("Константа '%s' уже существует", _src_info.src_text().c_str()) );
//		parser->error("Second appearance of the same constant name: " + *(_cons->getName()));
		error_push_only( _const->src_info(), "См. первое определение" );
		error_push_done();
	}
	const RDOFUNSequence* _seq = findFUNSequence( _src_info.src_text() );
	if ( _seq ) {
		error_push_only( _src_info, rdo::format( "Последовательность '%s' уже существует", _src_info.src_text().c_str() ) );
		error_push_only( _seq->src_info(), "См. первое определение" );
		error_push_done();
	}
	const RDOFUNFunction* _fun = findFUNFunction( _src_info.src_text() );
	if ( _fun ) {
		error_push_only( _src_info, rdo::format( "Функция '%s' уже существует", _src_info.src_text().c_str() ) );
		error_push_only( _fun->src_info(), "См. первое определение" );
		error_push_done();
	}
}

void RDOParser::checkActivityName( const RDOParserSrcInfo& _src_info )
{
	std::vector< RDODPTSearch* >::const_iterator it_search = getDPTSearchs().begin();
	while ( it_search != getDPTSearchs().end() ) {
		std::vector< RDODPTSearchActivity* >::const_iterator it_search_act = std::find_if( (*it_search)->getActivities().begin(), (*it_search)->getActivities().end(), compareName<RDODPTSearchActivity>(_src_info.src_text()) );
		if ( it_search_act != (*it_search)->getActivities().end() ) {
			error_push_only( _src_info, rdo::format("Активность '%s' уже существует", _src_info.src_text().c_str()) );
			error_push_only( (*it_search_act)->src_info(), "См. первое определение" );
			error_push_done();
//			error("Activity name: " + *_name + " already defined");
		}
		it_search++;
	}
	std::vector< RDODPTSome* >::const_iterator it_some = getDPTSomes().begin();
	while ( it_some != getDPTSomes().end() ) {
		std::vector< RDODPTSomeActivity* >::const_iterator it_some_act = std::find_if( (*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName<RDODPTSomeActivity>(_src_info.src_text()) );
		if ( it_some_act != (*it_some)->getActivities().end() ) {
			error_push_only( _src_info, rdo::format("Активность '%s' уже существует", _src_info.src_text().c_str()) );
			error_push_only( (*it_some_act)->src_info(), "См. первое определение" );
			error_push_done();
		}
		it_some++;
	}
	const RDODPTFreeActivity* free_act = findDPTFreeActivity( _src_info.src_text() );
	if ( free_act ) {
		error_push_only( _src_info, rdo::format("Активность '%s' уже существует", _src_info.src_text().c_str()) );
		error_push_only( free_act->src_info(), "См. первое определение" );
		error_push_done();
//		error("Free activity name: " + *_name + " already defined");
	}
	const RDOOPROperation* opr = findOPROperation( _src_info.src_text() );
	if ( opr ) {
		error_push_only( _src_info, rdo::format("Операция '%s' уже существует", _src_info.src_text().c_str()) );
		error_push_only( opr->src_info(), "См. первое определение" );
		error_push_done();
	}
}

void RDOParser::checkDPTName( const RDOParserSrcInfo& _src_info )
{
	if ( _src_info.src_text().empty() )
	{
		// Актуально для операций и свободных блоков активностей
		return;
	}
	std::vector< RDODPTSearch* >::const_iterator search_it = std::find_if(getDPTSearchs().begin(), getDPTSearchs().end(), compareName<RDODPTSearch>(_src_info.src_text()));
	if ( search_it != getDPTSearchs().end() ) {
		error_push_only( _src_info, rdo::format("Точка '%s' уже существует", _src_info.src_text().c_str()) );
		error_push_only( (*search_it)->src_info(), "См. первое определение" );
		error_push_done();
//		error( _src_info, "DPT name: " + _src_info.src_text() + " already defined" );
	}
	std::vector< RDODPTSome* >::const_iterator some_it = std::find_if(getDPTSomes().begin(), getDPTSomes().end(), compareName<RDODPTSome>(_src_info.src_text()));
	if ( some_it != getDPTSomes().end() ) {
		error_push_only( _src_info, rdo::format("Точка '%s' уже существует", _src_info.src_text().c_str()) );
		error_push_only( (*some_it)->src_info(), "См. первое определение" );
		error_push_done();
	}
}

} // namespace rdoParse 

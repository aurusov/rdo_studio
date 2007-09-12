#include "pch.h"
#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdoopr.h"
#include "rdorss.h"
#include "rdodpt.h"
#include "rdocommon.h"
#include <rdo_runtime.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

RDOParser* parser = NULL;

RDOParser::RDOParser():
	parsing_object( NULL ),
	parsers( NULL ),
	parser_base( NULL ),
	have_kw_Resources( false ),
	have_kw_ResourcesEnd( false ),
	have_kw_Operations( false ),
	have_kw_OperationsEnd( false ),
	lastDPTSearch( NULL ),
	lastDPTSome( NULL ),
//	patternCounter( 1 ),
//	pokazCounter( 1 ),
//	constCounter( 0 ),
	smr( NULL )
{
	parser  = this;
	runtime = new rdoRuntime::RDORuntime();
	parsers = new RDOParserList( this );
	parsers->reset();
	runtime->memory_insert( sizeof(RDOParser) );
	runtime->memory_insert( sizeof(rdoRuntime::RDORuntime) );
}

RDOParser::~RDOParser()
{
	DeleteAllObjects( allNames );
	DeleteAllObjects( allDoubles );
	std::vector< RDODeletable* >::reverse_iterator it = allDeletables.rbegin();
	while ( it != allDeletables.rend() ) {
		delete *it;
		it = allDeletables.rbegin();
	}
	TRACE( "PARSER : allDeletables.size() = %d\n",allDeletables.size() );
	if ( parsers ) {
		delete parsers;
		parsers = NULL;
	}
	parser = NULL;
}

void RDOParser::insertRTPResType( RDORTPResType* value )
{
	parsing_object = value;
	allRTPResType.push_back( value );
}

void RDOParser::insertEnum( RDORTPEnumParamType* value )
{
	allEnums.push_back( value );
}

void RDOParser::insertRSSResource( RDORSSResource* value )
{
	parsing_object = value;
	allRSSResource.push_back( value );
}

void RDOParser::insertPATPattern( RDOPATPattern* value )
{
	parsing_object = value;
	allPATPatterns.push_back( value );
}

void RDOParser::insertOPROperation( RDOOPROperation* value )
{
	allOPROperations.push_back( value );
}

void RDOParser::insertFRMFrame( rdoRuntime::RDOFRMFrame* value )
{
	allFRMFrame.push_back( value );
}

void RDOParser::insertFUNConstant( RDOFUNConstant* value )
{
	parsing_object = value;
	allFUNConstant.push_back( value );
}

void RDOParser::insertFUNFunction( RDOFUNFunction* value )
{
	parsing_object = value;
	allFUNFunctions.push_back( value );
}

void RDOParser::insertFUNSequences( RDOFUNSequence* value )
{
	parsing_object = value;
	allFUNSequences.push_back( value );
}

void RDOParser::insertFUNGroup( RDOFUNGroup* value )
{
	parsing_object = value;
	allFUNGroupStack.push_back( value );
}

void RDOParser::insertDPTSearch( RDODPTSearch* value )
{
	parsing_object = value;
	allDPTSearch.push_back( value ); 
	lastDPTSearch = value;
	lastDPTSome   = NULL;
}

void RDOParser::insertDPTSome( RDODPTSome* value )
{
	parsing_object = value;
	allDPTSome.push_back( value );
	lastDPTSearch = NULL;
	lastDPTSome   = value;
}

void RDOParser::insertDPTFreeActivity( RDODPTFreeActivity* value )
{
	parsing_object = value;
	allDPTFreeActivity.push_back( value );
	lastDPTSearch = NULL;
	lastDPTSome   = NULL;
}

void RDOParser::insertPMDPokaz( RDOPMDPokaz* value )
{
//	parsing_object = value;
	allPMDPokaz.push_back( value );
}

void RDOParser::insertDPTProcess( RDOPROCProcess* value )
{
	parsing_object = value;
	allDPTProcess.push_back( value );
}

std::stringstream& RDOParser::getModelStructure()
{
	modelStructure.str("");
	modelStructure.clear();

	if ( modelStructure.str().empty() ) {

		// RTP
		modelStructure << std::endl << "$Resource_type" << std::endl;
		std::for_each( allRTPResType.begin(), allRTPResType.end(), std::mem_fun(&RDORTPResType::writeModelStructure) );

		// RSS
		modelStructure << std::endl << "$Resources" << std::endl;
		std::for_each( allRSSResource.begin(), allRSSResource.end(), std::mem_fun(&RDORSSResource::writeModelStructure) );

		// PAT
		modelStructure << std::endl << "$Pattern" << std::endl;
		std::for_each( allPATPatterns.begin(), allPATPatterns.end(), std::mem_fun(&RDOPATPattern::writeModelStructure) );

		// OPR/DPT
		int counter = 1;
		modelStructure << std::endl << "$Activities" << std::endl;
		modelStructure << runtime->writeActivitiesStructure( counter );

		// DPT only
		for( int i = 0; i < allDPTSearch.size(); i++ ) {
			for( int j = 0; j < allDPTSearch.at(i)->getActivities().size(); j++ ) {
				RDODPTSearchActivity* curr = allDPTSearch.at(i)->getActivities().at(j);
				modelStructure << counter++ << " " << curr->getName() << " " << curr->getType()->getPatternId() << std::endl;
			}
		}

		// PDM
		modelStructure << std::endl << "$Watching" << std::endl;
		modelStructure << runtime->writePokazStructure();
	}

	return modelStructure;
}

void RDOParser::parse( int files )
{
//	resourceTypeCounter = 1;
//	resourceCounter = 0;

	int min1, max1, min2, max2;
	RDOParserList::getParserMinMax( rdoModelObjects::obPRE , min1, max1 );
	RDOParserList::getParserMinMax( rdoModelObjects::obPOST, min2, max2 );

	if ( files & rdoModelObjects::obPRE ) {
		parse( rdoModelObjects::obPRE );
	}

	std::list< rdoModelObjects::RDOFileType > file_list = RDOParserList::getParserFiles( files );

	std::map< int, RDOParserBase* >::const_iterator it = parsers->begin();
	while ( it != parsers->end() ) {
		if ( it->first > max1 && it->first < min2 && std::find( file_list.begin(), file_list.end(), it->second->type ) != file_list.end() ) {
			parser_base = it->second;
			it->second->parse();
			parser_base = NULL;
		}
		it++;
	}

	if ( files & rdoModelObjects::obPOST ) {
		parse( rdoModelObjects::obPOST );
	}
}

void RDOParser::parse( rdoModelObjects::RDOParseType file )
{
	int min, max;
	RDOParserList::getParserMinMax( file, min, max );
	if ( min == -1 || max == -1 ) return;
	std::map< int, RDOParserBase* >::const_iterator it = parsers->find( min );
	while ( it != parsers->end() ) {
		if ( it->first <= max ) {
			parser_base = it->second;
			it->second->parse();
			parser_base = NULL;
		} else {
			break;
		}
		it++;
	}
}

void RDOParser::parse( rdoModelObjects::RDOParseType file, std::istream& stream )
{
	int min, max;
	RDOParserList::getParserMinMax( file, min, max );
	if ( min == -1 || max == -1 ) return;
	std::map< int, RDOParserBase* >::const_iterator it = parsers->find( min );
	while ( it != parsers->end() ) {
		if ( it->first <= max ) {
			parser_base = it->second;
			it->second->parse( stream );
			parser_base = NULL;
		} else {
			break;
		}
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
	throw rdoParse::RDOSyntaxException( errors.back().message );
}

void RDOParser::error_push_only( const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	errors.push_back( rdoSimulator::RDOSyntaxError( _error_code, _message, lexer_loc_line(), lexer_loc_pos(), getFileToParse() ) );
}

void RDOParser::error( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	error_push_only( _src_info, _message, _error_code );
	throw rdoParse::RDOSyntaxException( errors.back().message );
}

void RDOParser::error( const RDOParserSrcInfo& _src_info1, const RDOParserSrcInfo& _src_info2, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	error_push_only( _src_info1.src_pos().last_line != _src_info2.src_pos().last_line ? _src_info1 : _src_info2, _message, _error_code );
	throw rdoParse::RDOSyntaxException( errors.back().message );
}

void RDOParser::error_push_only( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code )
{
	errors.push_back( rdoSimulator::RDOSyntaxError( _error_code, _message, _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_filetype() ) );
}

void RDOParser::error_push_done()
{
	if ( !errors.empty() ) {
		throw rdoParse::RDOSyntaxException( errors.back().message );
	}
}

void RDOParser::error_modify( const std::string& _message )
{
	if ( !errors.empty() ) {
		errors.back().message = _message;
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
	errors.push_back( rdoSimulator::RDOSyntaxError( _error_code, _message, lexer_loc_line(), lexer_loc_pos(), getFileToParse(), true ) );
}

void RDOParser::warning( const RDOParserSrcInfo& _src_info, const std::string& _message, rdoSimulator::RDOSyntaxError::ErrorCode _error_code ) 
{
	errors.push_back( rdoSimulator::RDOSyntaxError( _error_code, _message, _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_filetype(), true ) );
}

const RDOFUNConstant* RDOParser::findFUNConst( const std::string& name ) const
{
	std::vector<RDOFUNConstant *>::const_iterator it = 
		std::find_if(allFUNConstant.begin(), allFUNConstant.end(), compareName2<RDOFUNConstant>(name));

	if(it == allFUNConstant.end())
		return NULL;

	return (*it);
}

void RDOParser::checkFunctionName( const RDOParserSrcInfo& _src_info )
{
	const RDOFUNConstant* _const = findFUNConst( _src_info.src_text() );
	if ( _const ) {
		error_push_only( _src_info, rdo::format("Константа '%s' уже существует", _src_info.src_text().c_str()) );
//		parser->error("Second appearance of the same constant name: " + *(_cons->getName()));
		error_push_only( _const->src_info(), "См. первое определение" );
		error_push_done();
	}
	const RDOFUNSequence* _seq = findSequence( _src_info.src_text() );
	if ( _seq ) {
		error_push_only( _src_info, rdo::format( "Последовательность '%s' уже существует", _src_info.src_text().c_str() ) );
		error_push_only( _seq->src_info(), "См. первое определение" );
		error_push_done();
	}
	const RDOFUNFunction* _fun = findFunction( _src_info.src_text() );
	if ( _fun ) {
		error_push_only( _src_info, rdo::format( "Функция '%s' уже существует", _src_info.src_text().c_str() ) );
		error_push_only( _fun->src_info(), "См. первое определение" );
		error_push_done();
	}
}

void RDOParser::checkActivityName( const RDOParserSrcInfo& _src_info )
{
	std::vector< RDODPTSearch* >::const_iterator it_search = getDPTSearch().begin();
	while ( it_search != getDPTSearch().end() ) {
		std::vector< RDODPTSearchActivity* >::const_iterator it_search_act = std::find_if( (*it_search)->getActivities().begin(), (*it_search)->getActivities().end(), compareName2<RDODPTSearchActivity>(_src_info.src_text()) );
		if ( it_search_act != (*it_search)->getActivities().end() ) {
			error_push_only( _src_info, rdo::format("Активность '%s' уже существует", _src_info.src_text().c_str()) );
			error_push_only( (*it_search_act)->src_info(), "См. первое определение" );
			error_push_done();
//			error("Activity name: " + *_name + " already defined");
		}
		it_search++;
	}
	std::vector< RDODPTSome* >::const_iterator it_some = getDPTSome().begin();
	while ( it_some != getDPTSome().end() ) {
		std::vector< RDODPTSomeActivity* >::const_iterator it_some_act = std::find_if( (*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName2<RDODPTSomeActivity>(_src_info.src_text()) );
		if ( it_some_act != (*it_some)->getActivities().end() ) {
			error_push_only( _src_info, rdo::format("Активность '%s' уже существует", _src_info.src_text().c_str()) );
			error_push_only( (*it_some_act)->src_info(), "См. первое определение" );
			error_push_done();
		}
		it_some++;
	}
	const RDODPTFreeActivity* free_act = findFreeActivity( _src_info.src_text() );
	if ( free_act ) {
		error_push_only( _src_info, rdo::format("Активность '%s' уже существует", _src_info.src_text().c_str()) );
		error_push_only( free_act->src_info(), "См. первое определение" );
		error_push_done();
//		error("Free activity name: " + *_name + " already defined");
	}
	const RDOOPROperation* opr = findOperation( _src_info.src_text() );
	if ( opr ) {
		error_push_only( _src_info, rdo::format("Операция '%s' уже существует", _src_info.src_text().c_str()) );
		error_push_only( opr->src_info(), "См. первое определение" );
		error_push_done();
	}
}

void RDOParser::checkDPTName( const RDOParserSrcInfo& _src_info )
{
	std::vector< RDODPTSearch* >::const_iterator search_it = std::find_if(getDPTSearch().begin(), getDPTSearch().end(), compareName2<RDODPTSearch>(_src_info.src_text()));
	if ( search_it != getDPTSearch().end() ) {
		error_push_only( _src_info, rdo::format("Точка '%s' уже существует", _src_info.src_text().c_str()) );
		error_push_only( (*search_it)->src_info(), "См. первое определение" );
		error_push_done();
//		error( _src_info, "DPT name: " + _src_info.src_text() + " already defined" );
	}
	std::vector< RDODPTSome* >::const_iterator some_it = std::find_if(getDPTSome().begin(), getDPTSome().end(), compareName2<RDODPTSome>(_src_info.src_text()));
	if ( some_it != getDPTSome().end() ) {
		error_push_only( _src_info, rdo::format("Точка '%s' уже существует", _src_info.src_text().c_str()) );
		error_push_only( (*some_it)->src_info(), "См. первое определение" );
		error_push_done();
	}
}

const RDORTPResType* RDOParser::findRTPResType( const std::string& name ) const
{
	std::vector< RDORTPResType* >::const_iterator it = std::find_if( allRTPResType.begin(), allRTPResType.end(), compareName2<RDORTPResType>(name) );
	return it != allRTPResType.end() ? *it : NULL;
}

const RDORSSResource* RDOParser::findRSSResource( const std::string& name ) const
{
	std::vector< RDORSSResource* >::const_iterator it = std::find_if( allRSSResource.begin(), allRSSResource.end(), compareName2<RDORSSResource>(name) );
	if ( it != allRSSResource.end() ) {
		return *it;
	}
	return NULL;
}

const RDOFUNFunction* RDOParser::findFunction( const std::string& name ) const
{
	std::vector< RDOFUNFunction* >::const_iterator it = std::find_if(allFUNFunctions.begin(), 
		allFUNFunctions.end(), 
		compareName2<RDOFUNFunction>(name));
	if(it != allFUNFunctions.end())
		return (*it);

	return NULL;
}

const RDOFUNSequence* RDOParser::findSequence( const std::string& name ) const
{
	std::vector< RDOFUNSequence* >::const_iterator it = std::find_if(allFUNSequences.begin(), 
		allFUNSequences.end(), 
		compareName2<RDOFUNSequence>(name));
	if(it != allFUNSequences.end())
		return (*it);

	return NULL;
}

const RDOPATPattern* RDOParser::findPattern( const std::string& name ) const
{
	std::vector<RDOPATPattern *>::const_iterator it = std::find_if(allPATPatterns.begin(), 
		allPATPatterns.end(), 
		compareName2<RDOPATPattern>(name));
	if(it != allPATPatterns.end())
		return (*it);

	return NULL;
}

const RDOOPROperation* RDOParser::findOperation( const std::string& name ) const
{
	std::vector< RDOOPROperation* >::const_iterator it = std::find_if( allOPROperations.begin(), allOPROperations.end(), compareName2<RDOOPROperation>(name) );
	return it != allOPROperations.end() ? *it : NULL;
}

const RDODPTFreeActivity* RDOParser::findFreeActivity( const std::string& name ) const
{
	std::vector< RDODPTFreeActivity* >::const_iterator it = std::find_if( allDPTFreeActivity.begin(), allDPTFreeActivity.end(), compareName2<RDODPTFreeActivity>(name) );
	return it != allDPTFreeActivity.end() ? *it : NULL;
}

void RDOParser::LoadStdFunctions()
{
	RDORTPIntParamType* intType   = new RDORTPIntParamType( this, new RDORTPIntDiap(this), new RDORTPIntDefVal(this) );
	RDORTPRealParamType* realType = new RDORTPRealParamType( this, new RDORTPRealDiap(this), new RDORTPRealDefVal(this) );

	RDOFUNFunction* fun = new RDOFUNFunction( this, "Abs", realType );
	RDOFUNFunctionParam* param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcAbs( runtime );

	fun = new RDOFUNFunction( this, "ArcCos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcCos( runtime );

	fun = new RDOFUNFunction( this, "ArcSin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcSin( runtime );

	fun = new RDOFUNFunction( this, "ArcTan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcTan( runtime );

	fun = new RDOFUNFunction( this, "Cos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcCos( runtime );

	fun = new RDOFUNFunction( this, "Cotan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcCotan( runtime );

	fun = new RDOFUNFunction( this, "Exp", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcExp( runtime );

	fun = new RDOFUNFunction( this, "Floor", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcFloor( runtime );

	fun = new RDOFUNFunction( this, "Frac", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcFrac( runtime );

	fun = new RDOFUNFunction( this, "IAbs", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcIAbs( runtime );

	fun = new RDOFUNFunction( this, "IMax", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMax( runtime );

	fun = new RDOFUNFunction( this, "IMin", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMin( runtime );

	fun = new RDOFUNFunction( this, "Int", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcInt( runtime );

	fun = new RDOFUNFunction( this, "IntPower", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcIntPower( runtime );

	fun = new RDOFUNFunction( this, "Ln", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcLn( runtime );

	fun = new RDOFUNFunction( this, "Log10", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog10( runtime );

	fun = new RDOFUNFunction( this, "Log2", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog2( runtime );

	fun = new RDOFUNFunction( this, "LogN", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcLogN( runtime );

	fun = new RDOFUNFunction( this, "Max", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcMax( runtime );

	fun = new RDOFUNFunction( this, "Min", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcMin( runtime );

	fun = new RDOFUNFunction( this, "Power", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcPower( runtime );

	fun = new RDOFUNFunction( this, "Round", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcRound( runtime );

	fun = new RDOFUNFunction( this, "Sin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcSin( runtime );

	fun = new RDOFUNFunction( this, "Sqrt", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcSqrt( runtime );

	fun = new RDOFUNFunction( this, "Tan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcTan( runtime );

	// -----------------------------------------
	// И для маленьких букв
	// -----------------------------------------
	fun = new RDOFUNFunction( this, "abs", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcAbs( runtime );

	fun = new RDOFUNFunction( this, "arccos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcCos( runtime );

	fun = new RDOFUNFunction( this, "arcsin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcSin( runtime );

	fun = new RDOFUNFunction( this, "arctan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcTan( runtime );

	fun = new RDOFUNFunction( this, "cos", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcCos( runtime );

	fun = new RDOFUNFunction( this, "cotan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcCotan( runtime );

	fun = new RDOFUNFunction( this, "exp", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcExp( runtime );

	fun = new RDOFUNFunction( this, "floor", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcFloor( runtime );

	fun = new RDOFUNFunction( this, "frac", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcFrac( runtime );

	fun = new RDOFUNFunction( this, "iabs", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcIAbs( runtime );

	fun = new RDOFUNFunction( this, "imax", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMax( runtime );

	fun = new RDOFUNFunction( this, "imin", intType);
	param = new RDOFUNFunctionParam( fun, "p1", intType);
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMin( runtime );

	fun = new RDOFUNFunction( this, "int", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcInt( runtime );

	fun = new RDOFUNFunction( this, "intpower", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", intType);
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcIntPower( runtime );

	fun = new RDOFUNFunction( this, "ln", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcLn( runtime );

	fun = new RDOFUNFunction( this, "log10", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog10( runtime );

	fun = new RDOFUNFunction( this, "log2", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog2( runtime );

	fun = new RDOFUNFunction( this, "logn", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcLogN( runtime );

	fun = new RDOFUNFunction( this, "max", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcMax( runtime );

	fun = new RDOFUNFunction( this, "min", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcMin( runtime );

	fun = new RDOFUNFunction( this, "power", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	param = new RDOFUNFunctionParam( fun, "p2", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcPower( runtime );

	fun = new RDOFUNFunction( this, "round", intType);
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcRound( runtime );

	fun = new RDOFUNFunction( this, "sin", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcSin( runtime );

	fun = new RDOFUNFunction( this, "sqrt", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcSqrt( runtime );

	fun = new RDOFUNFunction( this, "tan", realType );
	param = new RDOFUNFunctionParam( fun, "p1", realType );
	fun->add( param );
	fun->functionCalc = new rdoRuntime::RDOFunCalcTan( runtime );
}

} // namespace rdoParse 

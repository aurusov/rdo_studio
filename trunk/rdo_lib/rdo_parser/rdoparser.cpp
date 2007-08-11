#include "pch.h"
#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"
#include "rdofun.h"
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
	lastDPTSearch( NULL ),
//	patternCounter( 1 ),
//	pokazCounter( 1 ),
//	constCounter( 0 ),
	smr( NULL )
{
	parser  = this;
	runTime = new rdoRuntime::RDORuntime();
	parsers = new RDOParserList( this );
	parsers->reset();
	runTime->memory_insert( sizeof(RDOParser) );
	runTime->memory_insert( sizeof(rdoRuntime::RDORuntime) );
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

void RDOParser::insertRTPParam( RDORTPParam* value )
{
//	parsing_object = value;
	allRTPParams.push_back( value );
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
}

void RDOParser::insertDPTSome( RDODPTSome* value )
{
	parsing_object = value;
	allDPTSome.push_back( value );
	lastDPTSearch = NULL;
}

void RDOParser::insertDPTFreeActivity( RDODPTFreeActivity* value )
{
	parsing_object = value;
	allDPTFreeActivity.push_back( value );
	lastDPTSearch = NULL;
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
		modelStructure << runTime->writeActivitiesStructure( counter );

		// DPT only
		for( int i = 0; i < allDPTSearch.size(); i++ ) {
			for( int j = 0; j < allDPTSearch.at(i)->getActivities().size(); j++ ) {
				RDODPTSearchActivity* curr = allDPTSearch.at(i)->getActivities().at(j);
				modelStructure << counter++ << " " << *curr->getName() << " " << curr->getRule()->getPatternId() << std::endl;
			}
		}

		// PDM
		modelStructure << std::endl << "$Watching" << std::endl;
		modelStructure << runTime->writePokazStructure();
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

void RDOParser::error( rdoSimulator::RDOSyntaxError::ErrorCode error_code, ... )
{
	va_list params;
	va_start( params, error_code );
	std::string str = rdoSimulator::RDOSyntaxError::getMessage( error_code, params );
	va_end( params );
	error( str, error_code );
}

void RDOParser::error( const std::string& message, rdoSimulator::RDOSyntaxError::ErrorCode error_code ) 
{
	errors.push_back( rdoSimulator::RDOSyntaxError( error_code, message, lexer_loc_line(), lexer_loc_pos(), getFileToParse() ) );
	throw rdoParse::RDOSyntaxException( errors.back().message );
}

void RDOParser::error_modify( const std::string& message )
{
	if ( !errors.empty() ) {
		errors.back().message = message;
		throw rdoParse::RDOSyntaxException( "" );
	}
}

void RDOParser::warning( rdoSimulator::RDOSyntaxError::ErrorCode error_code, ... )
{
	va_list params;
	va_start( params, error_code );
	std::string str = rdoSimulator::RDOSyntaxError::getMessage( error_code, params );
	va_end( params );
	warning( str, error_code );
}

void RDOParser::warning( const std::string& message, rdoSimulator::RDOSyntaxError::ErrorCode error_code ) 
{
	errors.push_back( rdoSimulator::RDOSyntaxError( error_code, message, lexer_loc_line(), lexer_loc_pos(), getFileToParse(), true ) );
}

void RDOParser::addConstant( RDORTPParam* const _cons )
{
	if ( !findFUNConst(_cons->getName()) ) {
		RDOFUNConstant* newConst = new RDOFUNConstant( this, _cons );
		runTime->setConstValue( newConst->number, newConst->getType()->getRSSDefaultValue() );
	}
}

const RDOFUNConstant* RDOParser::findFUNConst( const std::string& _cons ) const
{
	std::vector<RDOFUNConstant *>::const_iterator it = 
		std::find_if(allFUNConstant.begin(), allFUNConstant.end(), compareName2<RDOFUNConstant>(_cons));

	if(it == allFUNConstant.end())
		return NULL;

	return (*it);
}

void RDOParser::checkActivityName( const std::string* _name )
{
	std::vector< RDODPTSearch* >::const_iterator it_search = getDPTSearch().begin();
	while ( it_search != getDPTSearch().end() ) {
		if ( std::find_if( (*it_search)->getActivities().begin(), (*it_search)->getActivities().end(), compareName<RDODPTSearchActivity>(_name) ) != (*it_search)->getActivities().end() ) {
			error( rdo::format("јктивность с таким именем уже существует в точке типа search: %s", (*it_search)->getName()->c_str()) );
//			error("Activity name: " + *_name + " already defined");
		}
		it_search++;
	}
	std::vector< RDODPTSome* >::const_iterator it_some = getDPTSome().begin();
	while ( it_some != getDPTSome().end() ) {
		if ( std::find_if( (*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName<RDODPTSomeActivity>(_name) ) != (*it_some)->getActivities().end() ) {
			error( rdo::format("јктивность с таким именем уже существует в точке типа some: %s", (*it_some)->getName()->c_str()) );
		}
		it_some++;
	}
	if ( std::find_if( getDPTFreeActivity().begin(), getDPTFreeActivity().end(), compareName<RDODPTFreeActivity>(_name) ) != getDPTFreeActivity().end() ) {
		error( rdo::format("јктивность с таким именем уже существует: %s", _name->c_str()) );
//		error("Free activity name: " + *_name + " already defined");
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

void RDOParser::LoadStdFunctions()
{
	RDORTPIntParamType *intType = new RDORTPIntParamType( this, new RDORTPIntDiap(), new RDORTPIntDefVal() );
	RDORTPRealParamType *realType = new RDORTPRealParamType( this, new RDORTPRealDiap(), new RDORTPRealDefVal() );

	RDOFUNFunction *fun = new RDOFUNFunction( this, "Abs", realType );
	allFUNFunctions.push_back(fun);
	RDOFUNFunctionParam *param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcAbs( runTime );

	fun = new RDOFUNFunction( this, "ArcCos", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcCos( runTime );

	fun = new RDOFUNFunction( this, "ArcSin", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcSin( runTime );

	fun = new RDOFUNFunction( this, "ArcTan", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcTan( runTime );

	fun = new RDOFUNFunction( this, "Cos", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCos( runTime );

	fun = new RDOFUNFunction( this, "Cotan", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCotan( runTime );

	fun = new RDOFUNFunction( this, "Exp", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcExp( runTime );

	fun = new RDOFUNFunction( this, "Floor", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFloor( runTime );

	fun = new RDOFUNFunction( this, "Frac", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFrac( runTime );

	fun = new RDOFUNFunction( this, "IAbs", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIAbs( runTime );

	fun = new RDOFUNFunction( this, "IMax", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", intType);
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMax( runTime );

	fun = new RDOFUNFunction( this, "IMin", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", intType);
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMin( runTime );

	fun = new RDOFUNFunction( this, "Int", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcInt( runTime );

	fun = new RDOFUNFunction( this, "IntPower", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIntPower( runTime );

	fun = new RDOFUNFunction( this, "Ln", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLn( runTime );

	fun = new RDOFUNFunction( this, "Log10", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog10( runTime );

	fun = new RDOFUNFunction( this, "Log2", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog2( runTime );

	fun = new RDOFUNFunction( this, "LogN", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLogN( runTime );

	fun = new RDOFUNFunction( this, "Max", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMax( runTime );

	fun = new RDOFUNFunction( this, "Min", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMin( runTime );

	fun = new RDOFUNFunction( this, "Power", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcPower( runTime );

	fun = new RDOFUNFunction( this, "Round", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcRound( runTime );

	fun = new RDOFUNFunction( this, "Sin", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSin( runTime );

	fun = new RDOFUNFunction( this, "Sqrt", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSqrt( runTime );

	fun = new RDOFUNFunction( this, "Tan", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcTan( runTime );

	// -----------------------------------------
	// » дл€ маленьких букв
	// -----------------------------------------
	fun = new RDOFUNFunction( this, "abs", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcAbs( runTime );

	fun = new RDOFUNFunction( this, "arccos", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcCos( runTime );

	fun = new RDOFUNFunction( this, "arcsin", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcSin( runTime );

	fun = new RDOFUNFunction( this, "arctan", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcTan( runTime );

	fun = new RDOFUNFunction( this, "cos", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCos( runTime );

	fun = new RDOFUNFunction( this, "cotan", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCotan( runTime );

	fun = new RDOFUNFunction( this, "exp", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcExp( runTime );

	fun = new RDOFUNFunction( this, "floor", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFloor( runTime );

	fun = new RDOFUNFunction( this, "frac", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFrac( runTime );

	fun = new RDOFUNFunction( this, "iabs", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIAbs( runTime );

	fun = new RDOFUNFunction( this, "imax", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", intType);
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMax( runTime );

	fun = new RDOFUNFunction( this, "imin", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", intType);
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMin( runTime );

	fun = new RDOFUNFunction( this, "int", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcInt( runTime );

	fun = new RDOFUNFunction( this, "intpower", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIntPower( runTime );

	fun = new RDOFUNFunction( this, "ln", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLn( runTime );

	fun = new RDOFUNFunction( this, "log10", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog10( runTime );

	fun = new RDOFUNFunction( this, "log2", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog2( runTime );

	fun = new RDOFUNFunction( this, "logn", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLogN( runTime );

	fun = new RDOFUNFunction( this, "max", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMax( runTime );

	fun = new RDOFUNFunction( this, "min", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMin( runTime );

	fun = new RDOFUNFunction( this, "power", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( "p2", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcPower( runTime );

	fun = new RDOFUNFunction( this, "round", intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcRound( runTime );

	fun = new RDOFUNFunction( this, "sin", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSin( runTime );

	fun = new RDOFUNFunction( this, "sqrt", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSqrt( runTime );

	fun = new RDOFUNFunction( this, "tan", realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( "p1", realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcTan( runTime );
}

} // namespace rdoParse 

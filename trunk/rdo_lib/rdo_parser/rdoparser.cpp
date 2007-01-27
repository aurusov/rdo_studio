#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser.h"
#include "rdoruntime.h"
#include "rdoparser_rdo.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdodpt.h"
#include "rdocommon.h"

namespace rdoParse 
{

RDOParser* parser = NULL;

RDOParser::RDOParser():
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
	DeleteAllObjects( allDeletables );
	if ( parsers ) {
		delete parsers;
		parsers = NULL;
	}
	parser = NULL;
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
	throw rdoParse::RDOSyntaxException( "" );
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

const RDORTPResType* RDOParser::findRTPResType( const std::string* const type ) const
{
	std::vector< RDORTPResType* >::const_iterator it = std::find_if( allRTPResType.begin(), allRTPResType.end(), compareName<RDORTPResType>(type) );
	return it != allRTPResType.end() ? *it : NULL;
}

const RDORTPResType* RDOParser::findRTPResType( const std::string& name ) const
{
	std::vector< RDORTPResType* >::const_iterator it = std::find_if( allRTPResType.begin(), allRTPResType.end(), compareName<RDORTPResType>(&name) );
	return it != allRTPResType.end() ? *it : NULL;
}

const RDORSSResource* RDOParser::findRSSResource(const std::string *const name) const
{
	std::vector<RDORSSResource *>::const_iterator it = std::find_if(allRSSResource.begin(), 
		allRSSResource.end(), 
		compareName<RDORSSResource>(name));
	if(it != allRSSResource.end())
		return (*it);

	return NULL;
}

const RDOFUNFunction *RDOParser::findFunction(const std::string *const name) const
{
	std::vector<RDOFUNFunction *>::const_iterator it = std::find_if(allFUNFunctions.begin(), 
		allFUNFunctions.end(), 
		compareName<RDOFUNFunction>(name));
	if(it != allFUNFunctions.end())
		return (*it);

	return NULL;
}

const RDOFUNSequence *RDOParser::findSequence(const std::string *const name) const
{
	std::vector<RDOFUNSequence *>::const_iterator it = std::find_if(allFUNSequences.begin(), 
		allFUNSequences.end(), 
		compareName<RDOFUNSequence>(name));
	if(it != allFUNSequences.end())
		return (*it);

	return NULL;
}

const RDOPATPattern *RDOParser::findPattern(const std::string *const name) const
{
	std::vector<RDOPATPattern *>::const_iterator it = std::find_if(allPATPatterns.begin(), 
		allPATPatterns.end(), 
		compareName<RDOPATPattern>(name));
	if(it != allPATPatterns.end())
		return (*it);

	return NULL;
}

void addCalcToRuntime( rdoRuntime::RDOCalc* calc )
{
	parser->runTime->allCalcs.push_back( calc ); 
}

void removeCalcToRuntime( rdoRuntime::RDOCalc* calc )
{
	std::list< rdoRuntime::RDOCalc* >& list = parser->runTime->allCalcs;
	if ( std::find( list.begin(), list.end(), calc ) != list.end() ) {
		list.remove( calc );
	}
}

void RDOParser::LoadStdFunctions()
{
	RDORTPIntResParam *intType = new RDORTPIntResParam(new RDORTPIntDiap(), new RDORTPIntDefVal(false));
	RDORTPRealResParam *realType = new RDORTPRealResParam(new RDORTPRealDiap(), new RDORTPRealDefVal(false));

	RDOFUNFunction *fun = new RDOFUNFunction( this, registerName("Abs"), realType );
	allFUNFunctions.push_back(fun);
	RDOFUNFunctionParam *param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcAbs();

	fun = new RDOFUNFunction( this, registerName("ArcCos"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcCos();

	fun = new RDOFUNFunction( this, registerName("ArcSin"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcSin();

	fun = new RDOFUNFunction( this, registerName("ArcTan"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcTan();

	fun = new RDOFUNFunction( this, registerName("Cos"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCos();

	fun = new RDOFUNFunction( this, registerName("Cotan"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCotan();

	fun = new RDOFUNFunction( this, registerName("Exp"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcExp();

	fun = new RDOFUNFunction( this, registerName("Floor"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFloor();

	fun = new RDOFUNFunction( this, registerName("Frac"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFrac();

	fun = new RDOFUNFunction( this, registerName("IAbs"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIAbs();

	fun = new RDOFUNFunction( this, registerName("IMax"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMax();

	fun = new RDOFUNFunction( this, registerName("IMin"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMin();

	fun = new RDOFUNFunction( this, registerName("Int"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcInt();

	fun = new RDOFUNFunction( this, registerName("IntPower"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIntPower();

	fun = new RDOFUNFunction( this, registerName("Ln"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLn();

	fun = new RDOFUNFunction( this, registerName("Log10"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog10();

	fun = new RDOFUNFunction( this, registerName("Log2"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog2();

	fun = new RDOFUNFunction( this, registerName("LogN"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLogN();

	fun = new RDOFUNFunction( this, registerName("Max"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMax();

	fun = new RDOFUNFunction( this, registerName("Min"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMin();

	fun = new RDOFUNFunction( this, registerName("Power"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcPower();

	fun = new RDOFUNFunction( this, registerName("Round"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcRound();

	fun = new RDOFUNFunction( this, registerName("Sin"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSin();

	fun = new RDOFUNFunction( this, registerName("Sqrt"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSqrt();

	fun = new RDOFUNFunction( this, registerName("Tan"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcTan();

	// -----------------------------------------
	// » дл€ маленьких букв
	// -----------------------------------------
	fun = new RDOFUNFunction( this, registerName("abs"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcAbs();

	fun = new RDOFUNFunction( this, registerName("arccos"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcCos();

	fun = new RDOFUNFunction( this, registerName("arcsin"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcSin();

	fun = new RDOFUNFunction( this, registerName("arctan"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcTan();

	fun = new RDOFUNFunction( this, registerName("cos"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCos();

	fun = new RDOFUNFunction( this, registerName("cotan"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCotan();

	fun = new RDOFUNFunction( this, registerName("exp"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcExp();

	fun = new RDOFUNFunction( this, registerName("floor"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFloor();

	fun = new RDOFUNFunction( this, registerName("frac"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFrac();

	fun = new RDOFUNFunction( this, registerName("iabs"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIAbs();

	fun = new RDOFUNFunction( this, registerName("imax"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMax();

	fun = new RDOFUNFunction( this, registerName("imin"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMin();

	fun = new RDOFUNFunction( this, registerName("int"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcInt();

	fun = new RDOFUNFunction( this, registerName("intpower"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIntPower();

	fun = new RDOFUNFunction( this, registerName("ln"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLn();

	fun = new RDOFUNFunction( this, registerName("log10"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog10();

	fun = new RDOFUNFunction( this, registerName("log2"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog2();

	fun = new RDOFUNFunction( this, registerName("logn"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLogN();

	fun = new RDOFUNFunction( this, registerName("max"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMax();

	fun = new RDOFUNFunction( this, registerName("min"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMin();

	fun = new RDOFUNFunction( this, registerName("power"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	param = new RDOFUNFunctionParam( registerName("p2"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcPower();

	fun = new RDOFUNFunction( this, registerName("round"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcRound();

	fun = new RDOFUNFunction( this, registerName("sin"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSin();

	fun = new RDOFUNFunction( this, registerName("sqrt"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSqrt();

	fun = new RDOFUNFunction( this, registerName("tan"), realType );
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam( registerName("p1"), realType );
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcTan();
}

RDOParser::RDOHotKeyToolkit::RDOHotKeyToolkit()
{														   
/*
const int RDOESCAPE	= 0x101;
const int RDOTAB		= 0x10F;
const int RDOSHIFT	= 0x12A;		// Left
const int RDOCONTROL	= 0x11D;		// Left or Right
const int RDOBACK		= 0x10E;
const int RDORETURN	= 0x11C;
const int RDOINSERT	= 0x152;
const int RDOHOME		= 0x024;
const int RDOPRIOR	= 0x149;
const int RDODELETE	= 0x153;
const int RDOEND		= 0x14F;
const int RDONEXT		= 0x151;
const int RDOUP		= 0x026;
const int RDOLEFT		= 0x025;
const int RDODOWN		= 0x028;
const int RDORIGHT	= 0x027;
const int RDODIVIDE	= 0x135;
const int RDOMULTIPLY= 0x137;		// definetly Print Screen, if somebody know 'Gray*' scan code, tell me
const int RDOSUBTRACT= 0x06D;
const int RDOADD		= 0x06B;
const int RDOCLEAR	= 0x14C;		// I think so
const int RDOSPACE	= VK_SPACE;
const int RDODECIMAL	= 0x134;		// I hope...
const int RDOF2		= 0x13C;
const int RDOF3		= 0x13D;
const int RDOF4		= 0x13E;
const int RDOF5		= 0x13F;
const int RDOF6		= 0x140;
const int RDOF7		= 0x141;
const int RDOF8		= 0x142;
const int RDOF9		= 0x143;
const int RDOF10		= 0x144;
const int RDOF11		= 0x157;
const int RDOF12		= 0x158;
const int RDONUMPAD0	= 0x152;
const int RDONUMPAD1	= 0x14F;
const int RDONUMPAD2	= 0x150;
const int RDONUMPAD3	= 0x151;
const int RDONUMPAD4	= 0x14B;
const int RDONUMPAD5	= 0x14C;
const int RDONUMPAD6	= 0x14D;
const int RDONUMPAD7	= 0x147;
const int RDONUMPAD8	= 0x148;
const int RDONUMPAD9	= 0x149;
*/
	keys.insert(std::map<std::string, int>::value_type("ESCAPE", VK_ESCAPE));
	keys.insert(std::map<std::string, int>::value_type("TAB", VK_TAB));
	keys.insert(std::map<std::string, int>::value_type("SHIFT", VK_SHIFT));
	keys.insert(std::map<std::string, int>::value_type("CONTROL", VK_CONTROL));
	keys.insert(std::map<std::string, int>::value_type("BACK", VK_BACK));
	keys.insert(std::map<std::string, int>::value_type("RETURN", VK_RETURN));
	keys.insert(std::map<std::string, int>::value_type("INSERT", VK_INSERT));
	keys.insert(std::map<std::string, int>::value_type("HOME", VK_HOME));
	keys.insert(std::map<std::string, int>::value_type("PRIOR", VK_PRIOR));
	keys.insert(std::map<std::string, int>::value_type("DELETE", VK_DELETE));
	keys.insert(std::map<std::string, int>::value_type("END", VK_END));
	keys.insert(std::map<std::string, int>::value_type("NEXT", VK_NEXT));
	keys.insert(std::map<std::string, int>::value_type("UP", VK_UP));
	keys.insert(std::map<std::string, int>::value_type("LEFT", VK_LEFT));
	keys.insert(std::map<std::string, int>::value_type("DOWN", VK_DOWN));
	keys.insert(std::map<std::string, int>::value_type("RIGHT", VK_RIGHT));
	keys.insert(std::map<std::string, int>::value_type("DIVIDE", VK_DIVIDE));
	keys.insert(std::map<std::string, int>::value_type("MULTIPLY", VK_MULTIPLY));
	keys.insert(std::map<std::string, int>::value_type("SUBTRACT", VK_SUBTRACT));
	keys.insert(std::map<std::string, int>::value_type("ADD", VK_ADD));
	keys.insert(std::map<std::string, int>::value_type("CLEAR", VK_CLEAR));
	keys.insert(std::map<std::string, int>::value_type("SPACE", VK_SPACE));
	keys.insert(std::map<std::string, int>::value_type("DECIMAL", VK_DECIMAL));
	keys.insert(std::map<std::string, int>::value_type("F2", VK_F2));
	keys.insert(std::map<std::string, int>::value_type("F3", VK_F3));
	keys.insert(std::map<std::string, int>::value_type("F4", VK_F4));
	keys.insert(std::map<std::string, int>::value_type("F5", VK_F5));
	keys.insert(std::map<std::string, int>::value_type("F6", VK_F6));
	keys.insert(std::map<std::string, int>::value_type("F7", VK_F7));
	keys.insert(std::map<std::string, int>::value_type("F8", VK_F8));
	keys.insert(std::map<std::string, int>::value_type("F9", VK_F9));
	keys.insert(std::map<std::string, int>::value_type("F10", VK_F10));
	keys.insert(std::map<std::string, int>::value_type("F11", VK_F11));
	keys.insert(std::map<std::string, int>::value_type("F12", VK_F12));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD0", VK_NUMPAD0));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD1", VK_NUMPAD1));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD2", VK_NUMPAD2));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD3", VK_NUMPAD3));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD4", VK_NUMPAD4));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD5", VK_NUMPAD5));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD6", VK_NUMPAD6));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD7", VK_NUMPAD7));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD8", VK_NUMPAD8));
	keys.insert(std::map<std::string, int>::value_type("NUMPAD9", VK_NUMPAD9));
	keys.insert(std::map<std::string, int>::value_type("NOKEY", -1));

	for(char i = '0'; i <= '9'; i++)
		keys.insert(std::map<std::string, int>::value_type(std::string(1, i), (int)i));

	for(i = 'A'; i <= 'Z'; i++)
		keys.insert(std::map<std::string, int>::value_type(std::string(1, i), (int)i));
}

int RDOParser::RDOHotKeyToolkit::codeFromString( std::string* key )
{
	std::map< std::string, int >::iterator it = keys.find( *key );
	if ( it == keys.end() ) {
		parser->error( "Unknown key name: " + *key );
	}
	return (*it).second;
}

} // namespace rdoParse 

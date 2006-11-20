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
	parsers.reset();
}

RDOParser::~RDOParser()
{
	DeleteAllObjects( allNames );
	DeleteAllObjects( allDoubles );
	DeleteAllObjects( allDeletables );
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
		modelStructure << std::endl << "$Activities" << std::endl;
		modelStructure << runTime->writeActivitiesStructure();

		// DPT only
		for( int i = 0; i < allDPTSearch.size(); i++ ) {
			int counter = 1;
			for( int j = 0; j < allDPTSearch.at(i)->getActivities().size(); j++ ) {
				RDODPTSearchActivity *curr = allDPTSearch.at(i)->getActivities().at(j);
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

	std::map< int, RDOParserBase* >::const_iterator it = parsers.begin();
	while ( it != parsers.end() ) {
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
	std::map< int, RDOParserBase* >::const_iterator it = parsers.find( min );
	while ( it != parsers.end() ) {
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
	std::map< int, RDOParserBase* >::const_iterator it = parsers.find( min );
	while ( it != parsers.end() ) {
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

	RDOFUNFunction *fun = new RDOFUNFunction(registerName("Abs"), realType);
	allFUNFunctions.push_back(fun);
	RDOFUNFunctionParam *param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcAbs();

	fun = new RDOFUNFunction(registerName("ArcCos"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcCos();

	fun = new RDOFUNFunction(registerName("ArcSin"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcSin();

	fun = new RDOFUNFunction(registerName("ArcTan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcTan();

	fun = new RDOFUNFunction(registerName("Cos"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCos();

	fun = new RDOFUNFunction(registerName("Cotan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCotan();

	fun = new RDOFUNFunction(registerName("Exp"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcExp();

	fun = new RDOFUNFunction(registerName("Floor"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFloor();

	fun = new RDOFUNFunction(registerName("Frac"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFrac();

	fun = new RDOFUNFunction(registerName("IAbs"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIAbs();

	fun = new RDOFUNFunction(registerName("IMax"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMax();

	fun = new RDOFUNFunction(registerName("IMin"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMin();

	fun = new RDOFUNFunction(registerName("Int"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcInt();

	fun = new RDOFUNFunction(registerName("IntPower"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIntPower();

	fun = new RDOFUNFunction(registerName("Ln"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLn();

	fun = new RDOFUNFunction(registerName("Log10"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog10();

	fun = new RDOFUNFunction(registerName("Log2"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog2();

	fun = new RDOFUNFunction(registerName("LogN"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLogN();

	fun = new RDOFUNFunction(registerName("Max"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMax();

	fun = new RDOFUNFunction(registerName("Min"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMin();

	fun = new RDOFUNFunction(registerName("Power"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcPower();

	fun = new RDOFUNFunction(registerName("Round"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcRound();

	fun = new RDOFUNFunction(registerName("Sin"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSin();

	fun = new RDOFUNFunction(registerName("Sqrt"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSqrt();

	fun = new RDOFUNFunction(registerName("Tan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcTan();

	// -----------------------------------------
	// � ��� ��������� ����
	// -----------------------------------------
	fun = new RDOFUNFunction(registerName("abs"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcAbs();

	fun = new RDOFUNFunction(registerName("arccos"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcCos();

	fun = new RDOFUNFunction(registerName("arcsin"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcSin();

	fun = new RDOFUNFunction(registerName("arctan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcArcTan();

	fun = new RDOFUNFunction(registerName("cos"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCos();

	fun = new RDOFUNFunction(registerName("cotan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcCotan();

	fun = new RDOFUNFunction(registerName("exp"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcExp();

	fun = new RDOFUNFunction(registerName("floor"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFloor();

	fun = new RDOFUNFunction(registerName("frac"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcFrac();

	fun = new RDOFUNFunction(registerName("iabs"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIAbs();

	fun = new RDOFUNFunction(registerName("imax"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMax();

	fun = new RDOFUNFunction(registerName("imin"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIMin();

	fun = new RDOFUNFunction(registerName("int"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcInt();

	fun = new RDOFUNFunction(registerName("intpower"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcIntPower();

	fun = new RDOFUNFunction(registerName("ln"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLn();

	fun = new RDOFUNFunction(registerName("log10"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog10();

	fun = new RDOFUNFunction(registerName("log2"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLog2();

	fun = new RDOFUNFunction(registerName("logn"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcLogN();

	fun = new RDOFUNFunction(registerName("max"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMax();

	fun = new RDOFUNFunction(registerName("min"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcMin();

	fun = new RDOFUNFunction(registerName("power"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcPower();

	fun = new RDOFUNFunction(registerName("round"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcRound();

	fun = new RDOFUNFunction(registerName("sin"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSin();

	fun = new RDOFUNFunction(registerName("sqrt"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcSqrt();

	fun = new RDOFUNFunction(registerName("tan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new rdoRuntime::RDOFunCalcTan();
}

} // namespace rdoParse 

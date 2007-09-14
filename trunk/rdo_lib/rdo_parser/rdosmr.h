#ifndef RDOSMR_SMR
#define RDOSMR_SMR

#include "rdoparser_object.h"
#include <rdotrace.h>
#include <rdocommon.h>

namespace rdoRuntime
{
class RDOCalc;
}

namespace rdoParse 
{

int smr1parse( void* lexer );
int smr1lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void smr1error( char* mes );

int smr2parse( void* lexer );
int smr2lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void smr2error( char* mes );

class RDOFUNLogic;
class RDOFUNArithm;

class BreakPoint: public RDODeletable
{
	std::string *name;
	RDOFUNLogic *logic;
};

// ----------------------------------------------------------------------------
// ---------- RDOSMR
// ----------------------------------------------------------------------------
class RDOSMR: public RDOParserObject
{
private:
	std::map< std::string, std::string > files;
	std::map< std::string, double > values;

public:
	rdoSimulator::ShowMode showMode;

	bool showModeSet;
	bool frameNumberSet;

	int frameNumber;

	RDOFUNLogic* terminateIf;
	std::vector< BreakPoint* > breakPoints;
	rdoRuntime::RDOCalc* startCalcs;

public:
	RDOSMR( RDOParser* _parser, const std::string& _modelName );

	void setFile( const std::string& file_type, const std::string& file_name ) {
		files[ file_type ] = file_name;
	}
	bool hasFile( const std::string& file_type ) {
		return files.find( file_type ) != files.end();
	}
	std::string getFile( const std::string& file_type ) {
		return hasFile( file_type ) ? files[ file_type ] : "";
	}

	void setValue( const std::string& value_name, double value ) {
		values[ value_name ] = value;
	}
	bool hasValue( const std::string& value_name ) {
		return values.find( value_name ) != values.end();
	}
	double getValue( const std::string& value_name ) {
		return hasValue( value_name ) ? values[ value_name ] : 0;
	}

	void setShowMode(rdoSimulator::ShowMode sm);
	void setFrameNumber(int fn);
	void setTerminateIf(RDOFUNLogic *logic);
	void setConstValue( const std::string& constName, RDOFUNArithm* arithm );
	void setResParValue( const std::string& resName, const std::string& parName, RDOFUNArithm* arithm );
	void setSeed( const std::string& seqName, int _base );
};

}		// namespace rdoParse 

#endif //RDOSMR_SMR

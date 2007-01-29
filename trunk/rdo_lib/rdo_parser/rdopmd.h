#ifndef RDOPMD_PMD
#define RDOPMD_PMD

#include "rdoparsebase.h"
#include <rdopokaz.h>

namespace rdoRuntime
{
class RDOCalc;
class RDOResource;
}

namespace rdoParse 
{

int pmdparse( void* lexer );
int pmdlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer );
void pmderror( char* mes );

class RDOFUNLogic;
class RDOFUNArithm;

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
class RDOPMDPokaz: public RDOParserObject
{
protected:
	rdoRuntime::RDOPMDPokaz* pokaz_runtime;
	void endOfCreation( rdoRuntime::RDOPMDPokaz* _pokaz_runtime );

public:
	RDOPMDPokaz( RDOParser* _parser );
	virtual ~RDOPMDPokaz() {}
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz
{
public:
	RDOPMDWatchPar( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resName, std::string* _parName );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz
{
public:
	RDOPMDWatchState( RDOParser* _parser, std::string* _name, bool _trace, RDOFUNLogic* _logic );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDPokaz
{
public:
	RDOPMDWatchQuant( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resTypeName );
	void setLogic( RDOFUNLogic* _logic );
	void setLogicNoCheck();
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDPokaz
{
public:
	RDOPMDWatchValue( RDOParser* _parser, std::string* _name, bool _trace, std::string* _resTypeName );
	void setLogic( RDOFUNLogic* _logic, RDOFUNArithm* _arithm );
	void setLogicNoCheck( RDOFUNArithm* _arithm );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
public:
	RDOPMDGetValue( RDOParser* _parser, std::string* _name, RDOFUNArithm* _arithm );
};

} // namespace rdoParse

#endif // RDOPMD_PMD

#ifndef RDOPMD_PMD
#define RDOPMD_PMD

#include "rdoparser_object.h"
#include <rdopokaz.h>

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
class RDOPMDPokaz: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOPMDPokaz( RDOParser* _parser, const RDOParserSrcInfo& _src_info );
	virtual ~RDOPMDPokaz() {}
	const std::string& name() const { return src_text(); }

protected:
	LPIPokaz m_pokaz;
	void endOfCreation(CREF(LPIPokaz) pokaz);
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz
{
public:
	RDOPMDWatchPar( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, const RDOParserSrcInfo& _res_src_info, const RDOParserSrcInfo& _par_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz
{
public:
	RDOPMDWatchState( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, RDOFUNLogic* _logic );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchTemp
// ----------------------------------------------------------------------------
class RDOPMDWatchTemp: public RDOPMDPokaz
{
public:
	RDOPMDWatchTemp( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _res_type_src_info );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDWatchTemp
{
public:
	RDOPMDWatchQuant( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, const RDOParserSrcInfo& _res_type_src_info );
	void setLogic( RDOFUNLogic* _logic );
	void setLogicNoCheck();
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDWatchTemp
{
public:
	RDOPMDWatchValue( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, const RDOParserSrcInfo& _res_type_src_info );
	void setLogic( RDOFUNLogic* _logic, RDOFUNArithm* _arithm );
	void setLogicNoCheck( RDOFUNArithm* _arithm );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
public:
	RDOPMDGetValue( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDOFUNArithm* _arithm );
};

} // namespace rdoParse

#endif // RDOPMD_PMD

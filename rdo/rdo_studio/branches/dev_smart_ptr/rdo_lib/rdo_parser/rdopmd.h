#ifndef RDOPMD_PMD
#define RDOPMD_PMD

#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdopokaz.h"
#include "rdo_lib/rdo_parser/rdofun.h"

namespace rdoParse 
{

int  pmdparse(PTR(void) lexer);
int  pmdlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void pmderror(PTR(char) mes);

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
	RDOPMDWatchState( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, REF(LPRDOFUNLogic) _logic );
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
	void setLogic( REF(LPRDOFUNLogic) _logic );
	void setLogicNoCheck();
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDWatchTemp
{
public:
	RDOPMDWatchValue( RDOParser* _parser, const RDOParserSrcInfo& _src_info, bool _trace, const RDOParserSrcInfo& _res_type_src_info );
	void setLogic( REF(LPRDOFUNLogic) _logic, REF(LPRDOFUNArithm) _arithm );
	void setLogicNoCheck( REF(LPRDOFUNArithm) _arithm );
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
public:
	RDOPMDGetValue( RDOParser* _parser, const RDOParserSrcInfo& _src_info, REF(LPRDOFUNArithm) _arithm );
};

} // namespace rdoParse

#endif // RDOPMD_PMD

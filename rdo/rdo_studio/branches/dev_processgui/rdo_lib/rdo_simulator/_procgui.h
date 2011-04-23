// ====================================================================== INCLUDES

// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
// ===============================================================================

class ProcGUIBlock
{
private:
	PTR(rdoParse::RDOParser)     m_Parser;
	PTR(rdoRuntime::RDORuntime)  m_Runtime;
	LPILogic                     l_runtime;
	LPIPROCBlock                 b_runtime;
public:
	ProcGUIBlock(PTR(rdoParse::RDOParser) m_pParser, PTR(rdoRuntime::RDORuntime) m_pRuntime);
	virtual ~ProcGUIBlock();
	void Create();
};
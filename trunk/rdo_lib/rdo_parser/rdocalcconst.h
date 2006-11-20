#ifndef RDO_CALC_CONST
#define RDO_CALC_CONST

#include "rdoStdFuncs.h"
#include <rdocommon.h>

namespace rdoRuntime
{

class RDORuntime;

class RDOCalc
{
private:
	virtual RDOValue calcValue( RDORuntime* sim ) const = 0;

public:
	RDOCalc();
	virtual ~RDOCalc();

	rdoModelObjects::RDOFileType fileToParse;
	int lineno;
	RDOValue calcValueBase( RDORuntime* sim ) const;
};

class RDOCalcConst: public RDOCalc
{
private:
	RDOValue constanta;

public:
	RDOCalcConst( const RDOValue& val ): constanta( val ) {};
	virtual RDOValue calcValue( RDORuntime* sim ) const { return constanta; }
};

}  // namespace rdoRuntime

#endif //RDO_CALC_CONST

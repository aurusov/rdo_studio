#ifndef RDO_CALC_CONST
#define RDO_CALC_CONST

#include "rdoparser_object.h"
#include "rdortp.h"
#include <rdocommon.h>

namespace rdoRuntime
{

class RDORuntime;

class RDOCalc: public rdoParse::RDODeletable, public rdoParse::RDOErrorPos
{
private:
	rdoModelObjects::RDOFileType fileToParse;
	virtual RDOValue calcValue( RDORuntime* sim ) const = 0;

public:
	RDOCalc();
	virtual ~RDOCalc();

	RDOValue calcValueBase( RDORuntime* sim ) const;

	rdoModelObjects::RDOFileType getFileType() const { return fileToParse; }
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

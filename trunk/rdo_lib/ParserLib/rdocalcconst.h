#ifndef RDO_CALC_CONST
#define RDO_CALC_CONST

#include "rdoStdFuncs.h"

namespace rdoRuntime
{

class RDORuntime;

class RDOCalc
{
   virtual RDOValue calcValue(RDORuntime *sim) const = 0;
public:
	rdoParse::FileToParse fileToParse;
	int lineno;
   RDOValue calcValueBase(RDORuntime *sim) const;
   virtual ~RDOCalc() {}
   RDOCalc();
};

class RDOCalcConst: public RDOCalc
{
   RDOValue constanta;
public:
	RDOCalcConst(RDOValue val) : constanta(val) {}
   RDOValue calcValue(RDORuntime *sim) const { return constanta; }
};


}  // namespace rdoRuntime

#endif //RDO_CALC_CONST

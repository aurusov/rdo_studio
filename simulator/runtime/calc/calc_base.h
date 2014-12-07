#ifndef _LIB_RUNTIME_CALC_BASE_H_
#define _LIB_RUNTIME_CALC_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);

PREDECLARE_POINTER(RDOCalc);
class RDOCalc: public rdo::counter_reference
{
DECLARE_FACTORY(RDOCalc)

public:
	RDOValue calcValue(const LPRDORuntime& pRuntime);

	virtual bool compare(const LPRDOCalc& pCalc) const;

	const RDOSrcInfo& srcInfo() const;
	void setSrcInfo(const RDOSrcInfo& srcInfo);

	typedef std::vector<LPRDOCalc> RDOCalcList;

protected:
	RDOCalc();
	virtual ~RDOCalc();

	virtual RDOValue doCalc(const LPRDORuntime& pRuntime) = 0;

private:
	RDOSrcInfo m_srcInfo;
};

#define DECLARE_ICalc \
private:              \
	RDOValue doCalc(const LPRDORuntime& pRuntime);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_BASE_H_

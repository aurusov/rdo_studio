#ifndef RDO_KEYBOARD_H
#define RDO_KEYBOARD_H

#include "rdo_operation.h"
#include "rdo_keyboard_interface.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOKeyboard
// ----------------------------------------------------------------------------
class RDOKeyboard: public RDOOperation, public IKeyboard
{
DEFINE_FACTORY(RDOKeyboard);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOOperation)
	QUERY_INTERFACE(IKeyboard)
QUERY_INTERFACE_END

private:
	RDOKeyboard( RDORuntime* runtime, RDOPatternKeyboard* pattern, bool trace, const std::string& name );
	RDOKeyboard( RDORuntime* runtime, RDOPatternKeyboard* pattern, bool trace, RDOCalc* condition, const std::string& name );
	virtual ~RDOKeyboard();

	bool m_shift;
	bool m_control;
	int  m_scan_code;

	virtual bool choiceFrom( RDOSimulator* sim );

	DECLARE_IKeyboard;
};

} // namespace rdoRuntime

#endif // RDO_KEYBOARD_H

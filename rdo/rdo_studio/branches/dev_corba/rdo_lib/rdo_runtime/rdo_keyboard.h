#ifndef RDO_KEYBOARD_H
#define RDO_KEYBOARD_H

#include "rdo_operation.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOKeyboard
// ----------------------------------------------------------------------------
class RDOKeyboard: public RDOOperation
{
public:
	RDOKeyboard( RDORuntime* runtime, RDOPatternKeyboard* pattern, bool trace, const std::string& name );
	RDOKeyboard( RDORuntime* runtime, RDOPatternKeyboard* pattern, bool trace, RDOCalc* condition, const std::string& name );
	virtual ~RDOKeyboard();

	enum AddHotKey {
		addhk_ok,
		addhk_already,
		addhk_notfound,
		addhk_dont
	};
	AddHotKey addHotKey( RDORuntime* runtime, const std::string& hotKey );
	bool hasHotKey() const { return m_scan_code != -1 ? true : false; }

private:
	bool m_shift;
	bool m_control;
	int  m_scan_code;

	virtual bool choiceFrom( RDOSimulator* sim );
};

} // namespace rdoRuntime

#endif // RDO_KEYBOARD_H

#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdo_keyboard_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOKeyboard: public RDOOperation, public IKeyboard
{
DECLARE_FACTORY(RDOKeyboard);
private:
	RDOKeyboard(const LPRDORuntime& pRuntime, RDOPatternKeyboard* pattern, bool trace, const std::string& name);
	RDOKeyboard(const LPRDORuntime& pRuntime, RDOPatternKeyboard* pattern, bool trace, const LPRDOCalc& pCondition, const std::string& name);
	virtual ~RDOKeyboard();

	bool m_shift;
	bool m_control;
	std::size_t m_scan_code;

	virtual bool choiceFrom(const LPRDORuntime& pRuntime);

	DECLARE_IKeyboard;
};

CLOSE_RDO_RUNTIME_NAMESPACE

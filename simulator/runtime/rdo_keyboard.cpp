/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_keyboard.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      18.08.2010
  \brief     Клавиатурные операции
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/pch/stdpch.h"
#include "simulator/runtime/rdo_keyboard.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

#ifndef COMPILER_VISUAL_STUDIO
	//! @todo А что на самом деле делать под линухами ?
	const std::size_t VK_SHIFT = 0x10;
	const std::size_t VK_CONTROL = 0x11;
#endif

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOKeyboard
// --------------------------------------------------------------------------------
RDOKeyboard::RDOKeyboard(const LPRDORuntime& pRuntime, RDOPatternKeyboard* pattern, bool trace, const std::string& name)
	: RDOOperation(pRuntime, pattern, trace, name)
	, m_shift     (false                         )
	, m_control   (false                         )
	, m_scan_code (RDOHotKey::UNDEFINED_KEY      )
{}

RDOKeyboard::RDOKeyboard(const LPRDORuntime& pRuntime, RDOPatternKeyboard* pattern, bool trace, const LPRDOCalc& pCondition, const std::string& name)
	: RDOOperation(pRuntime, pattern, trace, pCondition, name)
	, m_shift     (false                                     )
	, m_control   (false                                     )
	, m_scan_code (RDOHotKey::UNDEFINED_KEY                  )
{}

RDOKeyboard::~RDOKeyboard()
{}

bool RDOKeyboard::hasHotKey() const
{
	return m_scan_code != std::size_t(-1) ? true : false;
}

IKeyboard::AddHotKeyResult RDOKeyboard::addHotKey(const LPRDORuntime& pRuntime, const std::string& hotKey)
{
	RDOHotKey::KeyCode scanCode = pRuntime->hotkey().toolkit().codeFromString(hotKey);
	if (scanCode   == RDOHotKey::UNDEFINED_KEY)
		return IKeyboard::addhk_notfound;
	if (m_scan_code != RDOHotKey::UNDEFINED_KEY && scanCode != VK_SHIFT && scanCode != VK_CONTROL)
		return IKeyboard::addhk_already;
	switch (scanCode)
	{
	case VK_SHIFT:
		m_shift = true;
		pRuntime->hotkey().keyInModel().insert(VK_SHIFT);
		break;
	case VK_CONTROL:
		m_control = true;
		pRuntime->hotkey().keyInModel().insert(VK_CONTROL);
		break;
	default:
		m_scan_code = scanCode;
		if (m_scan_code)
		{
			pRuntime->hotkey().keyInModel().insert(scanCode);
		}
		break;
	}
	return IKeyboard::addhk_ok;
}

bool RDOKeyboard::choiceFrom(const LPRDORuntime& pRuntime)
{
	pRuntime->setCurrentActivity(this);

	if (!pRuntime->hotkey().areaList().check(m_oprName))
	{
		if (!pRuntime->hotkey().keyDown().isPressed(m_scan_code, m_shift, m_control))
			return false;
	}
	return RDOOperation::choiceFrom(pRuntime);
}

CLOSE_RDO_RUNTIME_NAMESPACE

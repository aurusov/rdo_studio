/*!
  \copyright (c) RDO-Team, 2012
  \file      hotkey.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2012
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/hotkey.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOHotKeyToolkit
// --------------------------------------------------------------------------------
RDOHotKeyToolkit::RDOHotKeyToolkit()
{
	m_keyList.insert(KeySet::value_type("ESCAPE",   VK_ESCAPE));
	m_keyList.insert(KeySet::value_type("TAB",      VK_TAB));
	m_keyList.insert(KeySet::value_type("SHIFT",    VK_SHIFT));
	m_keyList.insert(KeySet::value_type("CONTROL",  VK_CONTROL));
	m_keyList.insert(KeySet::value_type("BACK",     VK_BACK));
	m_keyList.insert(KeySet::value_type("RETURN",   VK_RETURN));
	m_keyList.insert(KeySet::value_type("INSERT",   VK_INSERT));
	m_keyList.insert(KeySet::value_type("HOME",     VK_HOME));
	m_keyList.insert(KeySet::value_type("PRIOR",    VK_PRIOR));
	m_keyList.insert(KeySet::value_type("DELETE",   VK_DELETE));
	m_keyList.insert(KeySet::value_type("END",      VK_END));
	m_keyList.insert(KeySet::value_type("NEXT",     VK_NEXT));
	m_keyList.insert(KeySet::value_type("UP",       VK_UP));
	m_keyList.insert(KeySet::value_type("LEFT",     VK_LEFT));
	m_keyList.insert(KeySet::value_type("DOWN",     VK_DOWN));
	m_keyList.insert(KeySet::value_type("RIGHT",    VK_RIGHT));
	m_keyList.insert(KeySet::value_type("DIVIDE",   VK_DIVIDE));
	m_keyList.insert(KeySet::value_type("MULTIPLY", VK_MULTIPLY));
	m_keyList.insert(KeySet::value_type("SUBTRACT", VK_SUBTRACT));
	m_keyList.insert(KeySet::value_type("ADD",      VK_ADD));
	m_keyList.insert(KeySet::value_type("CLEAR",    VK_CLEAR));
	m_keyList.insert(KeySet::value_type("SPACE",    VK_SPACE));
	m_keyList.insert(KeySet::value_type("DECIMAL",  VK_DECIMAL));
	m_keyList.insert(KeySet::value_type("F2",       VK_F2));
	m_keyList.insert(KeySet::value_type("F3",       VK_F3));
	m_keyList.insert(KeySet::value_type("F4",       VK_F4));
	m_keyList.insert(KeySet::value_type("F5",       VK_F5));
	m_keyList.insert(KeySet::value_type("F6",       VK_F6));
	m_keyList.insert(KeySet::value_type("F7",       VK_F7));
	m_keyList.insert(KeySet::value_type("F8",       VK_F8));
	m_keyList.insert(KeySet::value_type("F9",       VK_F9));
	m_keyList.insert(KeySet::value_type("F10",      VK_F10));
	m_keyList.insert(KeySet::value_type("F11",      VK_F11));
	m_keyList.insert(KeySet::value_type("F12",      VK_F12));
	m_keyList.insert(KeySet::value_type("NUMPAD0",  VK_NUMPAD0));
	m_keyList.insert(KeySet::value_type("NUMPAD1",  VK_NUMPAD1));
	m_keyList.insert(KeySet::value_type("NUMPAD2",  VK_NUMPAD2));
	m_keyList.insert(KeySet::value_type("NUMPAD3",  VK_NUMPAD3));
	m_keyList.insert(KeySet::value_type("NUMPAD4",  VK_NUMPAD4));
	m_keyList.insert(KeySet::value_type("NUMPAD5",  VK_NUMPAD5));
	m_keyList.insert(KeySet::value_type("NUMPAD6",  VK_NUMPAD6));
	m_keyList.insert(KeySet::value_type("NUMPAD7",  VK_NUMPAD7));
	m_keyList.insert(KeySet::value_type("NUMPAD8",  VK_NUMPAD8));
	m_keyList.insert(KeySet::value_type("NUMPAD9",  VK_NUMPAD9));
	m_keyList.insert(KeySet::value_type("NOKEY",    0));

	for (char i = '0'; i <= '9'; ++i)
	{
		m_keyList.insert(KeySet::value_type(tstring(1, i), (KeyCode)i));
	}

	for (char i = 'A'; i <= 'Z'; ++i)
	{
		m_keyList.insert(KeySet::value_type(tstring(1, i), (KeyCode)i));
	}
}

RDOHotKeyToolkit::KeyCode RDOHotKeyToolkit::codeFromString(CREF(tstring) key) const
{
	CIterator it = m_keyList.find(key);
	if (it == m_keyList.end())
	{
		return KeyCode(UNDEFINED_KEY);
	}
	return (*it).second;
}

CLOSE_RDO_RUNTIME_NAMESPACE

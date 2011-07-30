/******************************************************************************//**
 * @copyright (c) RDO-Team, 2010
 * @file      rdo_keyboard.cpp
 * @authors   Урусов Андрей, Лущан Дмитрий
 * @date      18.08.2010
 * @brief     
 * @indent    4T
 */

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_keyboard.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOKeyboard
// ********************************************************************************
RDOKeyboard::RDOKeyboard( CREF(LPRDORuntime) pRuntime, RDOPatternKeyboard* pattern, rbool trace, CREF(tstring) name ):
	RDOOperation( pRuntime, pattern, trace, name ),
	m_shift( false ),
	m_control( false ),
	m_scan_code( RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY )
{}

RDOKeyboard::RDOKeyboard( CREF(LPRDORuntime) pRuntime, RDOPatternKeyboard* pattern, rbool trace, CREF(LPRDOCalc) pCondition, CREF(tstring) name ):
	RDOOperation( pRuntime, pattern, trace, pCondition, name ),
	m_shift( false ),
	m_control( false ),
	m_scan_code( RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY )
{}

RDOKeyboard::~RDOKeyboard()
{}

rbool RDOKeyboard::hasHotKey() const
{
	return m_scan_code != -1 ? true : false;
}

IKeyboard::AddHotKeyResult RDOKeyboard::addHotKey(CREF(LPRDORuntime) pRuntime, CREF(tstring) hotKey)
{
	RDORuntime::RDOHotKeyToolkit::KeyCode scan_code = pRuntime->rdoHotKeyToolkit.codeFromString( hotKey );
	if ( scan_code == RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY ) return IKeyboard::addhk_notfound;
	if ( m_scan_code != RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY && scan_code != VK_SHIFT && scan_code != VK_CONTROL ) return IKeyboard::addhk_already;
	switch ( scan_code ) {
		case VK_SHIFT  : m_shift     = true; pRuntime->using_scan_codes.push_back( VK_SHIFT   ); break;
		case VK_CONTROL: m_control   = true; pRuntime->using_scan_codes.push_back( VK_CONTROL ); break;
		default        : m_scan_code = scan_code; if ( m_scan_code ) pRuntime->using_scan_codes.push_back( scan_code ); break;
	}
	return IKeyboard::addhk_ok;
}

rbool RDOKeyboard::choiceFrom( CREF(LPRDORuntime) pRuntime )
{
	pRuntime->setCurrentActivity( this );

	if ( !pRuntime->checkAreaActivated( m_oprName ) )
	{
		if ( !pRuntime->checkKeyPressed( m_scan_code, m_shift, m_control ) ) return false;
	}
	return RDOOperation::choiceFrom( pRuntime ); 
}

CLOSE_RDO_RUNTIME_NAMESPACE

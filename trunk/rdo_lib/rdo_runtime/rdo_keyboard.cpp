#include "pch.h"
#include "rdo_keyboard.h"
#include "rdo_runtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOKeyboard
// ----------------------------------------------------------------------------
RDOKeyboard::RDOKeyboard( RDORuntime* runtime, RDOPatternKeyboard* pattern, bool trace, const std::string& name ):
	RDOOperation( runtime, pattern, trace, name ),
	m_shift( false ),
	m_control( false ),
	m_scan_code( RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY )
{
}

RDOKeyboard::RDOKeyboard( RDORuntime* runtime, RDOPatternKeyboard* pattern, bool trace, RDOCalc* condition, const std::string& name ):
	RDOOperation( runtime, pattern, trace, condition, name ),
	m_shift( false ),
	m_control( false ),
	m_scan_code( RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY )
{
}

RDOKeyboard::~RDOKeyboard()
{
}

RDOKeyboard::AddHotKey RDOKeyboard::addHotKey( RDORuntime* runtime, const std::string& hotKey )
{
	RDORuntime::RDOHotKeyToolkit::KeyCode scan_code = runtime->rdoHotKeyToolkit.codeFromString( hotKey );
	if ( scan_code == RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY ) return RDOKeyboard::addhk_notfound;
	if ( m_scan_code != RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY && scan_code != VK_SHIFT && scan_code != VK_CONTROL ) return RDOKeyboard::addhk_already;
	switch ( scan_code ) {
		case VK_SHIFT  : m_shift     = true; runtime->using_scan_codes.push_back( VK_SHIFT   ); break;
		case VK_CONTROL: m_control   = true; runtime->using_scan_codes.push_back( VK_CONTROL ); break;
		default        : m_scan_code = scan_code; if ( m_scan_code ) runtime->using_scan_codes.push_back( scan_code ); break;
	}
	return RDOKeyboard::addhk_ok;
}

bool RDOKeyboard::choiceFrom( RDOSimulator *sim )
{
	RDORuntime* runtime = static_cast<RDORuntime*>(sim);
	runtime->setCurrentActivity( this );

	if ( !runtime->checkAreaActivated( m_oprName ) ) {
		if ( !runtime->checkKeyPressed( m_scan_code, m_shift, m_control ) ) return false;
	}
	return RDOOperation::choiceFrom( sim ); 
}

} // namespace rdoRuntime

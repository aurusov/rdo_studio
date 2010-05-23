/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopatpreparse.inl
 * author   : Ћущан ƒмитрий
 * date     : 22.05.10
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEvent
// ----------------------------------------------------------------------------
inline RDOEvent::RDOEvent(CREF(tstring) name)
	: m_name(name)
{}

inline RDOEvent::~RDOEvent()
{}

inline CREF(tstring) RDOEvent::name() const
{
	return m_name;
}

CLOSE_RDO_PARSER_NAMESPACE

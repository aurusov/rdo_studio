/*
 * copyright: (c) RDO-Team, 2011
 * filename : context.inl
 * author   : Урусов Андрей
 * date     : 03.03.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

template <class T>
inline rdo::intrusive_ptr<T> Context::cast()
{
	LPContext pThis = this;
	rdo::intrusive_ptr<T> pThisResult = pThis.object_dynamic_cast<T>();
	if (pThisResult)
	{
		return pThisResult;
	}
	LPContext pPrev = m_pContextStack->prev(pThis);
	return pPrev ? pPrev->cast<T>() : rdo::intrusive_ptr<T>();
}

CLOSE_RDO_PARSER_NAMESPACE

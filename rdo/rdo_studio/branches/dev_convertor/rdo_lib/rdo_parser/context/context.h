/*
 * copyright: (c) RDO-Team, 2010
 * filename : context.h
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_H_
#define _RDOPARSER_CONTEXT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"

#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/context/stack.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Context
// ----------------------------------------------------------------------------
OBJECT(Context)
{
DECLARE_FACTORY(Context);
friend void ContextStack::push(LPContext pContext);

public:
	template <class T>
	rdo::intrusive_ptr<T> cast()
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

protected:
	Context();
	virtual ~Context();

private:
	LPContextStack m_pContextStack;

	void setContextStack(CREF(LPContextStack) pContextStack);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_CONTEXT_H_

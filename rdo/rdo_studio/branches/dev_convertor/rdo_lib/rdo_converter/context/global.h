/*
 * copyright: (c) RDO-Team, 2010
 * filename : global.h
 * author   : Урусов Андрей
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCONVERTER_CONTEXT_GLOBAL_H_
#define _RDOCONVERTER_CONTEXT_GLOBAL_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/context/context.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextGlobal
// ----------------------------------------------------------------------------
class ContextGlobal: public Context
{
DECLARE_FACTORY(ContextGlobal);
private:
	ContextGlobal();
	virtual ~ContextGlobal();

	LPContext m_timeNow;

	DECLARE_IContext;
};

DECLARE_POINTER(ContextGlobal);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _RDOCONVERTER_CONTEXT_GLOBAL_H_

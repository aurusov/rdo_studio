/*
 * copyright: (c) RDO-Team, 2011
 * filename : document_i.h
 * author   : Урусов Андрей
 * date     : 02.01.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_DOCUMENT_I_H_
#define _CONVERTOR_DOCUMENT_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/interface_ptr.h"

#include "rdo_lib/rdo_converter/namespace.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IDocument
// ----------------------------------------------------------------------------
OBJECT_INTERFACE(IDocument)
{
public:
	typedef rdoModelObjectsConvertor::RDOFileTypeOut Type;

	virtual void insert(Type type, ruint to, CREF(tstring) value) = 0;
	virtual void remove(Type type, ruint from, ruint to)          = 0;
};
#define DECLARE_IDocument                                  \
	void insert(Type type, ruint to, CREF(tstring) value); \
	void remove(Type type, ruint from, ruint to);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_DOCUMENT_I_H_

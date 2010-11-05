/*
 * copyright: (c) RDO-Team, 2010
 * filename : update_i.h
 * author   : Урусов Андрей
 * date     : 29.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_UPDATE_I_H_
#define _CONVERTOR_UPDATE_I_H_

// ====================================================================== INCLUDES
#include <fstream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/namespace.h"
#include "rdo_lib/rdo_converter/rdo_common/model_objects_convertor.h"
#include "rdo_lib/rdo_converter/update/document.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IDocUpdate
// ----------------------------------------------------------------------------
S_INTERFACE(IDocUpdate)
{
public:
	virtual void apply(REF(LPDocument) pDocument, REF(std::istream) streamIn) const = 0;
};
#define DECLARE_IDocUpdate \
	void apply(REF(LPDocument) pDocument, REF(std::istream) streamIn) const;

OBJECT(DocUpdate) IS IMPLEMENTATION_OF(IDocUpdate)
{
protected:
	DocUpdate(rdoModelObjectsConvertor::RDOFileTypeOut fileTo = rdoModelObjectsConvertor::UNDEFINED_OUT);

	rdoModelObjectsConvertor::RDOFileTypeOut m_fileTo;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_UPDATE_I_H_

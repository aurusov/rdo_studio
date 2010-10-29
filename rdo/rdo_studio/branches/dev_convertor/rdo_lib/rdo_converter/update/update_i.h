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
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IDocUpdate
// ----------------------------------------------------------------------------
S_INTERFACE(IDocUpdate)
{
public:
	virtual void apply(REF(std::istream) streamIn, REF(std::ostream) streamOut) const = 0;
};
#define DECLARE_IDocUpdate \
	void apply(REF(std::istream) streamIn, REF(std::ostream) streamOut) const;

OBJECT(DocUpdate) IS IMPLEMENTATION_OF(IDocUpdate)
{
protected:
	DocUpdate(rdoModelObjectsConvertor::RDOFileType fileTo = rdoModelObjectsConvertor::UNDEFINED, rdoModelObjectsConvertor::RDOFileType fileFrom = rdoModelObjectsConvertor::UNDEFINED);

	rdoModelObjectsConvertor::RDOFileType m_fileTo;
	rdoModelObjectsConvertor::RDOFileType m_fileFrom;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_UPDATE_I_H_

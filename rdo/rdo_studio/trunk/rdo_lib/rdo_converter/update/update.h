/*
 * copyright: (c) RDO-Team, 2010
 * filename : update.h
 * author   : Урусов Андрей
 * date     : 29.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_UPDATE_H_
#define _CONVERTOR_UPDATE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/rdomacros.h"

#include "rdo_lib/rdo_converter/update/update_i.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

CLASS(UpdateInsert): INSTANCE_OF(DocUpdate)
{
DECLARE_FACTORY(UpdateInsert)
private:
	UpdateInsert(std::istream::pos_type pos, CREF(tstring) value, rdoModelObjectsConvertor::RDOFileType file = rdoModelObjectsConvertor::UNDEFINED);

	std::istream::pos_type m_pos;
	tstring                m_value;

	DECLARE_IDocUpdate;
};

CLASS(UpdateDelete): INSTANCE_OF(DocUpdate)
{
DECLARE_FACTORY(UpdateDelete)
private:
	UpdateDelete(std::istream::pos_type posFrom, std::istream::pos_type posTo);

	std::istream::pos_type   m_posFrom;
	std::istream::pos_type   m_posTo;

	DECLARE_IDocUpdate;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_UPDATE_H_

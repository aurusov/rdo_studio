/*
 * copyright: (c) RDO-Team, 2010
 * filename : update_container.h
 * author   : Урусов Андрей
 * date     : 12.12.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_UPDATE_CONTAINER_H_
#define _CONVERTOR_UPDATE_CONTAINER_H_

// ====================================================================== INCLUDES
#include <list>
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"

#include "rdo_lib/rdo_converter/update/update_i.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- UpdateContainer
// ----------------------------------------------------------------------------
OBJECT(UpdateContainer)
{
DECLARE_FACTORY(UpdateContainer)
public:
	typedef std::list<LPDocUpdate> DocUpdateList;

	void                append    (CREF(LPDocUpdate) pDocUpdate);
	CREF(DocUpdateList) updateList() const;

private:
	 UpdateContainer();
	~UpdateContainer();

	DocUpdateList m_docUpdateList;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_UPDATE_CONTAINER_H_

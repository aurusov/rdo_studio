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

// ----------------------------------------------------------------------------
// ---------- UpdateInsert
// ----------------------------------------------------------------------------
CLASS(UpdateInsert): INSTANCE_OF(DocUpdate)
{
DECLARE_FACTORY(UpdateInsert)
private:
	UpdateInsert(ruint pos, CREF(tstring) value, IDocument::Type file = rdoModelObjectsConvertor::UNDEFINED_OUT);

	ruint    m_pos;
	tstring  m_value;

	DECLARE_IDocUpdate;
};

// ----------------------------------------------------------------------------
// ---------- UpdateDelete
// ----------------------------------------------------------------------------
CLASS(UpdateDelete): INSTANCE_OF(DocUpdate)
{
DECLARE_FACTORY(UpdateDelete)
private:
	UpdateDelete(ruint posFrom, ruint posTo);

	ruint m_posFrom;
	ruint m_posTo;

	DECLARE_IDocUpdate;
};

// ----------------------------------------------------------------------------
// ---------- UpdateMove
// ----------------------------------------------------------------------------
CLASS(UpdateMove): INSTANCE_OF(DocUpdate)
{
DECLARE_FACTORY(UpdateMove)
private:
	UpdateMove(ruint           posFromBegin,
	           ruint           posFromEnd,
	           ruint           posTo,
	           IDocument::Type fileTo   = rdoModelObjectsConvertor::UNDEFINED_OUT,
	           IDocument::Type fileFrom = rdoModelObjectsConvertor::UNDEFINED_OUT);

	ruint           m_posFromBegin;
	ruint           m_posFromEnd;
	ruint           m_posTo;
	IDocument::Type m_fileFrom;

	DECLARE_IDocUpdate;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_UPDATE_H_

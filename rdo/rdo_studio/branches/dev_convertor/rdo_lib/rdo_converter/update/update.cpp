/*
 * copyright: (c) RDO-Team, 2010
 * filename : update.cpp
 * author   : Урусов Андрей
 * date     : 29.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/update/update.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- DocUpdate
// ----------------------------------------------------------------------------
DocUpdate::DocUpdate(IDocument::Type fileTo)
	: m_fileTo(fileTo)
{
	if (m_fileTo == rdoModelObjectsConvertor::UNDEFINED_OUT)
	{
		m_fileTo = getCurrentType();
	}
}

IDocument::Type DocUpdate::getCurrentType() const
{
	switch (Converter::getFileToParse())
	{
	case rdoModelObjectsConvertor::PAT_IN: return rdoModelObjectsConvertor::PAT_OUT;
	case rdoModelObjectsConvertor::RTP_IN: return rdoModelObjectsConvertor::RTP_OUT;
	case rdoModelObjectsConvertor::RSS_IN: return rdoModelObjectsConvertor::RSS_OUT;
	case rdoModelObjectsConvertor::OPR_IN: NEVER_REACH_HERE;
	case rdoModelObjectsConvertor::FRM_IN: return rdoModelObjectsConvertor::FRM_OUT;
	case rdoModelObjectsConvertor::FUN_IN: return rdoModelObjectsConvertor::FUN_OUT;
	case rdoModelObjectsConvertor::DPT_IN: return rdoModelObjectsConvertor::DPT_OUT;
	case rdoModelObjectsConvertor::SMR_IN: return rdoModelObjectsConvertor::SMR_OUT;
	case rdoModelObjectsConvertor::PMD_IN: return rdoModelObjectsConvertor::PMD_OUT;
	case rdoModelObjectsConvertor::PMV_IN: return rdoModelObjectsConvertor::PMV_OUT;
	case rdoModelObjectsConvertor::TRC_IN: return rdoModelObjectsConvertor::TRC_OUT;
	default: NEVER_REACH_HERE;
	}
	NEVER_REACH_HERE;
	return rdoModelObjectsConvertor::TRC_OUT;
}

// ----------------------------------------------------------------------------
// ---------- UpdateInsert
// ----------------------------------------------------------------------------
UpdateInsert::UpdateInsert(ruint pos, CREF(tstring) value, IDocument::Type file)
	: DocUpdate(file )
	, m_pos    (pos  )
	, m_value  (value)
{}

void UpdateInsert::dump(REF(LPIDocument) pDocument) const
{
	tstring log = rdo::format(_T("update insert <%d>, %d, '%s', dump: '%s'\n"), m_fileTo, m_pos, m_value.c_str(), pDocument->get(m_fileTo, m_pos, m_pos).c_str());
	TRACE(log.c_str());
}

void UpdateInsert::apply(REF(LPIDocument) pDocument) const
{
	pDocument->insert(m_fileTo, m_pos, m_value);
}

void UpdateInsert::insert(IDocument::Type type, ruint to, ruint size)
{
	if (m_fileTo != type)
		return;

	ASSERT(to < m_pos || to > m_pos + m_value.length());

	if (to < m_pos)
	{
		m_pos += size;
	}
}

void UpdateInsert::remove(IDocument::Type type, ruint from, ruint to)
{
	if (m_fileTo != type)
		return;

	ASSERT(to <= m_pos || (from > m_pos + m_value.length() && to > m_pos + m_value.length()));

	if (to <= m_pos)
	{
		m_pos -= to - from;
	}
}

// ----------------------------------------------------------------------------
// ---------- UpdateDelete
// ----------------------------------------------------------------------------
UpdateDelete::UpdateDelete(ruint posFrom, ruint posTo)
	: m_posFrom(posFrom)
	, m_posTo  (posTo  )
{
	ASSERT(m_posFrom < m_posTo);
}

void UpdateDelete::dump(REF(LPIDocument) pDocument) const
{
	tstring log = rdo::format(_T("update delete <%d>, [%d - %d], dump: '%s'\n"), m_fileTo, m_posFrom, m_posTo, pDocument->get(m_fileTo, m_posFrom, m_posTo).c_str());
	TRACE(log.c_str());
}

void UpdateDelete::apply(REF(LPIDocument) pDocument) const
{
	pDocument->remove(m_fileTo, m_posFrom, m_posTo);
}

void UpdateDelete::insert(IDocument::Type type, ruint to, ruint size)
{
	if (m_fileTo != type)
		return;

	ASSERT(to <= m_posFrom || to > m_posTo);

	if (to <= m_posFrom)
	{
		m_posFrom += size;
		m_posTo   += size;
	}
}

void UpdateDelete::remove(IDocument::Type type, ruint from, ruint to)
{
	if (m_fileTo != type)
		return;

	ASSERT(to < m_posFrom || (from > m_posTo && to > m_posTo));

	if (to < m_posFrom)
	{
		m_posFrom -= to - from;
		m_posTo   -= to - from;
	}
}

// ----------------------------------------------------------------------------
// ---------- UpdateReplace
// ----------------------------------------------------------------------------
UpdateReplace::UpdateReplace(ruint posFrom, ruint posTo, CREF(tstring) value, IDocument::Type file)
	: DocUpdate(file)
{
	pDelete = rdo::Factory<UpdateDelete>::create(posFrom, posTo);
	ASSERT(pDelete);

	pInsert = rdo::Factory<UpdateInsert>::create(posFrom, value, file);
	ASSERT(pInsert);
}

void UpdateReplace::dump(REF(LPIDocument) pDocument) const
{
	TRACE(_T("update replace...\n"));
	pDelete->dump(pDocument);
	pInsert->dump(pDocument);
	TRACE(_T("update replace... ok\n"));
}

void UpdateReplace::apply(REF(LPIDocument) pDocument) const
{
	pDelete->apply(pDocument);
	pInsert->apply(pDocument);
}

void UpdateReplace::insert(IDocument::Type type, ruint to, ruint size)
{
	pDelete->insert(type, to, size);
	pInsert->insert(type, to, size);
}

void UpdateReplace::remove(IDocument::Type type, ruint from, ruint to)
{
	pDelete->remove(type, from, to);
	pInsert->remove(type, from, to);
}

// ----------------------------------------------------------------------------
// ---------- UpdateMove
// ----------------------------------------------------------------------------
UpdateMove::UpdateMove(ruint posFromBegin, ruint posFromEnd, ruint posTo, IDocument::Type fileTo, IDocument::Type fileFrom)
	: DocUpdate     (fileTo      )
	, m_posFromBegin(posFromBegin)
	, m_posFromEnd  (posFromEnd  )
	, m_posTo       (posTo       )
	, m_fileFrom    (fileFrom    )
{
	if (m_fileFrom == rdoModelObjectsConvertor::UNDEFINED_OUT)
	{
		m_fileFrom = getCurrentType();
	}
}

void UpdateMove::dump(REF(LPIDocument) pDocument) const
{
	tstring log = rdo::format(_T("update move <%d->%d>, [%d - %d] '%s', %d\n")
		, m_fileFrom
		, m_fileTo
		, m_posFromBegin
		, m_posFromEnd
		, pDocument->get(m_fileFrom, m_posFromBegin, m_posFromEnd).c_str()
		, m_posTo
	);
	TRACE(log.c_str());
}

void UpdateMove::apply(REF(LPIDocument) pDocument) const
{
	tstring cut = pDocument->get(m_fileFrom, m_posFromBegin, m_posFromEnd);
	pDocument->remove(m_fileFrom, m_posFromBegin, m_posFromEnd);

	ruint pos = m_posTo;
	if (m_posFromEnd < pos)
	{
		//! Удалили перед собой, сдвинемся к началу
		pos -= m_posFromEnd - m_posFromBegin;
	}
	pDocument->insert(m_fileTo, pos, cut);
}

void UpdateMove::insert(IDocument::Type type, ruint to, ruint size)
{
	if (m_fileTo != type)
		return;

	if (to <= m_posFromBegin && m_posFromBegin != m_posFromEnd)
	{
		//! Вставка до, сдвинемся к концу
		m_posFromBegin += size;
		m_posFromEnd   += size;
	}
	else if (to == m_posFromBegin && m_posFromBegin == m_posFromEnd)
	{
		//! Вставка в пустой интервал, расширим конец
		m_posFromEnd += size;
	}
	else if (to > m_posFromBegin && to <= m_posFromEnd)
	{
		//! Вставка внутрь, расширим конец
		m_posFromEnd += size;
	}

	if (to < m_posTo)
	{
		//! Вставка до, сдвинемся к концу
		m_posTo += size;
	}
}

void UpdateMove::remove(IDocument::Type type, ruint from, ruint to)
{
	if (m_fileTo != type)
		return;

	if (to < m_posFromBegin)
	{
		//! Удаление до, сдвинемся к началу
		m_posFromBegin -= to - from;
		m_posFromEnd   -= to - from;
	}
	else if (m_posFromBegin <= from && to <= m_posFromEnd)
	{
		//! Удаление внутри, подрежем конец
		m_posFromEnd -= to - from;
	}

	if (to < m_posTo)
	{
		//! Удаление до, сдвинемся к началу
		m_posTo -= to - from;
	}
}

CLOSE_RDO_CONVERTER_NAMESPACE

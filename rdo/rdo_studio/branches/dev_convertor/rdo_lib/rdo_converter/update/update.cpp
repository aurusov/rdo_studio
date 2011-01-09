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
DocUpdate::DocUpdate(IDocument::Type file)
	: m_file(file)
{
	if (m_file == rdoModelObjectsConvertor::UNDEFINED_OUT)
	{
		m_file = getCurrentType();
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
	tstring log = rdo::format(_T("update insert <%d>, %d, '%s', dump: '%s'\n"), m_file, m_pos, m_value.c_str(), pDocument->get(m_file, m_pos, m_pos).c_str());
	TRACE(log.c_str());
}

void UpdateInsert::apply(REF(LPIDocument) pDocument) const
{
	pDocument->insert(m_file, m_pos, m_value);
}

void UpdateInsert::insert(IDocument::Type type, ruint to, ruint size)
{
	if (m_file != type)
		return;

	ASSERT(to <= m_pos || to >= m_pos + m_value.length());

	if (to <= m_pos)
	{
		m_pos += size;
	}
}

void UpdateInsert::remove(IDocument::Type type, ruint from, ruint to)
{
	if (m_file != type)
		return;

	ASSERT(to <= m_pos || (from >= m_pos + m_value.length() && to > m_pos + m_value.length()));

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
	tstring log = rdo::format(_T("update delete <%d>, [%d - %d), dump: '%s'\n"), m_file, m_posFrom, m_posTo, pDocument->get(m_file, m_posFrom, m_posTo).c_str());
	TRACE(log.c_str());
}

void UpdateDelete::apply(REF(LPIDocument) pDocument) const
{
	pDocument->remove(m_file, m_posFrom, m_posTo);
}

void UpdateDelete::insert(IDocument::Type type, ruint to, ruint size)
{
	if (m_file != type)
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
	if (m_file != type)
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
	tstring log = rdo::format(_T("update move <%d->%d>, [%d - %d) '%s', %d\n")
		, m_fileFrom
		, m_file
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
	pDocument->insert(m_file, pos, cut);
}

void UpdateMove::insert(IDocument::Type type, ruint to, ruint size)
{
	if (m_file != type)
		return;

	if (to < m_posFromBegin && m_posFromBegin != m_posFromEnd)
	{
		//! Вставка до, сдвинемся к концу
		m_posFromBegin += size;
		m_posFromEnd   += size;
	}
	else if (to == m_posFromBegin && to + size <= m_posFromEnd && m_posFromBegin != m_posFromEnd)
	{
		//! Вставка внутри, расширим конец
		m_posFromEnd += size;
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
	if (m_file != type)
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

// ----------------------------------------------------------------------------
// ---------- UpdateSwap
// ----------------------------------------------------------------------------
UpdateSwap::UpdateSwap(ruint pos1Begin, ruint pos1End, ruint pos2Begin, ruint pos2End, IDocument::Type file)
	: DocUpdate  (file     )
	, m_pos1Begin(pos1Begin)
	, m_pos1End  (pos1End  )
	, m_pos2Begin(pos2Begin)
	, m_pos2End  (pos2End  )
{}

void UpdateSwap::dump(REF(LPIDocument) pDocument) const
{
	tstring log = rdo::format(_T("update swap <%d>: [%d, %d) <-> [%d - %d), '%s' <-> '%s'\n")
		, m_file
		, m_pos1Begin
		, m_pos1End
		, m_pos2Begin
		, m_pos2End
		, pDocument->get(m_file, m_pos1Begin, m_pos1End).c_str()
		, pDocument->get(m_file, m_pos2Begin, m_pos2End).c_str()
	);
	TRACE(log.c_str());
}

void UpdateSwap::apply(REF(LPIDocument) pDocument) const
{
	//! Запомним значения
	tstring cut1 = pDocument->get(m_file, m_pos1Begin, m_pos1End);
	tstring cut2 = pDocument->get(m_file, m_pos2Begin, m_pos2End);

	//! Удалим первый интервал
	pDocument->remove(m_file, m_pos1Begin, m_pos1End);

	ruint pos2Begin = m_pos2Begin;
	ruint pos2End   = m_pos2End;
	if (m_pos1End <= pos2Begin)
	{
		//! Удаление перед вторым интервалом, сдвинемся к началу
		pos2Begin -= m_pos1End - m_pos1Begin;
		pos2End   -= m_pos1End - m_pos1Begin;
	}

	//! Удалим второй интервал
	pDocument->remove(m_file, pos2Begin, pos2End);

	ruint pos1Begin = m_pos1Begin;
	ruint pos1End   = m_pos1End;
	if (pos2End <= pos1Begin)
	{
		//! Удаление перед первым интервалом, сдвинемся к началу
		pos1Begin -= pos2End - pos2Begin;
		pos1End   -= pos2End - pos2Begin;
	}

	//! Вставка первого буффера на место второго
	pDocument->insert(m_file, pos2Begin, cut1);

	if (pos2Begin <= pos1Begin)
	{
		//! Вставка перед первым интервалом, сдвинемся к концу
		pos1Begin += cut1.length();
		pos1End   += cut1.length();
	}

	//! Вставка второго буффера на место первого
	pDocument->insert(m_file, pos1Begin, cut2);
}

void UpdateSwap::insert(IDocument::Type type, ruint to, ruint size)
{
	if (m_file != type)
		return;

	insert(to, size, m_pos1Begin, m_pos1End);
	insert(to, size, m_pos2Begin, m_pos2End);
}

void UpdateSwap::remove(IDocument::Type type, ruint from, ruint to)
{
	if (m_file != type)
		return;

	remove(from, to, m_pos1Begin, m_pos1End);
	remove(from, to, m_pos2Begin, m_pos2End);
}

void UpdateSwap::insert(CREF(ruint) from, CREF(ruint) size, REF(ruint) posBegin, REF(ruint) posEnd)
{
	if (from < posBegin)
	{
		posBegin += size;
		posEnd   += size;
	}

	if (posBegin <= from && from + size <= posEnd)
	{
		posEnd += size;
	}

	if (from <= posEnd && posEnd < from + size)
	{
		ASSERT(posBegin <= from);
		posEnd += from + size - posEnd;
	}
}

void UpdateSwap::remove(CREF(ruint) from, CREF(ruint) to, REF(ruint) posBegin, REF(ruint) posEnd)
{
	if (to <= posBegin)
	{
		posBegin -= to - from;
		posEnd   -= to - from;
	}
	else if (posBegin <= from && to <= posEnd)
	{
		posEnd -= to - from;
	}
	else if (from < posEnd && posEnd < to)
	{
		posEnd = from;
	}
}

CLOSE_RDO_CONVERTER_NAMESPACE

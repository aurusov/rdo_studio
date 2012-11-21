/*!
  \copyright (c) RDO-Team, 2011
  \file      update.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      29.10.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/update/update.h"
#include "converter/smr2rdox/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- DocUpdate
// --------------------------------------------------------------------------------
DocUpdate::DocUpdate(IDocument::Type file)
	: m_file(file)
{
	if (m_file == IDocument::UNDEFINED)
	{
		m_file = getCurrentType();
	}
}

IDocument::Type DocUpdate::getCurrentType() const
{
	switch (Converter::getFileToParse())
	{
	case rdo::converter::smr2rdox::PAT_IN: return IDocument::PAT;
	case rdo::converter::smr2rdox::RTP_IN: return IDocument::RTP;
	case rdo::converter::smr2rdox::RSS_IN: return IDocument::RSS;
	case rdo::converter::smr2rdox::OPR_IN: return IDocument::OPR;
	case rdo::converter::smr2rdox::FRM_IN: return IDocument::FRM;
	case rdo::converter::smr2rdox::FUN_IN: return IDocument::FUN;
	case rdo::converter::smr2rdox::DPT_IN: return IDocument::DPT;
	case rdo::converter::smr2rdox::SMR_IN: return IDocument::SMR;
	case rdo::converter::smr2rdox::PMD_IN: return IDocument::PMD;
	case rdo::converter::smr2rdox::PMV_IN: return IDocument::PMV;
	case rdo::converter::smr2rdox::TRC_IN: return IDocument::TRC;
	default: NEVER_REACH_HERE;
	}
	NEVER_REACH_HERE;
	return IDocument::TRC;
}

// --------------------------------------------------------------------------------
// -------------------- UpdateInsert
// --------------------------------------------------------------------------------
UpdateInsert::UpdateInsert(CREF(Position) pos, CREF(tstring) value, IDocument::Type file)
	: DocUpdate(file )
	, m_pos    (pos  )
	, m_value  (value)
{}

void UpdateInsert::dump(REF(LPIDocument) pDocument) const
{
	tstring log = rdo::format(_T("update insert <%d>, %d, '%s', dump: '%s'\n")
		, m_file
		, m_pos.get()
		, m_value.c_str()
		, pDocument->get(m_file, m_pos.get(), m_pos.get()).c_str()
	);
	TRACE(log.c_str());
}

void UpdateInsert::apply(REF(LPIDocument) pDocument) const
{
	pDocument->insert(m_file, m_pos.get(), m_value);
}

void UpdateInsert::insert(IDocument::Type type, CREF(Position) to, ruint size)
{
	if (m_file != type)
		return;

	ASSERT(to <= m_pos || to >= m_pos + m_value.length());

	if (to <= m_pos)
	{
		m_pos += size;
	}
}

void UpdateInsert::remove(IDocument::Type type, CREF(Position) from, CREF(Position) to)
{
	if (m_file != type)
		return;

	ASSERT(to <= m_pos || (from >= m_pos + m_value.length() && to > m_pos + m_value.length()));

	if (to <= m_pos)
	{
		m_pos -= to - from;
	}
}

// --------------------------------------------------------------------------------
// -------------------- UpdateDelete
// --------------------------------------------------------------------------------
UpdateDelete::UpdateDelete(CREF(Position) posFrom, CREF(Position) posTo)
	: m_posFrom(posFrom)
	, m_posTo  (posTo  )
{
	ASSERT(m_posFrom < m_posTo);
}

void UpdateDelete::dump(REF(LPIDocument) pDocument) const
{
	tstring log = rdo::format(_T("update delete <%d>, [%d - %d), dump: '%s'\n")
		, m_file
		, m_posFrom.get()
		, m_posTo.get()
		, pDocument->get(m_file, m_posFrom.get(), m_posTo.get()).c_str()
	);
	TRACE(log.c_str());
}

void UpdateDelete::apply(REF(LPIDocument) pDocument) const
{
	pDocument->remove(m_file, m_posFrom.get(), m_posTo.get());
}

void UpdateDelete::insert(IDocument::Type type, CREF(Position) to, ruint size)
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

void UpdateDelete::remove(IDocument::Type type, CREF(Position) from, CREF(Position) to)
{
	if (m_file != type)
		return;

	ASSERT(to <= m_posFrom || (from >= m_posTo && to >= m_posTo));

	if (to <= m_posFrom)
	{
		m_posFrom -= to - from;
		m_posTo   -= to - from;
	}
}

// --------------------------------------------------------------------------------
// -------------------- UpdateReplace
// --------------------------------------------------------------------------------
UpdateReplace::UpdateReplace(CREF(Position) posFrom, CREF(Position) posTo, CREF(tstring) value, IDocument::Type file)
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

void UpdateReplace::insert(IDocument::Type type, CREF(Position) to, ruint size)
{
	pDelete->insert(type, to, size);
	pInsert->insert(type, to, size);
}

void UpdateReplace::remove(IDocument::Type type, CREF(Position) from, CREF(Position) to)
{
	pDelete->remove(type, from, to);
	pInsert->remove(type, from, to);
}

// --------------------------------------------------------------------------------
// -------------------- UpdateMove
// --------------------------------------------------------------------------------
UpdateMove::UpdateMove(CREF(Position) posFromBegin, CREF(Position) posFromEnd, CREF(Position) posTo, IDocument::Type fileTo, IDocument::Type fileFrom)
	: DocUpdate     (fileTo      )
	, m_posFromBegin(posFromBegin)
	, m_posFromEnd  (posFromEnd  )
	, m_posTo       (posTo       )
	, m_fileFrom    (fileFrom    )
{
	if (static_cast<int>(m_fileFrom) == static_cast<int>(rdo::converter::smr2rdox::UNDEFINED_OUT))
	{
		m_fileFrom = getCurrentType();
	}
}

void UpdateMove::dump(REF(LPIDocument) pDocument) const
{
	tstring log = rdo::format(_T("update move <%d->%d>, [%d - %d) '%s', %d\n")
		, m_fileFrom
		, m_file
		, m_posFromBegin.get()
		, m_posFromEnd.get()
		, pDocument->get(m_fileFrom, m_posFromBegin.get(), m_posFromEnd.get()).c_str()
		, m_posTo.get()
	);
	TRACE(log.c_str());
}

void UpdateMove::apply(REF(LPIDocument) pDocument) const
{
	tstring cut = pDocument->get(m_fileFrom, m_posFromBegin.get(), m_posFromEnd.get());
	pDocument->remove(m_fileFrom, m_posFromBegin.get(), m_posFromEnd.get());

	Position pos = m_posTo;
	if (pos.real())
	{
		if (m_posFromEnd < pos)
		{
			//! Удалили перед собой, сдвинемся к началу
			pos -= m_posFromEnd - m_posFromBegin;
		}
	}
	pDocument->insert(m_file, pos.get(), cut);
}

void UpdateMove::insert(IDocument::Type type, CREF(Position) to, ruint size)
{
	if (to.end())
		return;

	if (m_fileFrom == type)
	{
		if (to.begin())
		{
			//! Вставка до, сдвинемся к концу
			m_posFromBegin += size;
			m_posFromEnd   += size;
		}
		else
		{
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
		}
	}

	if (m_file == type)
	{
		if (m_posTo.real())
		{
			if (to.begin() || to < m_posTo)
			{
				//! Вставка до, сдвинемся к концу
				m_posTo += size;
			}
		}
	}
}

void UpdateMove::remove(IDocument::Type type, CREF(Position) from, CREF(Position) to)
{
	if (m_fileFrom == type)
	{
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
	}

	if (m_file == type)
	{
		if (m_posTo.real())
		{
			if (to < m_posTo)
			{
				//! Удаление до, сдвинемся к началу
				m_posTo -= to - from;
			}
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- UpdateSwap
// --------------------------------------------------------------------------------
UpdateSwap::UpdateSwap(CREF(Position) pos1Begin, CREF(Position) pos1End, CREF(Position) pos2Begin, CREF(Position) pos2End, IDocument::Type file)
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
		, m_pos1Begin.get()
		, m_pos1End.get()
		, m_pos2Begin.get()
		, m_pos2End.get()
		, pDocument->get(m_file, m_pos1Begin.get(), m_pos1End.get()).c_str()
		, pDocument->get(m_file, m_pos2Begin.get(), m_pos2End.get()).c_str()
	);
	TRACE(log.c_str());
}

void UpdateSwap::apply(REF(LPIDocument) pDocument) const
{
	//! Запомним значения
	tstring cut1 = pDocument->get(m_file, m_pos1Begin.get(), m_pos1End.get());
	tstring cut2 = pDocument->get(m_file, m_pos2Begin.get(), m_pos2End.get());

	//! Удалим первый интервал
	pDocument->remove(m_file, m_pos1Begin.get(), m_pos1End.get());

	Position pos2Begin = m_pos2Begin;
	Position pos2End   = m_pos2End;
	if (m_pos1End <= pos2Begin)
	{
		//! Удаление перед вторым интервалом, сдвинемся к началу
		pos2Begin -= m_pos1End - m_pos1Begin;
		pos2End   -= m_pos1End - m_pos1Begin;
	}

	//! Удалим второй интервал
	pDocument->remove(m_file, pos2Begin.get(), pos2End.get());

	Position pos1Begin = m_pos1Begin;
	Position pos1End   = m_pos1End;
	if (pos2End <= pos1Begin)
	{
		//! Удаление перед первым интервалом, сдвинемся к началу
		pos1Begin -= pos2End - pos2Begin;
		pos1End   -= pos2End - pos2Begin;
	}

	//! Вставка первого буффера на место второго
	pDocument->insert(m_file, pos2Begin.get(), cut1);

	if (pos2Begin <= pos1Begin)
	{
		//! Вставка перед первым интервалом, сдвинемся к концу
		pos1Begin += cut1.length();
		pos1End   += cut1.length();
	}

	//! Вставка второго буффера на место первого
	pDocument->insert(m_file, pos1Begin.get(), cut2);
}

void UpdateSwap::insert(IDocument::Type type, CREF(Position) to, ruint size)
{
	if (m_file != type)
		return;

	insert(to, size, m_pos1Begin, m_pos1End);
	insert(to, size, m_pos2Begin, m_pos2End);
}

void UpdateSwap::remove(IDocument::Type type, CREF(Position) from, CREF(Position) to)
{
	if (m_file != type)
		return;

	remove(from, to, m_pos1Begin, m_pos1End);
	remove(from, to, m_pos2Begin, m_pos2End);
}

void UpdateSwap::insert(CREF(Position) from, CREF(ruint) size, REF(Position) posBegin, REF(Position) posEnd)
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

void UpdateSwap::remove(CREF(Position) from, CREF(Position) to, REF(Position) posBegin, REF(Position) posEnd)
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

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

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
		switch (Converter::getFileToParse())
		{
		case rdoModelObjectsConvertor::PAT_IN: m_fileTo = rdoModelObjectsConvertor::PAT_OUT; break;
		case rdoModelObjectsConvertor::RTP_IN: m_fileTo = rdoModelObjectsConvertor::RTP_OUT; break;
		case rdoModelObjectsConvertor::RSS_IN: m_fileTo = rdoModelObjectsConvertor::RSS_OUT; break;
		case rdoModelObjectsConvertor::OPR_IN: NEVER_REACH_HERE;
		case rdoModelObjectsConvertor::FRM_IN: m_fileTo = rdoModelObjectsConvertor::FRM_OUT; break;
		case rdoModelObjectsConvertor::FUN_IN: m_fileTo = rdoModelObjectsConvertor::FUN_OUT; break;
		case rdoModelObjectsConvertor::DPT_IN: m_fileTo = rdoModelObjectsConvertor::DPT_OUT; break;
		case rdoModelObjectsConvertor::SMR_IN: m_fileTo = rdoModelObjectsConvertor::SMR_OUT; break;
		case rdoModelObjectsConvertor::PMD_IN: m_fileTo = rdoModelObjectsConvertor::PMD_OUT; break;
		case rdoModelObjectsConvertor::PMV_IN: m_fileTo = rdoModelObjectsConvertor::PMV_OUT; break;
		case rdoModelObjectsConvertor::TRC_IN: m_fileTo = rdoModelObjectsConvertor::TRC_OUT; break;
		default: NEVER_REACH_HERE;
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- UpdateInsert
// ----------------------------------------------------------------------------
UpdateInsert::UpdateInsert(ruint pos, CREF(tstring) value, IDocument::Type file)
	: DocUpdate(file )
	, m_pos    (pos  )
	, m_value  (value)
{}

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
// ---------- UpdateMove
// ----------------------------------------------------------------------------
UpdateMove::UpdateMove(ruint posFromBegin, ruint posFromEnd, ruint posTo, IDocument::Type fileTo)
	: DocUpdate()
	, m_posFromBegin(posFromBegin)
	, m_posFromEnd  (posFromEnd  )
	, m_posTo       (posTo       )
{}

void UpdateMove::apply(REF(LPIDocument) pDocument) const
{
}

void UpdateMove::insert(IDocument::Type type, ruint to, ruint size)
{
}

void UpdateMove::remove(IDocument::Type type, ruint from, ruint to)
{
}

CLOSE_RDO_CONVERTER_NAMESPACE

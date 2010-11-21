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
DocUpdate::DocUpdate(Document::Type fileTo)
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
UpdateInsert::UpdateInsert(std::istream::pos_type pos, CREF(tstring) value, Document::Type file)
	: DocUpdate(file )
	, m_pos    (pos  )
	, m_value  (value)
{}

void UpdateInsert::apply(REF(LPDocument) pDocument, REF(std::istream) streamIn) const
{
	std::istream::pos_type pos = streamIn.tellg();
	ASSERT(pos <= m_pos);

	while (pos < m_pos)
	{
		char byte;
		streamIn.get(byte);
		pDocument->write(m_fileTo, &byte, 1);
		pos = streamIn.tellg();
	}

	pDocument->write(m_fileTo, m_value.c_str(), m_value.length());
}

// ----------------------------------------------------------------------------
// ---------- UpdateDelete
// ----------------------------------------------------------------------------
UpdateDelete::UpdateDelete(std::istream::pos_type posFrom, std::istream::pos_type posTo)
	: m_posFrom(posFrom)
	, m_posTo  (posTo  )
{
	ASSERT(m_posFrom < m_posTo);
}

void UpdateDelete::apply(REF(LPDocument) pDocument, REF(std::istream) streamIn) const
{
	std::istream::pos_type pos = streamIn.tellg();
	ASSERT(pos <= m_posFrom);

	while (pos < m_posFrom)
	{
		char byte;
		streamIn.get(byte);
		pDocument->write(m_fileTo, &byte, 1);
		pos = streamIn.tellg();
	}

	streamIn.seekg(m_posTo);
}

// ----------------------------------------------------------------------------
// ---------- UpdateFlush
// ----------------------------------------------------------------------------
UpdateFlush::UpdateFlush()
{}

void UpdateFlush::apply(REF(LPDocument) pDocument, REF(std::istream) streamIn) const
{
	while (!streamIn.eof())
	{
		char byte;
		streamIn.get(byte);
		pDocument->write(m_fileTo, &byte, 1);
	}
}

CLOSE_RDO_CONVERTER_NAMESPACE

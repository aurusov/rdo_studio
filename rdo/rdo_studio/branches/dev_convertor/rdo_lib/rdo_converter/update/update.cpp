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

DocUpdate::DocUpdate(rdoModelObjectsConvertor::RDOFileType fileTo)
	: m_fileTo(fileTo)
{
	if (m_fileTo == rdoModelObjectsConvertor::UNDEFINED)
	{
		m_fileTo = Converter::getFileToParse();
	}
}

UpdateInsert::UpdateInsert(std::istream::pos_type pos, CREF(tstring) value, rdoModelObjectsConvertor::RDOFileType file)
	: DocUpdate(file )
	, m_pos    (pos  )
	, m_value  (value)
{}

void UpdateInsert::apply(REF(LPDocument) pDocument, REF(std::istream) streamIn) const
{
	std::istream::pos_type pos = streamIn.tellg();
	ASSERT(pos <= m_pos);

	REF(std::ofstream) streamOut = pDocument->getStream(m_fileTo);

	while (pos < m_pos)
	{
		char byte;
		streamIn.get(byte);
		streamOut.write(&byte, 1);
		pos = streamIn.tellg();
	}

	streamOut.write(m_value.c_str(), m_value.length());
}

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

	REF(std::ofstream) streamOut = pDocument->getStream(m_fileTo);

	while (pos < m_posFrom)
	{
		char byte;
		streamIn.get(byte);
		streamOut.write(&byte, 1);
		pos = streamIn.tellg();
	}

	streamIn.seekg(m_posTo);
}

CLOSE_RDO_CONVERTER_NAMESPACE

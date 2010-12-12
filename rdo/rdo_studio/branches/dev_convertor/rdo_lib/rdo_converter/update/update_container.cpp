/*
 * copyright: (c) RDO-Team, 2010
 * filename : update_container.cpp
 * author   : Урусов Андрей
 * date     : 12.12.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/update/update_container.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

UpdateContainer::UpdateContainer()
{}

UpdateContainer::~UpdateContainer()
{}

void UpdateContainer::append(CREF(LPDocUpdate) pDocUpdate)
{
	ASSERT(pDocUpdate);
	m_docUpdateList.push_back(pDocUpdate);
}

CREF(UpdateContainer::DocUpdateList) UpdateContainer::updateList() const
{
	return m_docUpdateList;
}

CLOSE_RDO_CONVERTER_NAMESPACE

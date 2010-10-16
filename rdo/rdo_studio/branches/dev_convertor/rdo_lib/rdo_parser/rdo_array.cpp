/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.cpp
 * author   : Чирков Михаил
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_array.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

 //----------------------------------------------------------------------------
 //---------- RDOArrayType
 //----------------------------------------------------------------------------
RDOArrayType::RDOArrayType()
	: RDOType(rdoRuntime::g_unknow)
{
	m_pType = rdo::Factory<rdoRuntime::RDOArrayType>::create();
}

RDOArrayType::~RDOArrayType()
{}

CLOSE_RDO_PARSER_NAMESPACE

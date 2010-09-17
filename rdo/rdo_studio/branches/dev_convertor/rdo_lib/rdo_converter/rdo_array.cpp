/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.cpp
 * author   : Чирков Михаил
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_array.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdoparser_error.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

 //----------------------------------------------------------------------------
 //---------- RDOArrayType
 //----------------------------------------------------------------------------
RDOArrayType::RDOArrayType()
	: RDOType(&rdoRuntime::g_unknow)
{
	m_type = new rdoRuntime::RDOArrayType(Converter::s_converter()->runtime());
}

RDOArrayType::~RDOArrayType()
{
}

CLOSE_RDO_CONVERTER_NAMESPACE

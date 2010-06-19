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
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

 //----------------------------------------------------------------------------
 //---------- RDOArrayType
 //----------------------------------------------------------------------------
RDOArrayType::RDOArrayType(CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) src_info)
	: RDOType          (&rdoRuntime::g_unknow)
	, RDOParserSrcInfo (src_info             )
	, m_pType          (pType                )
{
	ASSERT(m_pType);
	setSrcText(name());
	m_type = new rdoRuntime::RDOArrayType(RDOParser::s_parser()->runtime(), m_pType->type());
}

RDOArrayType::~RDOArrayType()
{}

inline tstring RDOArrayType::name() const
{
	return rdo::format(_T("array<%s>"), m_pType->name().c_str());
}

LPRDOType RDOArrayType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->typeID())
	{
	case rdoRuntime::RDOType::t_array:
		{
			LPRDOArrayType pArray(const_cast<PTR(RDOArrayType)>(this));
			LPRDOArrayType from(const_cast<PTR(RDOArrayType)>(static_cast<CPTR(RDOArrayType)>(from.get())));
			if(pArray->arrayType()->typeID() == rdoRuntime::RDOType::t_enum)
			{
				return pArray;
				break;
			}
			//! Это один и тот же тип
			if (pArray->arrayType()->type_cast(from->arrayType(), from->src_info(), pArray->src_info(), from->src_info()))
			{
				return pArray;
			}
			rdoParse::g_error().push_done();
			break;
		}
	default:
		{
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("Несоответствие размерности массива")));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
			break;
		}
	}
	return rdo::smart_ptr_null();
}

RDOValue RDOArrayType::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch(from->typeID())
	{
	case rdoRuntime::RDOType::t_array:
		{
			LPRDOArrayType pArrayT(const_cast<PTR(RDOArrayType)>(this));
			rdoRuntime::RDOArrayValue rArrayV = from->getArray();
			if(pArrayT->arrayType()->typeID() == rdoRuntime::RDOType::t_enum) break;
			for(rdoRuntime::RDOArrayValue::ArrayValue::const_iterator it = rArrayV.m_containerBegin(); it != rArrayV.m_containerEnd(); ++it)
			{
				pArrayT->arrayType()->value_cast(RDOValue(*it, from.type(), src_info), to_src_info, src_info);
			}
			break;
		}
	default:
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("Несоответствие размерности массива")));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
		break;
	}
	return from;
}

PTR(rdoRuntime::RDOCalc) RDOArrayType::calc_cast(PTR(rdoRuntime::RDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

RDOValue RDOArrayType::get_default() const
{
	LPRDOArrayType ArrayType(const_cast<PTR(RDOArrayType)>(this));
	return RDOValue(rdoRuntime::RDOArrayValue(rdoRuntime::RDOArrayType(NULL, ArrayType->type())), ArrayType, RDOParserSrcInfo());
}

void RDOArrayType::writeModelStructure(REF(std::ostream) stream) const
{
	CPTR(RDOArrayType) pArray(const_cast<PTR(RDOArrayType)>(this));
	stream << (*pArray).name()<< std::endl;
}

CREF(LPRDOType) RDOArrayType::arrayType() const
{
	return m_pType;
}

CREF(LPRDOType) RDOArrayType::getFirstType()
{
	if(m_pType->typeID() == rdoRuntime::RDOType::t_array)
	{
		LPRDOArrayType ArrayType(const_cast<PTR(RDOArrayType)>(static_cast<CPTR(RDOArrayType)>(m_pType.get())));
		return ArrayType->getFirstType();
	}
	else return m_pType;
}

void RDOArrayType::dinamicItemCast(CREF(RDOValue) value)
{
	switch(value->typeID())
	{
	case(rdoRuntime::RDOType::t_array):  break;
	case(rdoRuntime::RDOType::t_unknow): break;
	default:
		{
			getFirstType()->type_cast(value.type(), value.src_info(), src_info(), value.src_info());
			break;
		}
	}
}


//----------------------------------------------------------------------------
//---------- RDOArrayValue
//----------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(LPRDOArrayType pArrayType)
{
	m_arrayType = pArrayType;
}

RDOArrayValue::~RDOArrayValue()
{}

void RDOArrayValue::insert_array(CREF(RDOValue) value)
{
	NEVER_REACH_HERE;
	//LPRDOArrayType FirstArrayType = RDOParser::s_parser()->getLastArrayType();
	//FirstArrayType->dinamicItemCast(value);
	//m_arrayValue.push_back(value);
}

CREF(LPRDOArrayType) RDOArrayValue::getArrayType() const
{
	return m_arrayType;
}


rdoRuntime::RDOValue RDOArrayValue::getRArray() const
{
	rdoRuntime::RDOArrayValue arrayValue(*m_arrayType->__array());
	STL_FOR_ALL_CONST(ArrayValue, m_arrayValue, it)
	{
		arrayValue.insert_array(it->value());
	}
	rdoRuntime::RDOValue value(arrayValue);
	return value;
}

CLOSE_RDO_PARSER_NAMESPACE

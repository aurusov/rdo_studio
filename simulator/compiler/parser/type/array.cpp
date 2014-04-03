/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/type/array.cpp
  \author    Чирков Михаил
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/array.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

 //----------------------------------------------------------------------------
 //---------- RDOArrayType
 //----------------------------------------------------------------------------
RDOArrayType::RDOArrayType(CREF(LPTypeInfo) pItemType, CREF(RDOParserSrcInfo) src_info)
	: RuntimeWrapperType(rdo::Factory<rdo::runtime::RDOArrayType>::create(pItemType->type()->type()))
	, RDOParserSrcInfo  (src_info )
	, m_pItemType       (pItemType)
{
	ASSERT(m_pType  );
	ASSERT(pItemType);
	setSrcText(name());
}

RDOArrayType::~RDOArrayType()
{}

tstring RDOArrayType::name() const
{
	return rdo::format("array<%s>", m_pItemType->type()->name().c_str());
}

LPRDOType RDOArrayType::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(from_src_info);

	ASSERT(pFrom);

	LPRDOArrayType pFromArrayType = pFrom.object_dynamic_cast<RDOArrayType>();
	if (pFromArrayType)
	{
		LPRDOArrayType pThisArrayType(const_cast<PTR(RDOArrayType)>(this));

		if (pThisArrayType->getItemType()->type()->type_cast(pFromArrayType->getItemType()->type(), pFromArrayType->src_info(), pThisArrayType->src_info(), pFromArrayType->src_info()))
		{
			//! Это один и тот же тип
			return pThisArrayType;
		}

		parser::g_error().push_only(src_info,    rdo::format("Несоответствие размерности массива"));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}

	parser::g_error().push_only(src_info,    rdo::format("Ожидается тип массива, найдено: %s", from_src_info.src_text().c_str()));
	parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
	parser::g_error().push_done();

	return NULL;
}

LPRDOValue RDOArrayType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOArrayType pFromArrayType = pFrom->typeInfo()->type().object_dynamic_cast<RDOArrayType>();
	if (pFromArrayType)
	{
		LPRDOArrayType  pThisArrayType(const_cast<PTR(RDOArrayType)>(this));
		LPRDOArrayValue pThisArrayValue = rdo::Factory<RDOArrayValue>::create(pThisArrayType);
		ASSERT(pThisArrayValue);

		rdo::runtime::LPRDOArrayValue pFromArrayValue = pFrom->get<RDOArrayValue>()->createRuntimeValue();
		ASSERT(pFromArrayValue);

		for (rdo::runtime::LPRDOArrayIterator it = pFromArrayValue->begin(); !it->equal(pFromArrayValue->end()); it->next())
		{
			LPRDOValue pItemValue = rdo::Factory<RDOValue>::create(it->getValue(), src_info, pThisArrayType->getItemType());
			ASSERT(pItemValue);
			pThisArrayValue->insertItem(pThisArrayType->getItemType()->type()->value_cast(pItemValue, to_src_info, src_info));
		}
		return rdo::Factory<RDOValue>::create(pThisArrayType->typeInfo(), pThisArrayValue, pFrom->src_info());
	}

	parser::g_error().push_only(src_info,    rdo::format("Ожидается массив, найдено: %s", pFrom->src_text().c_str()));
	parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
	parser::g_error().push_done();

	return NULL;
}

rdo::runtime::LPRDOCalc RDOArrayType::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RuntimeWrapperType::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOArrayType::get_default() const
{
	return rdo::runtime::RDOValue(getRuntimeArrayType(), rdo::Factory<rdo::runtime::RDOArrayValue>::create(getRuntimeArrayType()));
}

void RDOArrayType::writeModelStructure(std::ostream& stream) const
{
	stream << "A ";
	m_pItemType->type()->writeModelStructure(stream);
}

CREF(LPTypeInfo) RDOArrayType::getItemType() const
{
	return m_pItemType;
}

rdo::runtime::LPRDOArrayType RDOArrayType::getRuntimeArrayType() const
{
	return m_pType.object_static_cast<rdo::runtime::RDOArrayType>();
}

LPTypeInfo RDOArrayType::typeInfo() const
{
	LPTypeInfo pType = rdo::Factory<TypeInfo>::create(
		const_cast<PTR(RDOArrayType)>(this),
		src_info()
	);
	ASSERT(pType);
	return pType;
}

CLOSE_RDO_PARSER_NAMESPACE

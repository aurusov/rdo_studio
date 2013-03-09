/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_enum.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_enum.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
	: RDOType(rdo::runtime::g_unknow)
{
	m_pType = rdo::Factory<rdo::runtime::RDOEnumType>::create();
	ASSERT(m_pType);
}

RDOEnumType::RDOEnumType(CREF(rdo::runtime::LPRDOEnumType) pEnumType)
	: RDOType(rdo::runtime::g_unknow)
{
	rdo::runtime::LPRDOEnumType pType = rdo::Factory<rdo::runtime::RDOEnumType>::create();
	m_pType = pType;
	ASSERT(m_pType);

	rdo::runtime::RDOEnumType::Enums::const_iterator it = pEnumType->begin();
	while (it != pEnumType->end())
	{
		pType->add(*it);
	}
}

RDOEnumType::~RDOEnumType()
{}

tstring RDOEnumType::name() const
{
	tstring str = _T("(");
	rdo::runtime::RDOEnumType::const_iterator it = getEnums()->begin();
	while (it != getEnums()->end())
	{
		str += *it;
		++it;
		if (it != getEnums()->end())
		{
			str += _T(", ");
		}
	}
	str += _T(")");
	return str;
}

LPRDOType RDOEnumType::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType__int::t_enum:
		{
			LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
			//! Это один и тот же тип
			if (pEnum == pFrom)
				return pEnum;

			//! Типы разные, сгенерим ошибку
			if (pEnum.compare(pFrom.object_static_cast<RDOEnumType>()))
			{
				rdo::converter::smr2rdox::g_error().push_only(src_info,     _T("Используются различные перечислимые типы с одинаковыми значениями"));
				rdo::converter::smr2rdox::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdo::converter::smr2rdox::g_error().push_only(src_info,     _T("и"));
				rdo::converter::smr2rdox::g_error().push_only(from_src_info, from_src_info.src_text());
				rdo::converter::smr2rdox::g_error().push_only(src_info,     _T("Возможно, удобнее использовать первый из них как перечислимый, а второй как such_as на него, тогда параметры можно будет сравнивать и присваивать"));
			}
			else
			{
				rdo::converter::smr2rdox::g_error().push_only(src_info,     _T("Несоответствие перечислимых типов"));
				rdo::converter::smr2rdox::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdo::converter::smr2rdox::g_error().push_only(src_info,     _T("и"));
				rdo::converter::smr2rdox::g_error().push_only(from_src_info, from_src_info.src_text());
			}
			rdo::converter::smr2rdox::g_error().push_done();
			break;
		}
		case rdo::runtime::RDOType::t_string       :
		case rdo::runtime::RDOType::t_identificator:
		{
			if (getEnums()->exist(from_src_info.src_text()))
			{
				LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
				return pEnum;
			}
			rdo::converter::smr2rdox::g_error().error(src_info, rdo::format(_T("Значение '%s' не является элементом перечислимого типа %s"), from_src_info.src_text().c_str(), to_src_info.src_text().c_str()));
			break;
		}
		default:
			rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format(_T("Ожидается значение перечислимого типа, найдено: %s"), from_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format(_T("См. тип: %s"), to_src_info.src_text().c_str()));
			rdo::converter::smr2rdox::g_error().push_done();
			break;
	}
	return NULL;
}

LPRDOValue RDOEnumType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	LPRDOValue pToValue;
	LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
	try
	{
		switch (pFrom->typeID())
		{
		case rdo::runtime::RDOType::t_identificator:
			pToValue = (getEnums()->findEnum(pFrom->value().getIdentificator()) != rdo::runtime::RDOEnumType::END) ?
				rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(getEnums(), pFrom->value().getIdentificator()), pEnum, pFrom->src_info()) :
				rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__unknow>::create().object_parent_cast<RDOType>(), pFrom->src_info());
			break;

		case rdo::runtime::RDOType::t_string:
			pToValue = (getEnums()->findEnum(pFrom->value().getAsString()) != rdo::runtime::RDOEnumType::END) ?
				rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(getEnums(), pFrom->value().getAsString()), pEnum, pFrom->src_info()) :
				rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__unknow>::create().object_parent_cast<RDOType>(), pFrom->src_info());
			break;

		case rdo::runtime::RDOType::t_enum:
			if (m_pType == pFrom->type()->type())
				pToValue = rdo::Factory<RDOValue>::create(pFrom);
			break;

		default:
			break;
		}
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdo::converter::smr2rdox::g_error().push_only(src_info,    rdo::format(_T("Неверное значение параметра перечислимого типа: %s"), pFrom->src_info().src_text().c_str()));
		rdo::converter::smr2rdox::g_error().push_only(to_src_info, rdo::format(_T("Возможные значения: %s"), name().c_str()));
		rdo::converter::smr2rdox::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOEnumType::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

LPRDOValue RDOEnumType::get_default() const
{
	LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
	return rdo::Factory<RDOValue>::create(rdo::runtime::RDOValue(getEnums(), 0), pEnum, RDOParserSrcInfo());
}

void RDOEnumType::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "E " << getEnums()->getValues().size() << std::endl;
	for (ruint i = 0; i < getEnums()->getValues().size(); i++)
	{
		stream << "    " << i << " " << getEnums()->getValues().at(i) << std::endl;
	}
}

void RDOEnumType::add(CREF(LPRDOValue) pNext)
{
	ASSERT(pNext);

	if (getEnums()->findEnum(pNext->value().getAsString()) != rdo::runtime::RDOEnumType::END)
	{
		rdo::converter::smr2rdox::g_error().error(pNext->src_info(), rdo::format(_T("Значение перечислимого типа уже существует: %s"), pNext->src_text().c_str()));
	}
	getEnums()->add(pNext->value().getAsString());
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

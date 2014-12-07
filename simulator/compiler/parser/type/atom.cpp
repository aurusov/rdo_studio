// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/atom.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_PARSER
// --------------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type, TypeName) \
std::string RDOType__##Type::name() const        \
{                                                \
	return TypeName;                             \
}

DECLARE_ATOM_TYPE_PARSER(unknow,        "unknow"       );
DECLARE_ATOM_TYPE_PARSER(void,          "void"         );
DECLARE_ATOM_TYPE_PARSER(identificator, "identificator");
DECLARE_ATOM_TYPE_PARSER(int,           "integer"      );
DECLARE_ATOM_TYPE_PARSER(real,          "real"         );
DECLARE_ATOM_TYPE_PARSER(bool,          "bool"         );
DECLARE_ATOM_TYPE_PARSER(string,        "string"       );

// --------------------------------------------------------------------------------
// -------------------- RDOType__unknow
// --------------------------------------------------------------------------------
LPIType RDOType__unknow::type_cast(const LPIType& /*pFrom*/, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& src_info) const
{
	parser::g_error().error(src_info, rdo::format("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к неизвестному типу", from_src_info.src_text().c_str()));
	return NULL;
}

LPRDOValue RDOType__unknow::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к неизвестному типу", pFrom->src_info().src_text().c_str()));
	parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
	parser::g_error().push_done();
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__unknow::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	return pCalc;
}

rdo::runtime::RDOValue RDOType__unknow::get_default() const
{
	return rdo::runtime::RDOValue();
}

void RDOType__unknow::writeModelStructure(std::ostream& /*stream*/) const
{
	parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать неизвестный тип в отчет");
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__void
// --------------------------------------------------------------------------------
LPIType RDOType__void::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& src_info) const
{
	if (pFrom.object_dynamic_cast<RDOType>()->typeID() == rdo::runtime::RDOType::t_void)
	{
		return rdo::Factory<RDOType__void>::create();
	}
	parser::g_error().error(src_info, rdo::format("Невозможно преобразовать '%s' к void", from_src_info.src_text().c_str()));
	return NULL;
}

LPRDOValue RDOType__void::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	if (pFrom->value().type()->typeID() == rdo::runtime::RDOType::t_void)
	{
		return rdo::Factory<RDOValue>::create(pFrom);
	}

	parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к void-типу", pFrom->src_info().src_text().c_str()));
	parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
	parser::g_error().push_done();
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__void::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	return pCalc;
}

rdo::runtime::RDOValue RDOType__void::get_default() const
{
	return rdo::runtime::RDOValue();
}

void RDOType__void::writeModelStructure(std::ostream& /*stream*/) const
{
	parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать void-тип в отчет");
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__int
// --------------------------------------------------------------------------------
LPIType RDOType__int::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
	{
		case rdo::runtime::RDOType::t_int :
			return rdo::Factory<RDOType__int>::create();
		case rdo::runtime::RDOType::t_real:
			parser::g_error().warning(src_info, rdo::format("Преобразование '%s' в '%s', возможна потеря данных", pFrom->name().c_str(), name().c_str()));
			return rdo::Factory<RDOType__real>::create();
		default:
			if (!from_src_info.src_text().empty())
			{
				parser::g_error().push_only(src_info, rdo::format("Ожидается целочисленное значение, найдено: %s", from_src_info.src_text().c_str()));
			}
			else
			{
				parser::g_error().push_only(from_src_info, rdo::format("Ожидается целочисленное значение, найдено: %s", pFrom->name().c_str()));
			}
			parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			parser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__int::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<RDOType__int*>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getInt(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (const rdo::runtime::RDOValueException&)
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к целому типу", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__int::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	if (pType.object_dynamic_cast<RDOType>()->typeID() == rdo::runtime::RDOType::t_real)
	{
		return rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(pCalc->srcInfo().src_pos(), pCalc);
	}
	return pCalc;
}

rdo::runtime::RDOValue RDOType__int::get_default() const
{
	return rdo::runtime::RDOValue(0);
}

void RDOType__int::writeModelStructure(std::ostream& stream) const
{
	stream << "I" << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__real
// --------------------------------------------------------------------------------
LPIType RDOType__real::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
	{
		case rdo::runtime::RDOType::t_int :
		case rdo::runtime::RDOType::t_real:
			return rdo::Factory<RDOType__real>::create();
		default:
			parser::g_error().push_only(src_info,    rdo::format("Ожидается вещественное значение, найдено: %s", from_src_info.src_text().c_str()));
			parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			parser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__real::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<RDOType__real*>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getDouble(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (const rdo::runtime::RDOValueException&)
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к вещественному типу", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__real::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	return pCalc;
}

rdo::runtime::RDOValue RDOType__real::get_default() const
{
	return rdo::runtime::RDOValue(0.0);
}

void RDOType__real::writeModelStructure(std::ostream& stream) const
{
	stream << "R" << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__string
// --------------------------------------------------------------------------------
LPIType RDOType__string::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
	{
		case rdo::runtime::RDOType::t_string:
			return rdo::Factory<RDOType__string>::create();
		default:
			parser::g_error().push_only(src_info,    rdo::format("Ожидается строковое значение, найдено: %s", from_src_info.src_text().c_str()));
			parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			parser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__string::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<RDOType__string*>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getString(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (const rdo::runtime::RDOValueException&)
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к строковому типу", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__string::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	return pCalc;
}

rdo::runtime::RDOValue RDOType__string::get_default() const
{
	return rdo::runtime::RDOValue("");
}

void RDOType__string::writeModelStructure(std::ostream& stream) const
{
	stream << "S" << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__identificator
// --------------------------------------------------------------------------------
LPIType RDOType__identificator::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& /*to_src_info*/, const RDOParserSrcInfo& src_info) const
{
	switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
	{
	case rdo::runtime::RDOType::t_identificator:
		return rdo::Factory<RDOType__identificator>::create();
	default:
		parser::g_error().error(src_info, rdo::format("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к идентификатору", from_src_info.src_text().c_str()));
	}
	return NULL;
}

LPRDOValue RDOType__identificator::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->typeID())
	{
	case rdo::runtime::RDOType::t_identificator:
		return rdo::Factory<RDOValue>::create(pFrom);
	default:
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к типу идентификатор", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__identificator::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	return pCalc;
}

rdo::runtime::RDOValue RDOType__identificator::get_default() const
{
	return rdo::runtime::RDOValue("", rdo::runtime::g_identificator);
}

void RDOType__identificator::writeModelStructure(std::ostream& /*stream*/) const
{
	parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать тип идектификатор в отчет");
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__bool
// --------------------------------------------------------------------------------
LPIType RDOType__bool::type_cast(const LPIType& pFrom, const RDOParserSrcInfo& from_src_info, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	switch (pFrom.object_dynamic_cast<RDOType>()->typeID())
	{
		case rdo::runtime::RDOType::t_bool:
			return rdo::Factory<RDOType__bool>::create();
		default:
			parser::g_error().push_only(src_info,    rdo::format("Ожидается булевское значение, найдено: %s", from_src_info.src_text().c_str()));
			parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			parser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__bool::value_cast(const LPRDOValue& pFrom, const RDOParserSrcInfo& to_src_info, const RDOParserSrcInfo& src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<RDOType__bool*>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getBool(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (const rdo::runtime::RDOValueException&)
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к булевскому типу", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__bool::calc_cast(const rdo::runtime::LPRDOCalc& pCalc, const LPIType& pType) const
{
	return pCalc;
}

rdo::runtime::RDOValue RDOType__bool::get_default() const
{
	return rdo::runtime::RDOValue(false);
}

void RDOType__bool::writeModelStructure(std::ostream& stream) const
{
	stream << "B" << std::endl;
}

CLOSE_RDO_PARSER_NAMESPACE

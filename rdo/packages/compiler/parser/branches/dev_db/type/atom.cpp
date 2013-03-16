/*!
  \copyright (c) RDO-Team, 2011
  \file      atom.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.04.2011
  \brief     
  \indent    4T
*/

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
#include "simulator/runtime/headers/db/interface_db.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_PARSER
// --------------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type, TypeName) \
tstring RDOType__##Type::name() const            \
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
LPRDOType RDOType__unknow::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(pFrom      );
	UNUSED(to_src_info);

	parser::g_error().error(src_info, rdo::format("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к неизвестному типу", from_src_info.src_text().c_str()));
	return NULL;
}

LPRDOValue RDOType__unknow::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к неизвестному типу", pFrom->src_info().src_text().c_str()));
	parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
	parser::g_error().push_done();
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__unknow::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__unknow::get_default() const
{
	return rdo::runtime::RDOValue();
}

void RDOType__unknow::writeModelStructure(REF(rdo::ostream) stream) const
{
	UNUSED(stream);
	parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать неизвестный тип в отчет");
	NEVER_REACH_HERE;
}

void RDOType__unknow::serializeInDB(REF(IDB) db) const
{
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__void
// --------------------------------------------------------------------------------
LPRDOType RDOType__void::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(to_src_info);
	ASSERT(pFrom      );

	if (pFrom->type()->typeID() == rdo::runtime::RDOType::t_void)
	{
		return rdo::Factory<RDOType__void>::create();
	}
	parser::g_error().error(src_info, rdo::format("Невозможно преобразовать '%s' к void", from_src_info.src_text().c_str()));
	return NULL;
}

LPRDOValue RDOType__void::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
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

rdo::runtime::LPRDOCalc RDOType__void::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__void::get_default() const
{
	return rdo::runtime::RDOValue();
}

void RDOType__void::writeModelStructure(REF(rdo::ostream) stream) const
{
	UNUSED(stream);
	parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать void-тип в отчет");
	NEVER_REACH_HERE;
}

void RDOType__void::serializeInDB(REF(IDB) db) const
{
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__int
// --------------------------------------------------------------------------------
LPRDOType RDOType__int::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_int :
			return rdo::Factory<RDOType__int>::create();
		case rdo::runtime::RDOType::t_real:
			parser::g_error().warning(src_info, rdo::format("Преобразование '%s' в '%s', возможна потеря данных", pFrom->name().c_str(), name().c_str()));
			return rdo::Factory<RDOType__real>::create();
		default:
			parser::g_error().push_only(src_info,    rdo::format("Ожидается целочисленное значение, найдено: %s", from_src_info.src_text().c_str()));
			parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
			parser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__int::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__int)>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getInt(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к целому типу", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__int::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	if (pType->typeID() == rdo::runtime::RDOType::t_real)
	{
		return rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(pCalc->srcInfo().src_pos(), pCalc);
	}
	return pCalc;
}

rdo::runtime::RDOValue RDOType__int::get_default() const
{
	return rdo::runtime::RDOValue(0);
}

void RDOType__int::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << "I" << std::endl;
}

void RDOType__int::serializeInDB(REF(IDB) db) const
{
	if (db.isEmptyContext())
	{
		get_default().serializeInDB(db);
	}

	db.insertRow("int",QString("DEFAULT,%1,NULL,NULL")
		.arg(db.popContext<int>()));

	db.pushContext(db.queryExecIndex("int"));
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__real
// --------------------------------------------------------------------------------
LPRDOType RDOType__real::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->type()->typeID())
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

LPRDOValue RDOType__real::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__real)>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getDouble(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к вещественному типу", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__real::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__real::get_default() const
{
	return rdo::runtime::RDOValue(0.0);
}

void RDOType__real::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << "R" << std::endl;
}

void RDOType__real::serializeInDB(REF(IDB) db) const
{
	if (db.isEmptyContext())
	{
		get_default().serializeInDB(db);
	}

	db.insertRow("real",QString("DEFAULT,%1,NULL,NULL")
		.arg(db.popContext<int>()));

	db.pushContext(db.queryExecIndex("real"));
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__string
// --------------------------------------------------------------------------------
LPRDOType RDOType__string::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->type()->typeID())
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

LPRDOValue RDOType__string::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__string)>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getString(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к строковому типу", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__string::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__string::get_default() const
{
	return rdo::runtime::RDOValue("");
}

void RDOType__string::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << "S" << std::endl;
}

void RDOType__string::serializeInDB(REF(IDB) db) const
{
	if (db.isEmptyContext())
	{
		get_default().serializeInDB(db);
	}

	db.insertRow("string",QString("DEFAULT,%1")
		.arg(db.popContext<int>()));

	db.pushContext(db.queryExecIndex("string"));
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__identificator
// --------------------------------------------------------------------------------
LPRDOType RDOType__identificator::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(to_src_info);
	ASSERT(pFrom      );

	switch (pFrom->type()->typeID())
	{
	case rdo::runtime::RDOType::t_identificator:
		return rdo::Factory<RDOType__identificator>::create();
	default:
		parser::g_error().error(src_info, rdo::format("Внутренная ошибка парсера. Невозможно преобразовать тип '%s' к идентификатору", from_src_info.src_text().c_str()));
	}
	return NULL;
}

LPRDOValue RDOType__identificator::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
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

rdo::runtime::LPRDOCalc RDOType__identificator::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__identificator::get_default() const
{
	return rdo::runtime::RDOValue("", rdo::runtime::g_identificator);
}

void RDOType__identificator::writeModelStructure(REF(rdo::ostream) stream) const
{
	UNUSED(stream);
	parser::g_error().error(RDOParserSrcInfo(), "Внутренная ошибка парсера. Невозможно записать тип идектификатор в отчет");
	NEVER_REACH_HERE;
}

void RDOType__identificator::serializeInDB(REF(IDB) db) const
{
	if (db.isEmptyContext())
	{
		get_default().serializeInDB(db);
	}

	db.insertRow("identificator",QString("DEFAULT,%1")
		.arg(db.popContext<int>()));

	db.pushContext(db.queryExecIndex("identificator"));
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__bool
// --------------------------------------------------------------------------------
LPRDOType RDOType__bool::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->type()->typeID())
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

LPRDOValue RDOType__bool::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__bool)>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getBool(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		parser::g_error().push_only(src_info,    rdo::format("Невозможно преобразовать значение '%s' к булевскому типу", pFrom->src_info().src_text().c_str()));
		parser::g_error().push_only(to_src_info, rdo::format("См. тип: %s", to_src_info.src_text().c_str()));
		parser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__bool::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__bool::get_default() const
{
	return rdo::runtime::RDOValue(false);
}

void RDOType__bool::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << "B" << std::endl;
}

void RDOType__bool::serializeInDB(REF(IDB) db) const
{
	if (db.isEmptyContext())
	{
		get_default().serializeInDB(db);
	}

	db.insertRow("bool",QString("DEFAULT,%1")
		.arg(db.popContext<int>()));

	db.pushContext(db.queryExecIndex("bool"));
}

CLOSE_RDO_PARSER_NAMESPACE

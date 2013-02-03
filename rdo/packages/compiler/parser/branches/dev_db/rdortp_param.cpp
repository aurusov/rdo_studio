/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp_param.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdortp_param.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPParam
// --------------------------------------------------------------------------------
RDORTPParam::RDORTPParam(CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault, CREF(RDOParserSrcInfo) src_info)
	: RDOParam(src_info, pType, pDefault)
{}

RDORTPParam::~RDORTPParam()
{}

CREF(tstring) RDORTPParam::name() const
{
	return RDOParam::name();
}

void RDORTPParam::writeModelStructure(REF(rdo::ostream) stream) const
{
	stream << name() << _T(" ");
	getTypeInfo()->type()->writeModelStructure(stream);
}

void RDORTPParam::serializeInDB(REF(IDB) db) const
{
	int rtp_id = boost::any_cast<int>(db.popContext());
	getTypeInfo()->type()->serializeInDB(db);
	db.insertRow("param_of_type",QString("DEFAULT,'%1',%2,%3")
			.arg(QString::fromStdString(this->name()))
			.arg(rtp_id)
			.arg(boost::any_cast<int>(db.popContext())));
}

CLOSE_RDO_PARSER_NAMESPACE

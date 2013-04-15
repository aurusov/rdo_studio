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
	stream << name() << " ";
	getTypeInfo()->type()->writeModelStructure(stream);
}

#ifdef SERIALIZE_IN_DB_RTP_DETAILS
void RDORTPParam::serializeInDB(REF(IDB) db) const
{
	std::vector<int> indexContainer = db.popContext<std::vector<int>>();

	LPRDOValue def_val = getDefault();
	if (def_val->defined())
	{
		def_val->serializeInDB(db);
	}

	getTypeInfo()->type()->serializeInDB(db);
	db.insertRow("param_of_type",QString("%1,'%2',%3,%4")
			.arg(indexContainer[1])
			.arg(QString::fromStdString(name()))
			.arg(indexContainer[0])
			.arg(db.popContext<int>()));
}
#endif

CLOSE_RDO_PARSER_NAMESPACE

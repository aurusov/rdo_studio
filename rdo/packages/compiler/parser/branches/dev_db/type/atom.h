/*!
  \copyright (c) RDO-Team, 2011
  \file      atom.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.04.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_ATOM_H_
#define _RDOPARSER_TYPE_ATOM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/runtime_wrapper_type.h"
#include "simulator/compiler/parser/headers/db/interface_db.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_PARSER
// --------------------------------------------------------------------------------
#define DEFINE_ATOM_TYPE_PARSER(Type)                     \
class RDOType__##Type: public RuntimeWrapperType          \
{                                                         \
private:                                                  \
typedef RuntimeWrapperType parent_type;                   \
public:                                                   \
	RDOType__##Type():                                    \
		RuntimeWrapperType(rdo::runtime::g_##Type)        \
	{}                                                    \
	~RDOType__##Type()                                    \
	{}                                                    \
	DECLARE_IType;                                        \
	DECLARE_IModelStructure;                              \
	virtual int serializeInDB(REF(IDB) db) const; \
};                                                        \
DECLARE_POINTER(RDOType__##Type);

DEFINE_ATOM_TYPE_PARSER(unknow       );
DEFINE_ATOM_TYPE_PARSER(identificator);
DEFINE_ATOM_TYPE_PARSER(int          );
DEFINE_ATOM_TYPE_PARSER(real         );
DEFINE_ATOM_TYPE_PARSER(bool         );
DEFINE_ATOM_TYPE_PARSER(string       );
DEFINE_ATOM_TYPE_PARSER(void         );

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_TYPE_ATOM_H_

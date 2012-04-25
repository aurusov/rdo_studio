/*!
  \copyright (c) RDO-Team, 2011
  \file      process.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _MBUILDER_PROCESS_H_
#define _MBUILDER_PROCESS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/mbuilder/namespace.h"
#include "simulator/compiler/mbuilder/rdo_resources.h"
// --------------------------------------------------------------------------------

OPEN_COMPILER_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BlockForQueue
// --------------------------------------------------------------------------------
class BlockForQueue
{
public:
	static rbool      checkType (RDOResType rtp,         CREF(parser::RDOParserSrcInfo) info    );
	static void       createRes (RDOResType rtp,         CREF(tstring)                  res_name);
	static RDOResType createType(CREF(tstring) rtp_name, CREF(parser::RDOParserSrcInfo) info    );
};

// --------------------------------------------------------------------------------
// -------------------- BlockForSeize
// --------------------------------------------------------------------------------
class BlockForSeize
{
public:
	static rbool      checkType  (RDOResType    rtp,      CREF(parser::RDOParserSrcInfo) info    );
	static void       createRes  (RDOResType    rtp,      CREF(tstring)                  res_name);
	static void       reobjectRes(RDOResType    rtp,      CREF(tstring)                  res_name);
	static RDOResType createType (CREF(tstring) rtp_name, CREF(parser::RDOParserSrcInfo) info    );
};

CLOSE_COMPILER_MBUILDER_NAMESPACE

#endif // _MBUILDER_PROCESS_H_

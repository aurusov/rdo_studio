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
	static bool checkType(RDOResType rtp, const parser::RDOParserSrcInfo& info);
	static void createRes(RDOResType rtp, const std::string& res_name);
	static RDOResType createType(const std::string& rtp_name, const parser::RDOParserSrcInfo& info);
};

// --------------------------------------------------------------------------------
// -------------------- BlockForSeize
// --------------------------------------------------------------------------------
class BlockForSeize
{
public:
	static bool checkType(RDOResType rtp, const parser::RDOParserSrcInfo& info);
	static void createRes(RDOResType rtp, const std::string& res_name);
	static void reobjectRes(RDOResType rtp, const std::string& res_name);
	static RDOResType createType (const std::string& rtp_name, const parser::RDOParserSrcInfo& info);
};

CLOSE_COMPILER_MBUILDER_NAMESPACE

#endif // _MBUILDER_PROCESS_H_

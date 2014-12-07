#pragma once

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

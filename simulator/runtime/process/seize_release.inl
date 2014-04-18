/*!
  \copyright (c) RDO-Team, 2012
  \file      seize_release.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессные операторы SEIZE и RELEASE
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
inline std::string RDOPROCBlockForSeize::getStateParamName()
{
	return "Состояние";
}

inline std::string RDOPROCBlockForSeize::getStateEnumFree()
{
	return "Свободен";
}

inline std::string RDOPROCBlockForSeize::getStateEnumBuzy()
{
	return "Занят";
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
inline RDOPROCSeize::RDOPROCSeize(LPIPROCProcess process, std::vector<parser_for_Seize> From_Par)
	: RDOPROCBlockForSeize(process, From_Par)
{
	static std::size_t g_index = 1;
	index = g_index++;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCRelease
// --------------------------------------------------------------------------------
inline RDOPROCRelease::RDOPROCRelease(LPIPROCProcess process, std::vector<parser_for_Seize> From_Par)
	: RDOPROCBlockForSeize(process, From_Par)
{
	static std::size_t g_index = 1;
	index = g_index++;
}

CLOSE_RDO_RUNTIME_NAMESPACE

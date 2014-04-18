/*!
  \copyright (c) RDO-Team, 2011
  \file      result_group.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     Группы собираемых показателей
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RESULT_GROUP_H_
#define _LIB_RUNTIME_RESULT_GROUP_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/filesystem/fstream.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/result/result_group_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOPMDResultGroup
  \brief     Группа собираемых при моделировании показателей
*/
class RDOPMDResultGroup: public IResult, public IResultGroup
{
DEFINE_IFACTORY(RDOPMDResultGroup);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IResult     )
	QUERY_INTERFACE(IResultGroup)
QUERY_INTERFACE_END

private:
	RDOPMDResultGroup(const std::string& name);
	virtual ~RDOPMDResultGroup();

	/*!
	  \enum      State
	  \brief     Состояние группы показателей
	*/
	enum State
	{
		RGS_STOP,
		RGS_START
	};

	typedef std::vector<LPIResult> ResultList;
	ResultList m_resultList;
	std::string m_name;
	State m_state;
	double m_timeStart;
	boost::filesystem::ofstream m_streamFull;
	boost::filesystem::ofstream m_streamTable;

	DECLARE_IResult;
	DECLARE_IResultGroup;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RESULT_GROUP_H_

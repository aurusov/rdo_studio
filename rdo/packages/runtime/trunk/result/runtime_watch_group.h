/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopokaz_group.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     Группы собираемых показателей
  \indent    4T
*/

#ifndef _LIB_RUNTIME_POKAZ_GROUP_H_
#define _LIB_RUNTIME_POKAZ_GROUP_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdopokaz_group_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOPMDPokazGroup
  \brief     Группа собираемых при моделировании показателей
*/
class RDOPMDPokazGroup: public IPokaz, public IPokazGroup
{
DEFINE_IFACTORY(RDOPMDPokazGroup);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPokaz     )
	QUERY_INTERFACE(IPokazGroup)
QUERY_INTERFACE_END

private:
	RDOPMDPokazGroup(CREF(tstring) name);
	virtual ~RDOPMDPokazGroup();

	/*!
	  \enum      State
	  \brief     Состояние группы показателей
	*/
	enum State
	{
		RGS_STOP,
		RGS_START
	};

	typedef std::vector<LPIPokaz> ResultList;
	ResultList     m_resultList;
	State          m_state;
	tstring        m_name;
	double         m_timeStart;
	std::ofstream  m_streamFull;
	std::ofstream  m_streamTable;

	DECLARE_IPokaz;
	DECLARE_IPokazGroup;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_POKAZ_GROUP_H_

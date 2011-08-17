/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoidgenerator.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      23.02.2010
  \brief     
  \indent    4T
*/

#ifndef _RDOIDGENERATOR_H_
#define _RDOIDGENERATOR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

class IDGenerator
{
public:
	typedef ruint ID;

	IDGenerator()
		: m_id(0)
	{}

	ID get()
	{
		return m_id++;
	}

private:
	ID m_id;
};

CLOSE_RDO_NAMESPACE

#endif //! _RDOIDGENERATOR_H_

/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoidgenerator.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      23.02.2010
  \brief     
  \indent    4T
*/

#ifndef _UTILS_RDOIDGENERATOR_H_
#define _UTILS_RDOIDGENERATOR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

class IDGenerator
{
public:
	typedef std::size_t ID;

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

} // namespace rdo

#endif // _UTILS_RDOIDGENERATOR_H_

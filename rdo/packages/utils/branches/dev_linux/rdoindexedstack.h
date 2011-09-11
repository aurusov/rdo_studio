/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoindexedstack.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      23.02.2010
  \brief     
  \indent    4T
*/

#ifndef _UTILS_RDOINDEXEDSTACK_H_
#define _UTILS_RDOINDEXEDSTACK_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
#include "utils/rdotypes.h"
#include "utils/rdoidgenerator.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template<class T>
class IndexedStack
{
public:
	typedef IDGenerator::ID ID;

	IndexedStack()
	{}

	ID push(CREF(T) object)
	{
		std::pair<Stack::iterator, rbool> result = m_stack.insert(
			std::pair<Stack::key_type, Stack::mapped_type>(m_generator.get(), object)
		);
		ASSERT(result.second);
		return result.first->first;
	}
	T pop(ID index)
	{
		Stack::iterator it = m_stack.find(index);
		ASSERT(it != m_stack.end());
		T object = it->second;
		m_stack.erase(it);
		return object;
	}

protected:
	typedef std::map<IDGenerator::ID, T> Stack;
	Stack       m_stack;
	IDGenerator m_generator;
};

CLOSE_RDO_NAMESPACE

#endif // _UTILS_RDOINDEXEDSTACK_H_

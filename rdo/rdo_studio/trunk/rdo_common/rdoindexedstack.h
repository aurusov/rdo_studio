/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdoindexedstack.h
 * author   : Урусов Андрей
 * date     : 23.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOINDEXEDSTACK_H_
#define _RDOINDEXEDSTACK_H_

// ====================================================================== INCLUDES
#include <map>
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdoidgenerator.h"
// ===============================================================================

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

#endif //! _RDOINDEXEDSTACK_H_

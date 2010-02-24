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

class IndexedStack
{
public:
	typedef IDGenerator::ID ID;

	IndexedStack()
	{}

	template<class T>
	ID push(PTR(T) object)
	{
		std::pair<Stack::iterator, rbool> result = m_stack.insert(std::pair(m_generator.get(), object));
		ASSERT(result.second);
		return result.first->first;
	}
	template<class T>
	PTR(T) pop(ID index)
	{
		Stack::const_iterator it = m_stack.find(index);
		ASSERT(it != m_stack.end());
		PTR(T) object = reinterpret_cast<PTR(T)>(it->second);
		m_stack.erase(it);
		return object;
	}

	void destroy()
	{
		STL_FOR_ALL(Stack, m_stack, it)
		{
			delete it->second;
		}
		m_stack.clear();
	}

private:
	typedef std::map<IDGenerator::ID, PTR(void)> Stack;
	Stack       m_stack;
	IDGenerator m_generator;
};

CLOSE_RDO_NAMESPACE

#endif //! _RDOINDEXEDSTACK_H_

/*!
  \copyright (c) RDO-Team, 2011
  \file      utils/stack.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.02.2010
  \brief     
  \indent    4T
*/

#ifndef _RDO_COMMON_STACK_H_
#define _RDO_COMMON_STACK_H_

// ----------------------------------------------------------------------- INCLUDES
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
class stack
{
public:
	void push(const T& item);
	void pop();

	bool empty() const;
	std::size_t size() const;

	const T& top() const;
	 T& top();

private:
	typedef std::list<T> Container;
	Container m_container;
};

} // namespace rdo

#include "utils/src/stuff/stack.inl"

#endif // _RDO_COMMON_STACK_H_

#pragma once

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

#include "utils/src/stuff/stack-inl.h"

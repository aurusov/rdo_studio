/*!
  \copyright (c) RDO-Team, 2011-2012
  \file      bison_value_pair.h
  \authors   Чирков Михаил
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      02.12.2010
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_BISON_VALUE_PAIR_H_
#define _SIMULATOR_COMPILER_PARSER_BISON_VALUE_PAIR_H_

// ----------------------------------------------------------------------- INCLUDES
#include <utility>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/compiler/parser/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BisonValuePair
// --------------------------------------------------------------------------------
template <class T1, class T2>
class BisonValuePair: INSTANCE_OF(rdo::counter_reference)
{
public:
	CREF(T1) getFirst () const;
	CREF(T2) getSecond() const;

private:
	typedef  BisonValuePair<T1, T2>  this_type;
	DECLARE_FACTORY(this_type);

	BisonValuePair(CREF(T1) first, CREF(T2) second);
	virtual ~BisonValuePair();

	typedef  std::pair<T1, T2>  Value;

	Value m_value;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/bison_value_pair.inl"

#endif // _SIMULATOR_COMPILER_PARSER_BISON_VALUE_PAIR_H_

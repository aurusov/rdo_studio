/*!
  \copyright (c) RDO-Team, 2011
  \file      rdobison.h
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     Структура координат токена бизона, дополненная линейной позицией
  \indent    4T
*/

#ifndef _CONVERTOR_RDOBISON_H_
#define _CONVERTOR_RDOBISON_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdotypes.h"
// --------------------------------------------------------------------------------

#ifndef YYLTYPE
struct yyltype
{
	ruint m_first_line;
	ruint m_first_pos;

	ruint m_last_line;
	ruint m_last_pos;

	ruint m_first_seek;
	ruint m_last_seek;
};

#define YYLTYPE yyltype
#endif //! YYLTYPE

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)       \
	Current.m_last_line = Rhs[N].m_last_line; \
	Current.m_last_pos  = Rhs[N].m_last_pos;  \
	Current.m_last_seek = Rhs[N].m_last_seek;
#endif //! YYLLOC_DEFAULT

#endif //! _CONVERTOR_RDOBISON_H_

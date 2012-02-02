/*!
  \copyright (c) RDO-Team, 2011
  \file      rdobison.h
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.08.2010
  \brief     Структура координат токена бизона, дополненная линейной позицией
  \indent    4T
*/

#ifndef _RDOBISON_H_
#define _RDOBISON_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

// Определим YYSTYPE именно в этом файле, чтобы убрать зависимость от rdogramma.h
#ifdef YYSTYPE
#undef YYSTYPE
#endif
typedef int YYSTYPE;

#ifdef YYLTYPE
#undef YYLTYPE
#endif

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

#ifdef YYLLOC_DEFAULT
#undef YYLLOC_DEFAULT
#endif

#define YYLLOC_DEFAULT(Current, Rhs, N)                             \
	do {                                                            \
		if (YYID(N))                                                \
		{                                                           \
			(Current).m_first_line = YYRHSLOC(Rhs, 1).m_first_line; \
			(Current).m_first_pos  = YYRHSLOC(Rhs, 1).m_first_pos;  \
			(Current).m_first_seek = YYRHSLOC(Rhs, 1).m_first_seek; \
			(Current).m_last_line  = YYRHSLOC(Rhs, N).m_last_line;  \
			(Current).m_last_pos   = YYRHSLOC(Rhs, N).m_last_pos;   \
			(Current).m_last_seek  = YYRHSLOC(Rhs, N).m_last_seek;  \
		}                                                           \
		else                                                        \
		{                                                           \
			(Current).m_first_line = YYRHSLOC(Rhs, 0).m_last_line;  \
			(Current).m_last_line  = YYRHSLOC(Rhs, 0).m_last_line;  \
			(Current).m_first_pos  = YYRHSLOC(Rhs, 0).m_last_pos;   \
			(Current).m_last_pos   = YYRHSLOC(Rhs, 0).m_last_pos;   \
			(Current).m_first_seek = YYRHSLOC(Rhs, 0).m_last_seek;  \
			(Current).m_last_seek  = YYRHSLOC(Rhs, 0).m_last_seek;  \
		}                                                           \
	}                                                               \
	while (YYID(0))

#endif // _RDOBISON_H_

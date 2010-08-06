/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdobison.h
 * author   : Лущан Дмитрий, Урусов Андрей
 * date     : 
 * bref     : Структура координат токена бизона, дополненная линейной позицией
 * indent   : 4T
 */

#ifndef _RDOBISON_H_
#define _RDOBISON_H_

// ====================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdotypes.h"
// ===============================================================================

#ifndef YYLTYPE
struct yyltype
{
	ruint first_line;
	ruint first_column;

	ruint last_line;
	ruint last_column;

	ruint first_linear_pos;
	ruint last_linear_pos;
};

#define YYLTYPE yyltype
#endif //! YYLTYPE

#endif //! _RDOBISON_H_

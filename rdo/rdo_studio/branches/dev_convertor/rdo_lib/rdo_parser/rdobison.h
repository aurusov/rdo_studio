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

#ifndef YYLTYPE
struct yyltype
{
  int first_line;
  int first_column;

  int last_line;
  int last_column;

  int linear_pos;
};

#define YYLTYPE yyltype
#endif //! YYLTYPE

#endif //! _RDOBISON_H_

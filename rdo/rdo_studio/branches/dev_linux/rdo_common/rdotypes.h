/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdotypes.h
 * author   : Урусов Андрей
 * date     : 18.04.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOTYPES_H_
#define _RDOTYPES_H_

// ====================================================================== INCLUDES
#ifdef WIN32
#include <tchar.h>
#else
#define _T(x)       x
#endif
#include <string>
// ====================================================================== SYNOPSIS
// ===============================================================================

typedef  char                       tchar;
typedef  std::basic_string<tchar>   tstring;
typedef  bool                       rbool;
typedef  signed int                 rsint;
typedef  unsigned int               ruint;
typedef  signed long long int       rsint64;
typedef  unsigned long long int     ruint64;
typedef  unsigned char              rbyte;

#endif //! _RDOTYPES_H_

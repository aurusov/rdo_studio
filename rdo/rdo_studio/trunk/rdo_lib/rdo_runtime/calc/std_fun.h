/*
 * copyright: (c) RDO-Team, 2011
 * filename : std_fun.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALC_STD_FUN_H_
#define _RDOCALC_STD_FUN_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Стандартные функции языка
// ----------------------------------------------------------------------------
#define DEFINE_RDO_STD_FUN(CalcName)       \
CALC_SUB(RDOFunCalc##CalcName, RDOFunCalc) \
{                                          \
DECLARE_FACTORY(RDOFunCalc##CalcName)      \
private:                                   \
	RDOFunCalc##CalcName()                 \
	{}                                     \
	DECALRE_ICalc;                         \
};

DEFINE_RDO_STD_FUN( Sin      );
DEFINE_RDO_STD_FUN( Cos      );
DEFINE_RDO_STD_FUN( Tan      );
DEFINE_RDO_STD_FUN( Cotan    );
DEFINE_RDO_STD_FUN( ArcCos   );
DEFINE_RDO_STD_FUN( ArcSin   );
DEFINE_RDO_STD_FUN( ArcTan   );
DEFINE_RDO_STD_FUN( Abs      );
DEFINE_RDO_STD_FUN( Sqrt     );
DEFINE_RDO_STD_FUN( Round    );
DEFINE_RDO_STD_FUN( Exp      );
DEFINE_RDO_STD_FUN( Floor    );
DEFINE_RDO_STD_FUN( Frac     );
DEFINE_RDO_STD_FUN( IAbs     );
DEFINE_RDO_STD_FUN( IMax     );
DEFINE_RDO_STD_FUN( IMin     );
DEFINE_RDO_STD_FUN( Int      );
DEFINE_RDO_STD_FUN( IntPower );
DEFINE_RDO_STD_FUN( Ln       );
DEFINE_RDO_STD_FUN( Log2     );
DEFINE_RDO_STD_FUN( LogN     );
DEFINE_RDO_STD_FUN( Log10    );
DEFINE_RDO_STD_FUN( Max      );
DEFINE_RDO_STD_FUN( Min      );
DEFINE_RDO_STD_FUN( Power    );

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOCALC_STD_FUN_H_

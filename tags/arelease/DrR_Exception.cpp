//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DrR_Exception.h"
#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// ---------- RubberError
// ---------------------------------------------------------------------------
__fastcall RubberError::RubberError(const HRESULT resID): Exception(resID)
{
  Message = LoadStr(resID);
}

__fastcall RubberError::RubberError(const HRESULT resID, const int funID): Exception(resID)
{
  Message = MiscFormat(LoadStr(sFunError).c_str(), LoadStr(funID).c_str(), Message.c_str());
}

__fastcall RubberError::RubberError(const AnsiString& Value): Exception(Value)
{
}


//---------------------------------------------------------------------------
#ifndef DrR_ExceptionH
#define DrR_ExceptionH
//---------------------------------------------------------------------------
#include <sysutils.hpp>
#include <vcl/dstring.h>
#pragma hdrstop
// ---------------------------------------------------------------------------
// ---------- RubberError
// ---------------------------------------------------------------------------
class RubberError : public Exception
{
public:
  __fastcall RubberError(const HRESULT resID);
  __fastcall RubberError(const HRESULT resID, const int funID);
  __fastcall RubberError(const AnsiString& Value);
};
//---------------------------------------------------------------------------
#endif


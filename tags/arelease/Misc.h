//---------------------------------------------------------------------------
#ifndef MiscH
#define MiscH
//---------------------------------------------------------------------------
#pragma hdrstop
//---------------------------------------------------------------------------
AnsiString GetNormalDirFromDir(const AnsiString& Value);
AnsiString GetString(const int ID);
void ShowTypedMessage(const AnsiString Message, const UINT Type);
void LoadTransparentBitmap(TImage* Image, const AnsiString FileName, const bool Transparent);
#endif

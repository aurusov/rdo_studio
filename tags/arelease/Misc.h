//---------------------------------------------------------------------------
#ifndef MiscH
#define MiscH
//---------------------------------------------------------------------------
#include <controls.hpp>
#include <vcl/dstring.h>
#include <systdate.h>
#pragma hdrstop
//---------------------------------------------------------------------------
void ShowInfoMessage(AnsiString& Value);
void ShowInfoMessage(const int resID);
int ShowQueryMessage(AnsiString& Value, const int flag);
int ShowQueryMessage(const int resID, const int flag);
bool ShowErrorMessage(AnsiString& Value, const bool bStopIcon = false);
bool ShowErrorMessage(const int resID, const bool bStopIcon = false);
AnsiString MiscFormat(char const *str, ...);
void LoadBMP(TImageList* images, const int Value);
bool TranslateMessage(AnsiString &Message);
AnsiString GetNormalDirFromDir(const AnsiString& Value);
AnsiString GetNormalDirFromName(const AnsiString& Value);
bool DirExists(const char* Value);
AnsiString GetUserName();
AnsiString GetCompName();
AnsiString GetLocaleStr(const int LocaleID, const AnsiString& defValue);
double RoundDouble(const double value, const int degree = 2);
AnsiString RoundDoubleToAnsiString(const double value, const int degree = 2);
void doDesktopCenter(TWinControl* win);
void SaveResourceToStream(const int res_id, TStream* stream);
AnsiString PickOutStr(AnsiString str, const int index);
double PickOutDouble(AnsiString str, const int index);
int PickOutInt(AnsiString str, const int index);
int SmenaToDate(int smena, TDateTime& date);
//---------------------------------------------------------------------------
#endif


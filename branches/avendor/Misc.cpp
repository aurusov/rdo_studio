//---------------------------------------------------------------------------
#include <vcl.h>
#include <SysUtils.hpp>
#include <stdio.h>
#include <winnls.h>
#pragma hdrstop

#include "Misc.h"
#include "DrR_Exception.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
void ShowInfoMessage(AnsiString& Value)
{
  AnsiString s = "";
  if (*Value.AnsiLastChar() != '.') s = '.';
  Application->MessageBox((Value.Trim() + s).c_str(), Application->Title.c_str(), MB_OK + MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void ShowInfoMessage(const int resID)
{
  AnsiString s = LoadStr(resID);
  ShowInfoMessage(s);
}
//---------------------------------------------------------------------------
int ShowQueryMessage(AnsiString& Value, const int flag)
{
  return Application->MessageBox((Value.Trim() + " ?").c_str(), Application->Title.c_str(), MB_ICONQUESTION + flag);
}
//---------------------------------------------------------------------------
int ShowQueryMessage(const int resID, const int flag)
{
  AnsiString s = LoadStr(resID);
  return ShowQueryMessage(s, flag);
}
//---------------------------------------------------------------------------
bool ShowErrorMessage(AnsiString& Value, const bool bStopIcon)
{
  if (Value.Length()) {
    UINT flag = MB_OK;

    flag += bStopIcon ? MB_ICONSTOP : MB_ICONWARNING;
    AnsiString s = "";
    if (*Value.AnsiLastChar() != '.') s = '.';
    Application->MessageBox((Value.Trim() + s).c_str(), Application->Title.c_str(), flag);
  }
  return false;
}
//---------------------------------------------------------------------------
bool ShowErrorMessage(const int resID, const bool bStopIcon)
{
  AnsiString s = LoadStr(resID);
  return ShowErrorMessage(s, bStopIcon);
}
//---------------------------------------------------------------------------
AnsiString MiscFormat(char const *str, ...)
{
  va_list paramList;
  va_start(paramList, str);
  int size = vsnprintf(NULL, 0, str, paramList);
  va_end(paramList);
  if (size) {
    AnsiString formatStr;
    formatStr.SetLength(size);
    vsnprintf(formatStr.c_str(), size, str, paramList);
    return formatStr;
  }
  return "";
}
//---------------------------------------------------------------------------
void LoadBMP(TImageList* images, const int Value)
{
  Graphics::TBitmap *ResBMP = new Graphics::TBitmap();

  ResBMP->LoadFromResourceID(0, Value);
  images->AddMasked(ResBMP, ResBMP->TransparentColor);
  delete ResBMP;
}
//---------------------------------------------------------------------------
bool TranslateMessage(AnsiString &Message)
{
  int res_id = Translate_ID;
  while (res_id < Translate_ID_End) {
    if (Message.AnsiCompare(LoadStr(res_id)) == 0) {
      Message = LoadStr(res_id+1);
      return true;
    }
    res_id += 2;
  }

  int sPos, j, k;
  char* sStr = "%s";
  AnsiString strID, strSource, strSource2, str;
  TStringList* strList = new TStringList();

  res_id = Translate_ID;
  while (res_id < Translate_ID_End)
  {
    strID = LoadStr(res_id);
    strSource  = Message;
    strSource2 = strSource;
    strList->Clear();
    k = 0;
    sPos = strID.Pos(sStr);
    while (sPos) {
      if (sPos != 1) {
        str = strID.SubString(1, sPos-1);
        j = strSource.Pos(str);
      } else { str.Delete(1, str.Length()); j = 1; }
      if (j) {
        k += sPos;
        j = str.Length();
        strSource.Delete(1, j);
        strID.Delete(1, j+2);
        if (strID.Length()) {
          j = strID.Pos(sStr) - 1;
          if (j < 0) j = strID.Length();
          j = strSource.Pos(strID.SubString(1, j))-1;
        } else {
          j = strSource.Length();
        }
        if (j >= 0) {
          strList->Add(strSource.SubString(1, j));
          strSource.Delete(1, j);
          strSource2.Delete(k, j);
          strSource2.Insert(sStr, k);
          k++;
        } else break;
      } else break;
      sPos = strID.Pos(sStr);
    }
    if (LoadStr(res_id).AnsiCompare(strSource2) == 0) {
      char const* a[10];
      memset(a, 0, sizeof(a));
      for (j = 0; j < strList->Count; j++) {
        for (int res_field_id = Translate_Field_ID; res_field_id < Translate_Field_ID_End; res_field_id += 2)
          if (strList->Strings[j].AnsiCompare(LoadStr(res_field_id)) == 0) {
            strList->Strings[j] = LoadStr(res_field_id+1);
            break;
          }
        if (j < 10) a[j] = strList->Strings[j].c_str();
      }
      Message = MiscFormat(LoadStr(res_id+1).c_str(), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
      delete strList;
      return true;
    }
    res_id += 2;
  }
  delete strList;
  return false;
}
//---------------------------------------------------------------------------
AnsiString GetNormalDirFromDir(const AnsiString& Value)
{
  AnsiString s = Value;

  if (s.Length()) {
    char c = *s.AnsiLastChar();
    if (c != '/' && c != '\\') s += '\\';
  }
  return s;
}
//---------------------------------------------------------------------------
AnsiString GetNormalDirFromName(const AnsiString& Value)
{
  return GetNormalDirFromDir(ExtractFileDir(Value));
}
//---------------------------------------------------------------------------
bool DirExists(const char* Value)
{
  DWORD rValue = GetFileAttributes(Value);
  return (int)rValue != -1 && (rValue & FILE_ATTRIBUTE_DIRECTORY);
}
//---------------------------------------------------------------------------
AnsiString GetUserName()
{
  AnsiString str = AnsiString::StringOfChar(' ', MAX_COMPUTERNAME_LENGTH+1);
  DWORD length = MAX_COMPUTERNAME_LENGTH+1;
  if (GetUserName(str.c_str(), &length)) {
    str.SetLength(length);
    int len = str.Length()+1;
    for (int i = 1; i < len;  i++)
      if (str[i] == '\0') str[i] = ' ';
  } else {
    str = LoadStr(sUnknowValue);
  }
  return str.Trim();
}
//---------------------------------------------------------------------------
AnsiString GetCompName()
{
  AnsiString str = AnsiString::StringOfChar(' ', MAX_COMPUTERNAME_LENGTH+1);
  DWORD length = MAX_COMPUTERNAME_LENGTH+1;
  if (GetComputerName(str.c_str(), &length)) {
    str.SetLength(length);
    int len = str.Length()+1;
    for (int i = 1; i < len;  i++)
      if (str[i] == '\0') str[i] = ' ';
  } else {
    str = LoadStr(sUnknowValue);
  }
  return str.Trim();
}
//---------------------------------------------------------------------------
AnsiString GetLocaleStr(const int LocaleID, const AnsiString& defValue)
{
  LCID Locale = GetThreadLocale();
  char buf[255];
  int length = GetLocaleInfo(Locale, LocaleID, buf, sizeof(buf));
  if (length) {
    AnsiString s = buf;
    s.SetLength(length-1);
    return s;
  }
  return defValue;
}
//---------------------------------------------------------------------------
double RoundDouble(const double value, const int degree)
{
  return RoundDoubleToAnsiString(value, degree).ToDouble();
}
//---------------------------------------------------------------------------
AnsiString RoundDoubleToAnsiString(const double value, const int degree)
{
  AnsiString s = "%1." + IntToStr(degree) + "f";
  AnsiString str = MiscFormat(s.c_str(), value);
  int length = str.Length();
  for (int i = 1; i <= length; i++)
    if (str[i] == '.') {
      str[i] = DecimalSeparator;
      break;
    }
  return str;
}
//---------------------------------------------------------------------------
void doDesktopCenter(TWinControl* win)
{
  RECT rect;
  if (SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0)) {
    int left = (rect.right - rect.left - win->Width) / 2 + rect.left;
    if (left < rect.left) left = rect.left;
    win->Left = left;
    int top = (rect.bottom - rect.top - win->Height) / 2 + rect.top;
    if (top < rect.top) top = rect.top;
    win->Top = top;
  }
}
//---------------------------------------------------------------------------
void SaveResourceToStream(const int res_id, TStream* stream)
{
  TResourceStream* res = new TResourceStream(0, res_id, RT_RCDATA);
  try {
    res->SaveToStream(stream);
    delete res;
  } catch (Exception& e) {
    delete res;
    throw;
  }
}
//---------------------------------------------------------------------------
AnsiString PickOutStr(AnsiString str, const int index)
{
  str = str.Trim();
  int space;
  for (int i = 0; i < index-1; i++) {
    space = str.Pos(" ");
    if (space) {
      str.Delete(1, space);
      str = str.TrimLeft();
    } else return "";
  }
  space = str.Pos(" ");
  str.Delete(space, str.Length() - space + 1);
  return str;
}
//---------------------------------------------------------------------------
double PickOutDouble(AnsiString str, const int index)
{
  str = PickOutStr(str, index);
  int length = str.Length();
  for (int i = 1; i <= length; i++)
    if (str[i] == '.') {
      str[i] = DecimalSeparator;
      break;
    }
  return str.ToDouble();
}
//---------------------------------------------------------------------------
int PickOutInt(AnsiString str, const int index)
{
  return PickOutDouble(str, index);
}
//---------------------------------------------------------------------------
int SmenaToDate(int smena, TDateTime& date)
{
  if (smena <= 0) return 0;
  int work_days = 1 + (smena-1) / 3;
  smena -= (work_days - 1) * 3;
  // Суббота
  if (date.DayOfWeek() == 7) { date++; date++; }
  // Воскресенье
  if (date.DayOfWeek() == 1) date++;
  for (int i = 1; i < work_days; i++) {
    date++;
    // Суббота
    if (date.DayOfWeek() == 7) { date++; date++; }
    // Воскресенье
    if (date.DayOfWeek() == 1) date++;
  }
  return smena;
}
//---------------------------------------------------------------------------


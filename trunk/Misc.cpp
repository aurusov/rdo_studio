//---------------------------------------------------------------------------
#include <vcl.h>
#include "inifiles.hpp"
#pragma hdrstop

#include "Misc.h"
#include "ConstRes.h"
#include "Main.h"
#include "Message_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
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
AnsiString GetString(const int ID)
{
  TIniFile* lng = NULL;
  AnsiString res = "";
  try {
    AnsiString path = GetNormalDirFromDir(ExtractFilePath(Application->ExeName)) + "data\\RAO_Explorer.lng";
    lng = new TIniFile(path);
    res = lng->ReadString(Form1->Language, IntToStr(ID - StringID), "");
    delete lng; lng = NULL;
  } catch (Exception& e) {
    if (lng) delete lng;
  }
  if (!res.data())
    LoadStr(ID);
  return res;
}

//---------------------------------------------------------------------------
void ShowTypedMessage(const AnsiString Message, const UINT Type)
{
  TMessageForm* msg = NULL;
  try {
    msg = new TMessageForm();
    msg->Message = Message;
    msg->Type = Type;
    msg->ShowModal();
    delete msg; msg = NULL;
  } catch (Exception& e) {
    if (msg) delete msg;
    throw;
  }
}

//---------------------------------------------------------------------------
void LoadTransparentBitmap(TImage* Image, const AnsiString FileName, const bool Transparent)
{
  Graphics::TBitmap* bmp = NULL;
  TImageList* transplist = NULL;
  try {
    Image->Transparent = Transparent;
    if (!Transparent)
      Image->Picture->Bitmap->LoadFromFile(FileName);
    else {
      transplist = new TImageList(NULL);
      Image->Picture->Bitmap = NULL;
      bmp = new Graphics::TBitmap();
      bmp->LoadFromFile(FileName);
      transplist->Width = bmp->Width;
      transplist->Height = bmp->Height;
      transplist->BkColor = bmp->TransparentColor;
      if (transplist->AddMasked(bmp, bmp->Canvas->Pixels[0][bmp->Height - 1]) == -1) {
        Image->Transparent = false;
        Image->Picture->Bitmap->LoadFromFile(FileName);
      } else
        transplist->GetBitmap(0, Image->Picture->Bitmap);
      delete bmp; bmp = NULL;
      delete transplist; transplist = NULL;
    }
  } catch (Exception &e) {
    if (bmp) delete bmp;
    if (transplist) delete transplist;
    throw;
  }
}

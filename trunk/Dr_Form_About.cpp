//---------------------------------------------------------------------------
#include <vcl.h>
#include <shellapi.h>
#pragma hdrstop

#include "Dr_Form_About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#define KoefHeight 54
//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm():
  TForm((void*)NULL),
  IliosEmail(NULL),
  MSTUEmail(NULL),
  OkB(NULL),
  FMouseDown(false),
  rgn(NULL)
{
  IliosEmail = new TDrHilightLabel(this);
  IliosEmail->Left          = 216;
  IliosEmail->Top           = 72;
  IliosEmail->Caption       = "ilios@kolomna.ru";
  IliosEmail->NormalColor   = (TColor)0x007F0018;
  IliosEmail->HilightColor  = (TColor)0x00907F00;
  IliosEmail->HilightCursor = crHandPoint;
  IliosEmail->HilightStyle  << fsUnderline;
  IliosEmail->OnClick       = IliosEmailClick;
  IliosEmail->Parent        = this;
  MSTUEmail = new TDrHilightLabel(this);
  MSTUEmail->Left          = 268;
  MSTUEmail->Top           = 136;
  MSTUEmail->Caption       = "evv@rk9.bmstu.ru";
  MSTUEmail->NormalColor   = (TColor)0x007F0018;
  MSTUEmail->HilightColor  = (TColor)0x00907F00;
  MSTUEmail->HilightCursor = crHandPoint;
  MSTUEmail->HilightStyle  << fsUnderline;
  MSTUEmail->OnClick       = MSTUEmailClick;
  MSTUEmail->Parent        = this;
  OkB = new TDrHilightLabel(this);
  OkB->Left          = 344;
  OkB->Top           = 194;
  OkB->Caption       = "Ok";
  OkB->NormalColor   = (TColor)0x007F0018;
  OkB->HilightColor  = (TColor)0x00907F00;
  OkB->HilightCursor = crHandPoint;
  OkB->NormalStyle   << fsBold;
  OkB->HilightStyle  << fsBold << fsUnderline;
  OkB->OnClick       = OkBClick;
  OkB->Parent        = this;

  rgn = CreateRoundRectRgn(0, 0, Width, KoefHeight * 4, KoefHeight, KoefHeight);
  SetWindowRgn(Handle, rgn, true);
  BigShape->Left   = 0;
  BigShape->Top    = 0;
  BigShape->Width  = ClientWidth - 1;
  BigShape->Height = KoefHeight * 4 - 1;
}

__fastcall TAboutForm::~TAboutForm()
{
  DeleteObject(rgn);
  if (IliosEmail) delete IliosEmail;
  if (MSTUEmail)  delete MSTUEmail;
  if (OkB)        delete OkB;
}

void __fastcall TAboutForm::WndProc(Messages::TMessage &Message)
{
  TForm::WndProc(Message);
  switch (Message.Msg) {
    case WM_LBUTTONDOWN  :
    case WM_LBUTTONDBLCLK: FMouseDown = true;
                           dX = -Message.LParamLo;
                           dY = -Message.LParamHi;
                           break;

    case WM_LBUTTONUP    : FMouseDown = false; break;

    case WM_MOUSEMOVE    : if (FMouseDown) {
                             Left += Message.LParamLo + dX;
                             Top  += Message.LParamHi + dY;
                           }
                           break;

    case WM_KEYDOWN      : if (Message.WParam == VK_ESCAPE) Close();
                           break;
  }
}

void __fastcall TAboutForm::IliosEmailClick(TObject *Sender)
{
  AnsiString email = "mailto:" + IliosEmail->Caption;
  ShellExecute(Application->Handle, "open", email.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void __fastcall TAboutForm::MSTUEmailClick(TObject *Sender)
{
  AnsiString email = "mailto:" + MSTUEmail->Caption;
  ShellExecute(Application->Handle, "open", email.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void __fastcall TAboutForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}


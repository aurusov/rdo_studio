//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Message_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TMessageForm::TMessageForm(void) : TForm((void*)NULL),
            rgn(NULL),
            OkB(NULL),
            FType(MB_OK),
            FMessage(""),
            KoefHeight(47)
{
  rgn = CreateRoundRectRgn(0, 0, Width, KoefHeight * 4, KoefHeight, KoefHeight);
  SetWindowRgn(Handle, rgn, true);
  BigShape->Left   = 0;
  BigShape->Top    = 0;
  BigShape->Width  = ClientWidth - 1;
  BigShape->Height = KoefHeight * 4 - 1;

  OkB = new THighlightText(this);
  OkB->Color         = (TColor)0x00435A65;
  OkB->Caption       = "Ok";
  OkB->NormColor   = (TColor)0x00D0D7D7;
  OkB->HighlColor  = clWhite;
  OkB->HighlCursor = crHandPoint;
  OkB->NormStyle   << fsBold;
  OkB->HighlStyle  << fsBold << fsUnderline;
  OkB->OnClick       = OkBClick;
  Canvas->Font->Assign(OkB->Font);
  Canvas->Font->Style = OkB->NormStyle;
  OkB->Left           = Width - 60 - Canvas->TextWidth(OkB->Caption);
  OkB->Top            = BottomShape->Top + BottomShape->Pen->Width + (BottomShape->Height - OkB->Height)/2;
  OkB->Parent         = this;

  Title->Caption = Application->Title;
}

//---------------------------------------------------------------------------
__fastcall TMessageForm::~TMessageForm(void)
{
  if (OkB)                delete OkB;
  DeleteObject(rgn);
}

//---------------------------------------------------------------------------
void __fastcall TMessageForm::WndProc(Messages::TMessage &Message)
{
  TForm::WndProc(Message);
  switch (Message.Msg) {

    case WM_LBUTTONDOWN  :
    case WM_LBUTTONDBLCLK: {
      FMouseDown = true;
      dX = -Message.LParamLo;
      dY = -Message.LParamHi;
      break;
    }

    case WM_LBUTTONUP    : FMouseDown = false; break;

    case WM_MOUSEMOVE    : {
      if (FMouseDown) {
        int x = Message.LParamLo;
        int y = Message.LParamHi;
        int screen_x = GetSystemMetrics(SM_CXSCREEN);
        int screen_y = GetSystemMetrics(SM_CYSCREEN);
        if ( !screen_x ) screen_x = 10000;
        if ( !screen_y ) screen_y = 10000;
        while ( x > screen_x ) x -= 32768;
        while ( y > screen_y ) y -= 32768;
        Left += x + dX;
        Top  += y + dY;
      }
      break;
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TMessageForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TMessageForm::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_RETURN && OkB->Enabled) {
    Key = 0;
    ModalResult = mrOk;
  }
  if (Key == VK_ESCAPE) {
    Key = 0;
    ModalResult = mrCancel;
  }
}

//---------------------------------------------------------------------------
void TMessageForm::SetType(const UINT Value)
{
  if (FType != Value) {
    FType = Value;
    AnsiString name;
    switch (FType) {
      case MB_ICONWARNING     : name = "IDB_ICONEXCL"; break;
      case MB_ICONASTERISK    : name = "IDB_ICONINFO"; break;
      case MB_ICONHAND        : name = "IDB_ICONSTOP"; break;
      case MB_ICONQUESTION    : name = "IDB_ICONQUEST"; break;
    }
    Graphics::TBitmap* bmp = NULL;
    try {
      bmp = new Graphics::TBitmap();
      bmp->LoadFromResourceName(0, name);
      Transp->Width = bmp->Width;
      Transp->Height = bmp->Height;
      Transp->BkColor = bmp->TransparentColor;
      Transp->AddMasked(bmp, bmp->Canvas->Pixels[0][bmp->Height - 1]);
      Transp->GetBitmap(0, Image->Picture->Bitmap);
      Transp->Clear();
      delete bmp; bmp = NULL;
    } catch (Exception& e) {
      if (bmp) delete bmp;
      throw;
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TMessageForm::FormShow(TObject *Sender)
{
  AnsiString s = "";
  if (FType != MB_ICONQUESTION) {
    if (*FMessage.AnsiLastChar() != '.') s = '.';
  } else {
    s = " ?";
  }
  MessageL->AutoSize = false;
  MessageL->Width = 329;
  MessageL->Height = 93;
  MessageL->Caption = FMessage.Trim() + s;
  MessageL->AutoSize = true;
  int height;
  if (MessageL->Top + MessageL->Height > Image->Top + Image->Height) {
    height = MessageL->Top + MessageL->Height + BottomShape->Height + 8 + 13;
    BottomShape->Top = MessageL->Top + MessageL->Height + 8;
  }
  else {
    height = Image->Top + Image->Height + BottomShape->Height + 8 + 13;
    BottomShape->Top = Image->Top + Image->Height + 8;
  }
  OkB->Top = BottomShape->Top + BottomShape->Pen->Width + (BottomShape->Height - OkB->Height)/2;
  Height = height + 20;
  KoefHeight = (double)height / (double)4;
  DeleteObject(rgn);
  rgn = CreateRoundRectRgn(0, 0, Width, KoefHeight * 4, KoefHeight, KoefHeight);
  SetWindowRgn(Handle, rgn, true);
  BigShape->Left   = 0;
  BigShape->Top    = 0;
  BigShape->Width  = ClientWidth - 1;
  BigShape->Height = KoefHeight * 4 - 1;
  MessageBeep(FType);
}

//---------------------------------------------------------------------------
void TMessageForm::SetMessage(const AnsiString Value)
{
  if (FMessage != Value) {
    FMessage = Value;
  }
}

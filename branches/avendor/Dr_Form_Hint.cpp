//---------------------------------------------------------------------------
#include <vcl.h>
#include <grids.hpp>
#pragma hdrstop

#include "Dr_Form_Hint.h"
#include "Form_Rubber.h"

#include "Misc.h"
//---------------------------------------------------------------------------
#include "DrButton.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall THintForm::THintForm(TComponent* Owner):
  TForm(Owner),
  rgn(NULL),
  bDesktopCenter(true),
  bActive(true),
  FMouseDown(false),
  shadowWidth(2)
{
  CaptionL->Caption = Caption;
  CaptionS->Align = alNone;
  BorderS->Align  = alNone;
  ShadowS->Align  = alNone;
  Resize();
}

__fastcall THintForm::~THintForm()
{
  if (rgn) DeleteObject(rgn);
}

void __fastcall THintForm::FormShow(TObject *Sender)
{
  if (bDesktopCenter) doDesktopCenter(this);
}

void __fastcall THintForm::CloseMClick(TObject *Sender)
{
  ExitBClick(NULL);
}

void __fastcall THintForm::ExitBClick(TObject *Sender)
{
  Close();
}

void __fastcall THintForm::CaptionSMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ( Shift.Contains(ssRight) ) {
    TPoint point;
    point.x = X;
    point.y = Y;
    point = ((TControl*)Sender)->ClientToScreen(point);
    CloseM->Default = false;
    ClosePM->Popup(point.x, point.y);
  }
}
void THintForm::CheckSize()
{
  TRect rect;
  if (SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0)) {
    if (Left + Width > rect.Width() - 1)
      Left = rect.Width() - Width - 1;
    if (Left < rect.left + 1)
      Left = rect.left + 1;
    if (Top + Height > rect.Height() - 1)
      Top = rect.Height() - Height - 1;
    if (Top < rect.top + 1)
      Top = rect.top + 1;
  }
}

void __fastcall THintForm::Show()
{
  CheckSize();
  TForm::Show();
}

int __fastcall THintForm::ShowModal()
{
  CheckSize();
  return TForm::ShowModal();
}

void __fastcall THintForm::Resize()
{
#define captionLeft 5

  TForm::Resize();
  if (rgn)
    DeleteObject(rgn);
  CaptionS->Brush->Color = (TColor)GetSysColor(bActive ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION);
  CaptionL->Font->Color  = (TColor)GetSysColor(bActive ? COLOR_CAPTIONTEXT   : COLOR_INACTIVECAPTIONTEXT);
  ExitB->Font->Color     = CaptionL->Font->Color;
  ExitB->Enabled         = bActive;
  int clientWidth   = Width - shadowWidth;
  int clientHeight  = Height - shadowWidth;
  int captionHeight = CaptionS->Height - 1;
  POINT points[10];
  points[0].x = 0;
  points[0].y = 0;
  points[1].x = clientWidth;
  points[1].y = 0;
  points[2].x = clientWidth;
  points[2].y = shadowWidth;
  points[3].x = Width;
  points[3].y = shadowWidth;
  points[4].x = Width;
  points[4].y = Height;
  points[5].x = shadowWidth + captionLeft;
  points[5].y = Height;
  points[6].x = shadowWidth + captionLeft;
  points[6].y = clientHeight;
  points[7].x = captionLeft;
  points[7].y = clientHeight;
  points[8].x = captionLeft;
  points[8].y = captionHeight + 1;
  points[9].x = 0;
  points[9].y = captionHeight + 1;
  rgn = CreatePolygonRgn(points, 10, ALTERNATE);
  SetWindowRgn(Handle, rgn, true);
  if ( Tag ) {
    if ( Tag-1 < RubberStudio->Images->Count ) {
      CaptionImage->Picture->Assign( NULL );
      CaptionImage->Canvas->Brush->Color = CaptionS->Brush->Color;
      CaptionImage->Canvas->FillRect( TRect( 0, 0, 16, 16 ) );
      RubberStudio->Images->GetBitmap( Tag-1, CaptionImage->Picture->Bitmap );
    }
    CaptionL->Left = 24;
  } else {
    CaptionL->Left = 4;
  }
  CaptionS->Left  = 0;
  CaptionS->Top   = 0;
  CaptionS->Width = clientWidth;
  ExitB->Left     = clientWidth - ExitB->Width - 5;
  BorderS->Left   = captionLeft;
  BorderS->Top    = captionHeight;
  BorderS->Width  = clientWidth - captionLeft;
  BorderS->Height = clientHeight - captionHeight;
  ShadowS->Left   = shadowWidth + captionLeft;
  ShadowS->Top    = shadowWidth;
  ShadowS->Width  = clientWidth - captionLeft;
  ShadowS->Height = clientHeight;
}

AnsiString __fastcall THintForm::GetCaption()
{
  return TForm::Caption;
}

void __fastcall THintForm::SetCaption(AnsiString Value)
{
  TForm::Caption = Value;
  CaptionL->Caption = Value;
}

void __fastcall THintForm::WndProc(Messages::TMessage &Message)
{
  TForm::WndProc( Message );
  switch ( Message.Msg ) {
    case WM_LBUTTONDOWN  :
    case WM_LBUTTONDBLCLK: {
      int x = Message.LParamLo;
      int y = Message.LParamHi;
      bool inCaptionImage = Tag &&
                            x > CaptionImage->Left && x < CaptionImage->Left + CaptionImage->Width &&
                            y > CaptionImage->Top  && y < CaptionImage->Top + CaptionImage->Height;
      if ( !inCaptionImage ) {
        FMouseDown = true;
        dX = -x;
        dY = -y;
      }
      break;
    }
    case WM_LBUTTONUP    : FMouseDown = false; break;
    case WM_MOUSEMOVE    : if (FMouseDown) {
                             Left += Message.LParamLo + dX;
                             Top  += Message.LParamHi + dY;
                           }
                           break;

    case WM_ACTIVATE     : bActive = (Message.WParamLo == WA_ACTIVE || Message.WParamLo == WA_CLICKACTIVE);
                           Resize();
                           break;
  }
}

void __fastcall THintForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if (Key == VK_ESCAPE) ExitBClick(NULL);
  bool bErase = Key == VK_TAB && dynamic_cast<TCustomGrid*>(ActiveControl);
  if ((Key == VK_RETURN &&
       !dynamic_cast<TCustomMemo*>(ActiveControl) &&
       !dynamic_cast<TDrButton*>(ActiveControl)
      ) || (Key == VK_TAB)) {
    TWinControl* win = NULL;
    for (int i = 0; i < ControlCount; i++) {
      TControl* control = Controls[i];
      if (dynamic_cast<TWinControl*>(control)) {
        if (((TWinControl*)control)->Focused()) {
          win = FindNextControl((TWinControl*)control, true, true, true);
          break;
        }
      }
    }
    if (win) win->SetFocus();
  }
  if (bErase) Key = 0;
}

void __fastcall THintForm::CaptionImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ( Shift.Contains(ssRight) ) {
    CaptionSMouseDown( Sender, Button, Shift, X + CaptionImage->Left, Y + CaptionImage->Top );
  } else {
    if ( Tag ) {
      if ( Shift.Contains(ssLeft) && Shift.Contains(ssDouble) ) {
        ExitBClick(NULL);
      } else {
        TPoint point;
        point.x = -2;
        point.y = CaptionImage->Height + 2;
        point = CaptionImage->ClientToScreen(point);
        CloseM->Default = true;
        ClosePM->Popup(point.x, point.y);
      }
    }
  }
}


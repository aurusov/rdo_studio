//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DrButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TDrButton *)
{
  new TDrButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TDrButton::TDrButton(TComponent* Owner):
  TWinControl(Owner),
  FFlat(false),
  FFlatColor(clActiveCaption),
  FEnabled(true),
  bFocus(false),
  bMouseDown(false)
{
  canvas = new TControlCanvas();
  canvas->Control = this;
  if (dynamic_cast<TWinControl*>(Owner)) Parent = (TWinControl*)Owner;
  ControlStyle << csSetCaption;
  SetBounds(0, 0, 75, 25);
  TabStop = true;
}

__fastcall TDrButton::~TDrButton(void)
{
  delete canvas;
}

void __fastcall TDrButton::WndProc(Messages::TMessage &Message)
{
  if (!ComponentState.Contains(csDesigning)) {
    switch (Message.Msg) {
      case WM_LBUTTONDOWN  :
      case WM_LBUTTONDBLCLK: SetFocus(); bMouseDown = true; Repaint(); break;

      case WM_LBUTTONUP    : bMouseDown = false; Repaint(); break;

      case WM_SETFOCUS     : if (!bFocus) {
                               bFocus = true;
//                               SetFocus();
                               Repaint();
                             }
                             break;

      case CM_MOUSELEAVE   :
      case WM_KILLFOCUS    : bFocus = false; bMouseDown = false; Repaint(); break;

      case WM_SIZE         :
      case WM_PAINT        : TWinControl::WndProc(Message); Repaint(); return;

      case WM_KEYDOWN      : if (Message.WParam == VK_SPACE && !bMouseDown) {
                               bMouseDown = true;
                               Repaint();
                               break;
                             }
                             if (Message.WParam == VK_RETURN) {
                               TWinControl::WndProc(Message);
                               if (OnClick) OnClick(this);
                               return;
                             }
      case WM_KEYUP        : if (Message.WParam == VK_SPACE) {
                               bMouseDown = false;
                               if (OnClick) OnClick(this);
                               Repaint();
                             }
                             break;
    }
  } else {
    switch (Message.Msg) {
      case WM_LBUTTONDOWN  :
      case WM_LBUTTONDBLCLK:
      case WM_LBUTTONUP    :
      case WM_PAINT        : Repaint(); break;
    }
  }
  TWinControl::WndProc(Message);
}

void __fastcall TDrButton::SetFlat(const bool Value)
{
  if (FFlat != Value) {
    FFlat = Value;
    Repaint();
  }
}

void __fastcall TDrButton::SetFlatColor(const TColor Value)
{
  if (FFlatColor != Value) {
    FFlatColor = Value;
    Repaint();
  }
}

void __fastcall TDrButton::SetEnabled(const bool Value)
{
  if (FEnabled != Value) {
    FEnabled = Value;
    TWinControl::Enabled = Value;
    Repaint();
  }
}

void __fastcall TDrButton::Repaint(void)
{
  bool design = ComponentState.Contains(csDesigning);
  canvas->Font->Assign(Font);
  // Очистка поверхности кнопки
  TRect rect = ClientRect;
  canvas->Brush->Color = clBtnFace;
  canvas->FillRect(rect);
  // Отрисовка текста кнопки
  canvas->Brush->Style = bsClear;
  DrawText(canvas->Handle, Caption.c_str(), Caption.Length(), &rect, DT_CALCRECT);
  rect.left   += (Width - rect.right) / 2 + 1;
  rect.right  += rect.Left;
  rect.top    += (Height - rect.bottom) / 2;
  rect.bottom += rect.Top;
  if (bMouseDown && !design) {
    rect.left++;
    rect.right++;
    rect.top++;
    rect.bottom++;
  }
  if (FEnabled || design) {
    canvas->Font->Color = clWindowText;
  } else {
    canvas->Font->Color = (TColor)GetSysColor(COLOR_BTNHILIGHT);
    TRect rect2 = rect;
    rect2.left++;
    rect2.right++;
    rect2.top++;
    rect2.bottom++;
    DrawText(canvas->Handle, Caption.c_str(), Caption.Length(), &rect2, DT_SINGLELINE);
    canvas->Font->Color = (TColor)GetSysColor(COLOR_GRAYTEXT);
  }
  DrawText(canvas->Handle, Caption.c_str(), Caption.Length(), &rect, DT_SINGLELINE);
  // Подчеркивание под текстом
  if (((bFocus || bMouseDown) && !FFlat) && !design) {
    canvas->Pen->Mode  = pmCopy;
    canvas->Pen->Style = psSolid;
    canvas->Pen->Width = 1;
    rect.left = (Width - rect.Width()) / 4;
    rect.right += rect.left;
    rect.top = (Height - rect.Height()) / 4;
    rect.bottom += rect.top - 2;
    TColor colorLight  = (TColor)GetSysColor(COLOR_3DHILIGHT);
    TColor colorShadow = (TColor)GetSysColor(COLOR_3DDKSHADOW);
    //
    canvas->Pen->Color = colorLight;
    canvas->MoveTo(rect.left, rect.top + 4);
    canvas->LineTo(rect.left, rect.top);
    canvas->LineTo(rect.left + 4, rect.top);
    canvas->Pen->Color = colorShadow;
    canvas->MoveTo(rect.left + 1, rect.top + 5);
    canvas->LineTo(rect.left + 1, rect.top + 1);
    canvas->LineTo(rect.left + 5, rect.top + 1);
    //
    canvas->MoveTo(rect.right - 3, rect.top + 1);
    canvas->LineTo(rect.right + 1, rect.top + 1);
    canvas->LineTo(rect.right + 1, rect.top + 5);
    canvas->Pen->Color = colorLight;
    canvas->MoveTo(rect.right - 4, rect.top);
    canvas->LineTo(rect.right, rect.top);
    canvas->LineTo(rect.right, rect.top + 4);
    //
    canvas->Pen->Color = colorShadow;
    canvas->MoveTo(rect.left + 1, rect.bottom - 3);
    canvas->LineTo(rect.left + 1, rect.bottom + 1);
    canvas->LineTo(rect.left + 5, rect.bottom + 1);
    canvas->Pen->Color = colorLight;
    canvas->MoveTo(rect.left, rect.bottom - 4);
    canvas->LineTo(rect.left, rect.bottom);
    canvas->LineTo(rect.left + 4, rect.bottom);
    //
    canvas->MoveTo(rect.right, rect.bottom - 4);
    canvas->LineTo(rect.right, rect.bottom);
    canvas->LineTo(rect.right - 4, rect.bottom);
    canvas->Pen->Color = colorShadow;
    canvas->MoveTo(rect.right + 1, rect.bottom - 3);
    canvas->LineTo(rect.right + 1, rect.bottom + 1);
    canvas->LineTo(rect.right - 3, rect.bottom + 1);
  }
  // Отрисовка ободка кнопки
  rect = ClientRect;
  UINT edge;
  if (!bFocus && !bMouseDown) {
    edge = BDR_RAISEDOUTER;
  } else {
    if (bFocus && !bMouseDown) {
      edge = BDR_RAISEDINNER;
    } else {
      if (bMouseDown) {
        edge = BDR_SUNKENOUTER;
      }
    }
  }
  if (!Flat || (FFlat && (bFocus || bMouseDown))) {
    if (design) edge = BDR_RAISEDOUTER;
    DrawEdge(canvas->Handle, &rect, edge, BF_RECT);
  } else {
    canvas->Pen->Color = FFlatColor;
    canvas->Pen->Mode  = pmCopy;
    canvas->Pen->Style = psSolid;
    canvas->Pen->Width = 1;
    canvas->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
  }
}
//---------------------------------------------------------------------------
namespace Drbutton
{
  void __fastcall PACKAGE Register()
  {
    TComponentClass classes[1] = {__classid(TDrButton)};
    RegisterComponents("Dron", classes, 0);
  }
}
//---------------------------------------------------------------------------


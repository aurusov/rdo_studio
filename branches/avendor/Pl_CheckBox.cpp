//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Pl_CheckBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TPaulCheckBox::TPaulCheckBox(TComponent* Owner) : TWinControl(Owner),
            FChecked(false)
{
  canvas = new TControlCanvas();
  canvas->Control = this;
  if (dynamic_cast<TWinControl*>(Owner))
    Parent = (TWinControl*)Owner;
  ControlStyle << csSetCaption << csOpaque << csDoubleClicks;
  SetBounds(0, 0, 97, 17);
  TabStop = true;
}

//---------------------------------------------------------------------------
__fastcall TPaulCheckBox::~TPaulCheckBox(void)
{
  delete canvas;
}

//---------------------------------------------------------------------------
void __fastcall TPaulCheckBox::SetEnabled(const bool Value)
{
  TControl::SetEnabled(Value);
  Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TPaulCheckBox::WndProc(Messages::TMessage &Message)
{
  switch (Message.Msg) {

    case WM_LBUTTONDOWN   :
    case WM_LBUTTONDBLCLK : if (Enabled) {
                              SetFocus();
                              Repaint();
                            }
                            break;

    case WM_SETFOCUS      :
    case WM_KILLFOCUS     : Repaint(); break;

    case CM_TEXTCHANGED   :
    case CM_SYSCOLORCHANGE:
    case CM_FONTCHANGE    :
    case WM_SIZE          :
    case WM_PAINT         : TWinControl::WndProc(Message); Repaint(); return;

    case WM_KEYUP         : if (Message.WParam == VK_SPACE) {
                              Click();
                              Repaint();
                              break;
                            }
  }
  TWinControl::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TPaulCheckBox::SetDisabledColor(const TColor Value)
{
  if (FDisabledColor != Value) {
    FDisabledColor = Value;
    Repaint();
  }
}

//---------------------------------------------------------------------------
void __fastcall TPaulCheckBox::SetBackColor(const TColor Value)
{
  if (FBackColor != Value) {
    FBackColor = Value;
    Repaint();
  }
}

//---------------------------------------------------------------------------
void __fastcall TPaulCheckBox::SetChecked(const bool Value)
{
  if (FChecked != Value) {
    FChecked = Value;
    Repaint();
  }
}

//---------------------------------------------------------------------------
void __fastcall TPaulCheckBox::Click(void)
{
  if (Enabled) {
    FChecked = !FChecked;
    Repaint();
    TWinControl::Click();
  }
}

//---------------------------------------------------------------------------
void __fastcall TPaulCheckBox::Repaint(void)
{
  TRect rect = ClientRect;
  TRect focusrect = rect;
  focusrect.left = rect.left + 18;

  canvas->Font->Assign(Font);
  canvas->Brush->Color = FBackColor;
  canvas->Brush->Style = bsSolid;
  canvas->FillRect(rect);

  AnsiString name = "IDB_CHECK";
  if (FChecked)
    name += "CHECKED";
  if (!Enabled)
    name += "D";

  Graphics::TBitmap* bmp = NULL;

  int offset;

  try {
    bmp = new Graphics::TBitmap();
    bmp->LoadFromResourceName(0, name);
    int x = 0;
    canvas->Draw(x, 1, bmp);
    offset = x + bmp->Width + 4;
    delete bmp; bmp = NULL;
  } catch (Exception& e) {
    if (bmp) delete bmp;
  }

  rect.left += offset;
  rect.right -= 2;

  if (!Enabled) {
    canvas->Font->Color = FDisabledColor;
    TFontStyles fs;
    fs.Clear();
    canvas->Font->Style = fs;
  }

  TRect calcrect = rect;
  DrawText(canvas->Handle, Caption.c_str(), Caption.Length(), &calcrect, DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
  DrawText(canvas->Handle, Caption.c_str(), Caption.Length(), &rect, DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

  InflateRect(&focusrect, 0, -1);
  focusrect.right = focusrect.left + calcrect.right - calcrect.left + 2;
  TControlCanvas* cnv = NULL;
  if (Focused()) {
    try {
      cnv = new TControlCanvas();
      cnv->Control = this;
      DrawFocusRect(cnv->Handle, &focusrect);
      delete cnv; cnv = NULL;
    } catch (Exception& e) {
      if (cnv) delete cnv;
      DrawFocusRect(canvas->Handle, &focusrect);
    }
  }
}


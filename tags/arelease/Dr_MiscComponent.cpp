//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_MiscComponent.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------- TDrComboEdit
//---------------------------------------------------------------------------
__fastcall TDrComboEdit::TDrComboEdit(TComponent *Owner):
  TWinControl(Owner),
  Edit(NULL),
  SpeedButton(NULL)
{
  Edit = new TEdit(this);
  Edit->OnChange   = EditChange;
  Edit->OnKeyDown  = EditKeyDown;
  Edit->OnKeyPress = EditKeyPress;
  Edit->Parent     = this;
  Edit->Left       = 0;
  Edit->Top        = 0;

  SpeedButton = new TSpeedButton(this);
  SpeedButton->OnClick = SpeedButtonClick;
  SpeedButton->Parent  = this;
  SpeedButton->Caption = "...";
  SpeedButton->Width   = 18;
  SpeedButton->Top     = 0;

  BevelInner = bvNone;
  BevelOuter = bvNone;
  Parent = (TWinControl*)Owner;

  Edit->Ctl3D       = false;
  Edit->ParentCtl3D = false;
  Width       = Edit->Width;
  Height      = Edit->Height;
  OnResize    = PanelResize;
  OnResize(this);
}

__fastcall TDrComboEdit::~TDrComboEdit()
{
  if (Edit)        delete Edit;
  if (SpeedButton) delete SpeedButton;
}

void __fastcall TDrComboEdit::PanelResize(TObject *Sender)
{
  Edit->Width         = Width - SpeedButton->Width;
  Edit->Height        = Height;
  SpeedButton->Left   = Edit->Width;
  SpeedButton->Height = Height;
}

void __fastcall TDrComboEdit::EditChange(TObject *Sender)
{
  if (FOnChange) OnChange(this);
}

void __fastcall TDrComboEdit::EditKeyDown(TObject *Sender, Word &Key, TShiftState Shift)
{
  if (OnKeyDown) OnKeyDown(this, Key, Shift);
}

void __fastcall TDrComboEdit::EditKeyPress(TObject *Sender, char &Key)
{
  if (OnKeyPress) OnKeyPress(this, Key);
}

void __fastcall TDrComboEdit::SpeedButtonClick(TObject *Sender)
{
  if (FOnClick) OnClick(this);
}
//---------------------------------------------------------------------------
//---------- TDrHilightLabel
//---------------------------------------------------------------------------
__fastcall TDrHilightLabel::TDrHilightLabel(TComponent* Owner):
  TStaticText(Owner),
  bDoHilight(false)
{
  NormalColor  = Font->Color;
  NormalCursor = crDefault;
  NormalStyle.Clear();
  HilightColor  = Font->Color;
  HilightCursor = crDefault;
  HilightStyle.Clear();
}

void __fastcall TDrHilightLabel::WndProc(Messages::TMessage &Message)
{
  switch (Message.Msg) {
    case CM_MOUSEENTER: bDoHilight = true;  UpdateHilight(); break;
    case CM_MOUSELEAVE: bDoHilight = false; UpdateHilight(); break;
    case WM_CREATE    : UpdateHilight(); break;
  }
  TStaticText::WndProc(Message);
}

void TDrHilightLabel::UpdateHilight()
{
  if (bDoHilight) {
    Font->Color = HilightColor;
    Cursor      = HilightCursor;
    Font->Style = HilightStyle;
  } else {
    Font->Color = NormalColor;
    Cursor      = NormalCursor;
    Font->Style = NormalStyle;
  }
}


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HighlightText.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall THighlightText::THighlightText(TComponent* Owner):
  TStaticText(Owner),
  FHighlight(true)
{
  NormColor  = Font->Color;
  NormCursor = crDefault;
  NormStyle.Clear();
  HighlColor  = Font->Color;
  HighlCursor = crDefault;
  HighlStyle.Clear();
  OnHighLight = NULL;
  Image = NULL;
}

//---------------------------------------------------------------------------
void __fastcall THighlightText::WndProc(Messages::TMessage &Message)
{
  switch (Message.Msg) {
    case CM_MOUSEENTER: UpdateText(true); break;
    case CM_MOUSELEAVE: UpdateText(false); break;
    case WM_CREATE    : UpdateText(false); break;
  }
  TStaticText::WndProc(Message);
}

//---------------------------------------------------------------------------
void THighlightText::UpdateText(const bool Value)
{
  if ((Value || FStayHighlighted) && FHighlight) {
    Font->Color = HighlColor;
    Cursor      = HighlCursor;
    Font->Style = HighlStyle;
  } else {
    Font->Color = NormColor;
    Cursor      = NormCursor;
    Font->Style = NormStyle;
  }
  if (OnHighLight) OnHighLight(this, Value && FHighlight);
}

//---------------------------------------------------------------------------
void THighlightText::SetHighlight(const bool Value)
{
  if (FHighlight != Value) {
    FHighlight = Value;
    UpdateText(false);
  }
}

//---------------------------------------------------------------------------
void THighlightText::SetStayHighlighted(const bool Value)
{
  if (FStayHighlighted != Value) {
    FStayHighlighted = Value;
    UpdateText(false);
  }
}

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HighlightImage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall THighlightImage::THighlightImage(TComponent* Owner): TImage(Owner),
            FHighlight(true),
            NormImageIndex(-1),
            HighlImageIndex(-1)
{
  NormCursor = crDefault;
  HighlCursor = crDefault;
  OnHighLight = NULL;
  Images = NULL;
  HotImages = NULL;
}

//---------------------------------------------------------------------------
void __fastcall THighlightImage::WndProc(Messages::TMessage &Message)
{
  switch (Message.Msg) {
    case CM_MOUSEENTER: UpdateImage(true); break;
    case CM_MOUSELEAVE: UpdateImage(false); break;
    case WM_CREATE    : UpdateImage(false); break;
  }
  TImage::WndProc(Message);
}

//---------------------------------------------------------------------------
void THighlightImage::UpdateImage(const bool Value)
{
  if (Value && FHighlight) {
    Canvas->FillRect(TRect(0, 0, Width, Height));
    if (HotImages)
      HotImages->Draw(Canvas, 0, 0, HighlImageIndex, true);
    Cursor      = HighlCursor;
  } else {
    Canvas->FillRect(TRect(0, 0, Width, Height));
    if (Images)
      Images->Draw(Canvas, 0, 0, NormImageIndex, true);
    Cursor      = NormCursor;
  }
  if (OnHighLight) OnHighLight(this, Value && FHighlight);
}

//---------------------------------------------------------------------------
void THighlightImage::SetHighlight(const bool Value)
{
  if (FHighlight != Value) {
    FHighlight = Value;
    UpdateImage(false);
  }
}

//---------------------------------------------------------------------------
void __fastcall THighlightImage::SetParent(TWinControl* AParent)
{
  TImage::SetParent(AParent);
  if (AParent)
    UpdateImage(false);
}

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_PictureViewer.h"
#include "ConstRes.h"
#include "Misc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------- TPictureViewer
//---------------------------------------------------------------------------
__fastcall TPictureViewer::TPictureViewer(TControl* AOwner, const TColor BackGroundColor, const TColor BorderColor)
           : TWinControl(AOwner),
             FBackGroundColor(BackGroundColor),
             FBorderColor(BorderColor)
{
  int border = 0;
  if (dynamic_cast<TPanel*>(AOwner)) {
    TPanel* panel = (TPanel*)AOwner;
    if (panel->BevelOuter != bvNone) border++;
    if (panel->BevelInner != bvNone) border++;
    border *= panel->BevelWidth;
    border += panel->BorderWidth;
  }
  Left   = border;
  Top    = border;
  border *= 2;
  Width  = AOwner->ClientWidth  - border;
  Height = AOwner->ClientHeight - border;

  Image = new TImage(this); Image->Parent = this;
  ShapeZoomPageB = new TShape(this); ShapeZoomPageB->Parent = this;
  ZoomPageB = new TSpeedButton(this); ZoomPageB->Parent = this; ZoomPageB->Cursor = crHandPoint;
  ShapeZoomOneB = new TShape(this); ShapeZoomOneB->Parent = this;
  ZoomOneB  = new TSpeedButton(this); ZoomOneB->Parent = this; ZoomOneB->Cursor = crHandPoint;
  ShapeFullViewB = new TShape(this); ShapeFullViewB->Parent = this;
  FullViewB = new TSpeedButton(this); FullViewB->Parent = this; FullViewB->Cursor = crHandPoint;

  ZoomPageB->Flat = true;
  ZoomPageB->ParentShowHint = false;
  ZoomPageB->ShowHint = true;
  ZoomPageB->Hint = GetString(sZoomFit);
  ZoomOneB->Flat = true;
  ZoomOneB->ParentShowHint = false;
  ZoomOneB->ShowHint = true;
  ZoomOneB->Hint = GetString(sZoomAct);
  FullViewB->Flat = true;
  FullViewB->ParentShowHint = false;
  FullViewB->ShowHint = true;
  FullViewB->Hint = GetString(sFullReview);
  ZoomPageB->Glyph->LoadFromResourceName(0, "IDB_ZOOMPAGE");
  ZoomOneB->Glyph->LoadFromResourceName(0, "IDB_ZOOMONE");
  FullViewB->Glyph->LoadFromResourceName(0, "IDB_ZOOMFULL");

  ShapeZoomPageB->Brush->Color = FBackGroundColor;
  ShapeZoomPageB->Pen->Color = FBorderColor;
  ShapeZoomOneB->Brush->Color = FBackGroundColor;
  ShapeZoomOneB->Pen->Color = FBorderColor;
  ShapeFullViewB->Brush->Color = FBackGroundColor;
  ShapeFullViewB->Pen->Color = FBorderColor;

  Image->Left   = 0;
  Image->Top    = 0;
  Image->Width  = Width;
  Image->Height = Height;

  FullViewB->Width     = 20;
  ShapeFullViewB->Width = FullViewB->Width + 2;
  FullViewB->Height    = 20;
  ShapeFullViewB->Height = FullViewB->Height + 2;
  FullViewB->NumGlyphs = 2;
  ZoomOneB->Width      = 20;
  ShapeZoomOneB->Width = ZoomOneB->Width + 2;
  ZoomOneB->Height     = 20;
  ShapeZoomOneB->Height = ZoomOneB->Height + 2;
  ZoomOneB->NumGlyphs  = 2;
  ZoomPageB->Width     = 20;
  ShapeZoomPageB->Width = ZoomPageB->Width + 2;
  ZoomPageB->Height    = 20;
  ShapeZoomPageB->Height = ZoomPageB->Height + 2;
  ZoomPageB->NumGlyphs = 2;
  FullViewB->Left  = Width - FullViewB->Width - 1;
  ShapeFullViewB->Left = FullViewB->Left - 1;
  FullViewB->Top   = Height - FullViewB->Height - 1;
  ShapeFullViewB->Top = FullViewB->Top - 1;
  ZoomOneB->Left   = FullViewB->Left - ZoomOneB->Width - 1;
  ShapeZoomOneB->Left = ZoomOneB->Left - 1;
  ZoomOneB->Top    = FullViewB->Top;
  ShapeZoomOneB->Top = ShapeFullViewB->Top;
  ZoomPageB->Left  = ZoomOneB->Left - ZoomPageB->Width - 1;
  ShapeZoomPageB->Left = ZoomPageB->Left - 1;
  ZoomPageB->Top   = FullViewB->Top;
  ShapeZoomPageB->Top = ShapeFullViewB->Top;
  ZoomPageB->OnClick = ZoomPageBClick;
  ZoomOneB->OnClick  = ZoomOneBClick;
  FullViewB->OnClick = FullViewBClick;

  Parent = (TWinControl*)AOwner;
}

//---------------------------------------------------------------------------
__fastcall TPictureViewer::~TPictureViewer()
{
  delete ZoomPageB;
  delete ZoomOneB;
  delete FullViewB;
  delete ShapeZoomPageB;
  delete ShapeZoomOneB;
  delete ShapeFullViewB;
  delete Image;
}

//---------------------------------------------------------------------------
void TPictureViewer::UpdateImage(const AnsiString FileName, const bool Transparent)
{
  ZoomPageB->Visible = false;
  ZoomOneB->Visible  = false;
  FullViewB->Visible = false;
  ShapeZoomPageB->Visible = false;
  ShapeZoomOneB->Visible  = false;
  ShapeFullViewB->Visible = false;
  if (FileName.data()) {
    LoadTransparentBitmap(Image, FileName, Transparent);
    if (Image->Picture->Bitmap->Width > Width || Image->Picture->Bitmap->Height > Height) {
      ZoomPageB->Visible = true;
      ZoomOneB->Visible  = true;
      FullViewB->Visible = true;
      ShapeZoomPageB->Visible = true;
      ShapeZoomOneB->Visible  = true;
      ShapeFullViewB->Visible = true;
      Image->Center = false;
      ZoomPageBClick(NULL);
    } else {
      Image->Center  = true;
      ZoomOneBClick(NULL);
    }
    Image->Visible = true;
  } else {
    Image->Visible = false;
  }
}

//---------------------------------------------------------------------------
void __fastcall TPictureViewer::ZoomPageBClick(TObject *Sender)
{
  ZoomPageB->Enabled = false;
  float kx, ky, kwidth;
  ky = (float)Height / ((float)Image->Picture->Bitmap->Height);
  kx = (float)Width / ((float)Image->Picture->Bitmap->Width);
  kwidth = (float)Image->Picture->Bitmap->Height / (float)Image->Picture->Bitmap->Width;
  if (kx > ky) {
    Image->Height = Height;
    Image->Width  = Image->Height / kwidth;
  } else {
    Image->Width = Width;
    Image->Height = Image->Width * kwidth;
  };
  Image->Left   = (Width - Image->Width) / 2;
  Image->Top    = (Height - Image->Height) / 2;
  Image->Stretch = true;
  ZoomOneB->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TPictureViewer::ZoomOneBClick(TObject *Sender)
{
  ZoomPageB->Enabled = true;
  Image->Stretch = false;
  Image->SetBounds(0, 0, Width, Height);
  ZoomOneB->Enabled = false;
}

//---------------------------------------------------------------------------
void __fastcall TPictureViewer::FullViewBClick(TObject *Sender)
{
  TForm* ImageFullView = new TForm(this);
  ImageFullView->Color = FBackGroundColor;
  TImage* FullImage = new TImage(ImageFullView);
  FullImage->Transparent = Image->Transparent;
  try {
    ImageFullView->BorderStyle = bsToolWindow;
    ImageFullView->FormStyle   = fsStayOnTop;
    ImageFullView->Position    = poScreenCenter;
    ImageFullView->Caption     = GetString(sPicturePreview);
    FullImage->Left = 0;
    FullImage->Top  = 0;
    FullImage->AutoSize = true;
    FullImage->Picture = Image->Picture;
    ImageFullView->ClientWidth  = FullImage->Width;
    ImageFullView->ClientHeight = FullImage->Height;
    FullImage->Parent = ImageFullView;
    FullViewB->Enabled = false;
    ImageFullView->ShowModal();
    FullViewB->Enabled = true;
    delete FullImage;     FullImage = NULL;
    delete ImageFullView; ImageFullView = NULL;
    Image->Update();
  } catch (Exception &e) {
    if (FullImage)     delete FullImage;
    if (ImageFullView) delete ImageFullView;
    FullViewB->Enabled = true;
  }
}

//---------------------------------------------------------------------------
void TPictureViewer::UpdateSize()
{
  int border = 0;
  if (dynamic_cast<TPanel*>(Parent)) {
    TPanel* panel = (TPanel*)Parent;
    if (panel->BevelOuter != bvNone) border++;
    if (panel->BevelInner != bvNone) border++;
    border *= panel->BevelWidth;
    border += panel->BorderWidth;
  }
  Left   = border;
  Top    = border;
  border *= 2;
  Width  = Parent->ClientWidth  - border;
  Height = Parent->ClientHeight - border;

  Image->Width  = Width;
  Image->Height = Height;

  FullViewB->Left  = Width - FullViewB->Width - 1;
  ShapeFullViewB->Left = FullViewB->Left - 1;
  FullViewB->Top   = Height - FullViewB->Height - 1;
  ShapeFullViewB->Top = FullViewB->Top - 1;
  ZoomOneB->Left   = FullViewB->Left - ZoomOneB->Width - 1;
  ShapeZoomOneB->Left = ZoomOneB->Left - 1;
  ZoomOneB->Top    = FullViewB->Top;
  ShapeZoomOneB->Top = ShapeFullViewB->Top;
  ZoomPageB->Left  = ZoomOneB->Left - ZoomPageB->Width - 1;
  ShapeZoomPageB->Left = ZoomPageB->Left - 1;
  ZoomPageB->Top   = FullViewB->Top;
  ShapeZoomPageB->Top = ShapeFullViewB->Top;
}

//---------------------------------------------------------------------------
void TPictureViewer::UpdateStrings()
{
  ZoomPageB->Hint = GetString(sZoomFit);
  ZoomOneB->Hint = GetString(sZoomAct);
  FullViewB->Hint = GetString(sFullReview);
}

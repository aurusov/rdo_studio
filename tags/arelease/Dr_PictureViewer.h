//---------------------------------------------------------------------------
#ifndef Dr_PictureViewerH
#define Dr_PictureViewerH
//---------------------------------------------------------------------------
#include <classes.hpp>
#include <controls.hpp>
#include <buttons.hpp>
#include <extctrls.hpp>
//---------------------------------------------------------------------------
//---------- TPictureViewer
//---------------------------------------------------------------------------
class TPictureViewer : public TWinControl
{
private:
  int ImageWidth;
  TImage* Image;
  TSpeedButton* ZoomPageB;
  TSpeedButton* ZoomOneB;
  TSpeedButton* FullViewB;
  TShape* ShapeZoomPageB;
  TShape* ShapeZoomOneB;
  TShape* ShapeFullViewB;
  void __fastcall ZoomPageBClick(TObject *Sender);
  void __fastcall ZoomOneBClick(TObject *Sender);
  void __fastcall FullViewBClick(TObject *Sender);
  TColor FBackGroundColor;
  TColor FBorderColor;
public:
  __fastcall TPictureViewer(TControl* AOwner, const TColor BackGroundColor, const TColor BorderColor);
  __fastcall ~TPictureViewer();
  void UpdateImage(const AnsiString FileName, const bool Transparent = true);
  void UpdateSize();
  void UpdateStrings();
};
//---------------------------------------------------------------------------
#endif


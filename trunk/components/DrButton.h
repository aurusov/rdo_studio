//---------------------------------------------------------------------------
#ifndef DrButtonH
#define DrButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TDrButton : public TWinControl
{
private:
  TControlCanvas* canvas;
  bool bFocus;
  bool bMouseDown;
  virtual void __fastcall WndProc(Messages::TMessage &Message);

  bool FFlat;
  void __fastcall SetFlat(const bool Value);
  TColor FFlatColor;
  void __fastcall SetFlatColor(const TColor Value);

  bool FEnabled;
  virtual void __fastcall SetEnabled(const bool Value);
public:
  __fastcall TDrButton(TComponent* Owner);
  __fastcall ~TDrButton(void);
  virtual void __fastcall Repaint(void);
  __property bool Enabled = {read=FEnabled, write=SetEnabled};
__published:
  __property bool Flat = {read=FFlat, write=SetFlat};
  __property TColor FlatColor = {read=FFlatColor, write=SetFlatColor};
  __property Caption;
  __property Font;
  __property TabOrder;
  __property TabStop;
  __property OnClick;
};
//---------------------------------------------------------------------------
#endif


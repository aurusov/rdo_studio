//---------------------------------------------------------------------------
#ifndef Dr_MiscComponentH
#define Dr_MiscComponentH
//---------------------------------------------------------------------------
#include <classes.hpp>
#include <controls.hpp>
#include <extctrls.hpp>
#include <stdctrls.hpp>
#include <classes.hpp>
#include <systobj.h>
#include <buttons.hpp>
#include <graphics.hpp>
#pragma hdrstop
//---------------------------------------------------------------------------
//---------- TDrComboEdit
//---------------------------------------------------------------------------
class TDrComboEdit : public TWinControl
{
private:
  TNotifyEvent FOnClick, FOnChange;
  void __fastcall PanelResize(TObject *Sender);
  void __fastcall EditChange(TObject *Sender);
  void __fastcall EditKeyDown(TObject *Sender, Word &Key, TShiftState Shift);
  void __fastcall EditKeyPress(TObject *Sender, char &Key);
  void __fastcall SpeedButtonClick(TObject *Sender);
public:
  __fastcall TDrComboEdit(TComponent* Owner);
  __fastcall ~TDrComboEdit();

  TEdit* Edit;
  TSpeedButton* SpeedButton;

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange };
  __property TNotifyEvent OnClick  = {read = FOnClick,  write = FOnClick };
  __property OnExit;
  __property OnKeyDown;
  __property OnKeyPress;
};
//---------------------------------------------------------------------------
//---------- TDrHilightLabel
//---------------------------------------------------------------------------
class TDrHilightLabel : public TStaticText
{
private:
  bool bDoHilight;
  void __fastcall WndProc(Messages::TMessage &Message);
  void UpdateHilight();
public:
  __fastcall TDrHilightLabel(TComponent* Owner);
  TColor      NormalColor;
  TCursor     NormalCursor;
  TFontStyles NormalStyle;
  TColor      HilightColor;
  TCursor     HilightCursor;
  TFontStyles HilightStyle;
};
//---------------------------------------------------------------------------
#endif


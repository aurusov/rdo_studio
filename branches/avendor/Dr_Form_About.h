//---------------------------------------------------------------------------
#ifndef Dr_Form_AboutH
#define Dr_Form_AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#pragma hdrstop

#include "Dr_MiscComponent.h"
//---------------------------------------------------------------------------
class TAboutForm : public TForm
{
__published:
  TLabel *Title;
  TLabel *Innotech;
  TLabel *InnotechEmailL;
  TLabel *MSTU;
  TLabel *MSTUEmailL;
  TShape *TopShape;
  TShape *LeftShape;
  TShape *BottomShape;
  TShape *BigShape;
  TImage *Image;
private:
  HRGN rgn;

  TDrHilightLabel* IliosEmail;
  TDrHilightLabel* MSTUEmail;
  TDrHilightLabel* OkB;

  void __fastcall IliosEmailClick(TObject *Sender);
  void __fastcall MSTUEmailClick(TObject *Sender);
  void __fastcall OkBClick(TObject *Sender);

  bool FMouseDown;
  int dX, dY;
  void __fastcall WndProc(Messages::TMessage &Message);
public:
  __fastcall TAboutForm();
  __fastcall ~TAboutForm();
};
//---------------------------------------------------------------------------
#endif


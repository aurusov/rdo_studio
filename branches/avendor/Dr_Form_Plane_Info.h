//---------------------------------------------------------------------------
#ifndef Dr_Form_Plane_InfoH
#define Dr_Form_Plane_InfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Dr_Form_Hint.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include "DrButton.h"
//---------------------------------------------------------------------------
class TPlaneInfo : public THintForm
{
__published:
  TTrackBar *PrecisionTB;
  TLabel *PrecisionL1;
  TLabel *PrecisionL2;
  TLabel *PrecisionL3;
  TDrButton *OkB;
  TDrButton *CancelB;
  TLabel *KoefL;
  TBevel *KoefBevel;
  TLabel *Koef1L;
  TEdit *Koef1E;
  TShape *Koef1S;
  TLabel *Koef2L;
  TEdit *Koef2E;
  TShape *Koef2S;
  TLabel *Koef3L;
  TEdit *Koef3E;
  TShape *Koef3S;
  TLabel *Koef4L;
  TEdit *Koef4E;
  TShape *Koef4S;
  TDrButton *KoefLoadB;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall PrecisionTBChange(TObject *Sender);
  void __fastcall KoefLoadBClick(TObject *Sender);
public:
  __fastcall TPlaneInfo(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif


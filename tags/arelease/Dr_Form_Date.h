//---------------------------------------------------------------------------
#ifndef Dr_Form_DateH
#define Dr_Form_DateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Dr_Form_Hint.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "DrButton.h"
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TDateForm : public THintForm
{
__published:
  TDBGrid *DBGrid;
  TShape *DBGridS;
  TDBEdit *DateValue;
  TLabel *DateValueL;
  TShape *DateValueS;
  TBevel *Bevel1;
  TBevel *Bevel2;
  TDrButton *DelB;
  TDrButton *InsB;
  TDrButton *SaveB;
  TDrButton *CancelB;
  TDrButton *OkB;
  TDrButton *CancelExitB;
  TLabel *VulcanCostTariff4L;
  TDBEdit *VulcanCostTariff4;
  TShape *VulcanCostTariff4S;
  TLabel *VulcanCostTariff5L;
  TShape *VulcanCostTariff5S;
  TDBEdit *VulcanCostTariff5;
  TLabel *CutCostTariffL;
  TDBEdit *CutCostTariff;
  TShape *CutCostTariffS;
  TLabel *ProgramL;
  TLabel *Program;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelExitBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall SaveBClick(TObject *Sender);
  void __fastcall InsBClick(TObject *Sender);
  void __fastcall DelBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall DBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
public:
  __fastcall TDateForm(TComponent* Owner);
  __fastcall ~TDateForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TDateForm *DateForm;
//---------------------------------------------------------------------------
#endif


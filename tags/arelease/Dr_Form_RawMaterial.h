//---------------------------------------------------------------------------
#ifndef Dr_Form_RawMaterialH
#define Dr_Form_RawMaterialH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Dr_Form_Hint.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "DrButton.h"
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TRawMaterialForm : public THintForm
{
__published:
  TBevel *Bevel1;
  TBevel *Bevel2;
  TDrButton *SaveB;
  TDrButton *InsB;
  TDrButton *DelB;
  TDrButton *CancelB;
  TDrButton *OkB;
  TDrButton *CancelExitB;
  TDBGrid *DBGrid;
  TShape *DBGridS;
  TLabel *RawMaterialNameL;
  TDBEdit *RawMaterialName;
  TShape *RawMaterialNameS;
  TLabel *DBGridCostL;
  TDBGrid *DBGridCost;
  TShape *DBGridCostS;
  TDrButton *RawMaterialCostSaveB;
  TDrButton *RawMaterialCostInsB;
  TDrButton *RawMaterialCostDelB;
  TBevel *Bevel3;
  TLabel *DateValueL;
  TShape *DateValueS;
  TEdit *Edit1;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelExitBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall SaveBClick(TObject *Sender);
  void __fastcall InsBClick(TObject *Sender);
  void __fastcall DelBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall DBGridCostDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
public:
  __fastcall TRawMaterialForm(TComponent* Owner);
  __fastcall ~TRawMaterialForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TRawMaterialForm *RawMaterialForm;
//---------------------------------------------------------------------------
#endif


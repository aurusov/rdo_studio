//---------------------------------------------------------------------------
#ifndef Dr_Form_RawMaterialInProductH
#define Dr_Form_RawMaterialInProductH
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
class TRawMaterialInProductForm : public THintForm
{
__published:
  TDBGrid *DBGrid;
  TShape *DBGridS;
  TBevel *Bevel1;
  TBevel *Bevel2;
  TDrButton *SaveB;
  TDrButton *InsB;
  TDrButton *DelB;
  TDrButton *CancelB;
  TDrButton *OkB;
  TDrButton *CancelExitB;
  TLabel *RMWeightL;
  TDBEdit *RMWeight;
  TShape *RMWeightS;
  TLabel *RawMaterialIDL;
  TShape *RawMaterialIDS;
  TLabel *RMCommentInProductL;
  TDBEdit *RMCommentInProduct;
  TShape *RMCommentInProductS;
  TDrButton *RawMaterialIDB;
  TEdit *RawMaterialID;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelExitBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall SaveBClick(TObject *Sender);
  void __fastcall InsBClick(TObject *Sender);
  void __fastcall DelBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall DBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall RawMaterialIDBClick(TObject *Sender);
public:
  __fastcall TRawMaterialInProductForm(TComponent* Owner);
  __fastcall ~TRawMaterialInProductForm();

  void UpdateRMName();
};
//---------------------------------------------------------------------------
extern PACKAGE TRawMaterialInProductForm *RawMaterialInProductForm;
//---------------------------------------------------------------------------
#endif

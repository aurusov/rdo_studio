//---------------------------------------------------------------------------
#ifndef Dr_Form_Program_MachineH
#define Dr_Form_Program_MachineH
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
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TMachineInProgForm : public THintForm
{
__published:
  TDBGrid *MachInProgGrid;
  TDBGrid *MachineGrid;
  TDrButton *InsB;
  TDrButton *DelB;
  TDrButton *OkB;
  TDrButton *CancelB;
  TShape *TopLineS;
  TDrButton *SelectAllB;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall MachInProgGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall MachineGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall InsBClick(TObject *Sender);
  void __fastcall DelBClick(TObject *Sender);
  void __fastcall SelectAllBClick(TObject *Sender);
public:
  __fastcall TMachineInProgForm(TComponent* Owner);
  __fastcall ~TMachineInProgForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TMachineInProgForm *MachineInProgForm;
//---------------------------------------------------------------------------
#endif


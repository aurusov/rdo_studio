//---------------------------------------------------------------------------
#ifndef Dr_Form_PersonalH
#define Dr_Form_PersonalH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
#include "Dr_Form_Hint.h"
#include "DrButton.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TPersonalForm : public THintForm
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
  TLabel *LastNameL;
  TLabel *FirstNameL;
  TLabel *SecondNameL;
  TShape *LastNameS;
  TShape *FirstNameS;
  TShape *SecondNameS;
  TDBEdit *LastName;
  TDBEdit *FirstName;
  TDBEdit *SecondName;
  TLabel *OfficeL;
  TShape *OfficeS;
  TDBEdit *Office;
  TDBCheckBox *Vulcan5Class;
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
  __fastcall TPersonalForm(TComponent* Owner);
  __fastcall ~TPersonalForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TPersonalForm *PersonalForm;
//---------------------------------------------------------------------------
#endif


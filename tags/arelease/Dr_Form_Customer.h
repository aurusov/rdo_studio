//---------------------------------------------------------------------------
#ifndef Dr_Form_CustomerH
#define Dr_Form_CustomerH
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
class TCustomerForm : public THintForm
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
  TLabel *CustomerNameL;
  TDBEdit *CustomerName;
  TShape *CustomerNameS;
  TDBEdit *Adress;
  TShape *AdressS;
  TLabel *AdressL;
  TLabel *PhoneL;
  TDBEdit *Phone;
  TShape *PhoneS;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelExitBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall SaveBClick(TObject *Sender);
  void __fastcall InsBClick(TObject *Sender);
  void __fastcall DelBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
public:
  __fastcall TCustomerForm(TComponent* Owner);
  __fastcall ~TCustomerForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TCustomerForm *CustomerForm;
//---------------------------------------------------------------------------
#endif


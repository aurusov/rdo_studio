//---------------------------------------------------------------------------
#ifndef Dr_Form_Select_DateH
#define Dr_Form_Select_DateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Dr_Form_Select.h"
#include "DrButton.h"
#include <Buttons.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TSelectDateForm : public TSelectForm
{
__published:
  TDrButton *DateEditB;
  void __fastcall DBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall DateEditBClick(TObject *Sender);
private:
  bool firstProgram;
  
public:
  __fastcall TSelectDateForm(TComponent* Owner, const bool firstProgram);
  __fastcall ~TSelectDateForm();

  void UpdateOkB();

  int GetDateID();
};
//---------------------------------------------------------------------------
extern PACKAGE TSelectDateForm *SelectDateForm;
//---------------------------------------------------------------------------
#endif


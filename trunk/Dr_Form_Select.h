//---------------------------------------------------------------------------
#ifndef Dr_Form_SelectH
#define Dr_Form_SelectH
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
class TSelectForm : public THintForm
{
__published:
  TDBGrid *DBGrid;
  TShape *TopLineS;
  TDrButton *OkB;
  TDrButton *CancelB;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall DBGridDblClick(TObject *Sender);
public:
  __fastcall TSelectForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
                                               

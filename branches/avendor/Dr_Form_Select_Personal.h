//---------------------------------------------------------------------------
#ifndef Dr_Form_Select_PersonalH
#define Dr_Form_Select_PersonalH
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
class TSelectPersonalForm : public TSelectForm
{
__published:
  void __fastcall DBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
public:
  __fastcall TSelectPersonalForm(TComponent* Owner);
  AnsiString GetPersonalName();
};
//---------------------------------------------------------------------------
#endif


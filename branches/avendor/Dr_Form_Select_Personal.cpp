//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Select_Personal.h"
#include "DataModule.h"

#include "Misc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Select"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TSelectPersonalForm::TSelectPersonalForm(TComponent* Owner): TSelectForm(Owner)
{
}

void __fastcall TSelectPersonalForm::DBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  if (Column->ID == 0) {
    DBGrid->Canvas->FillRect(Rect);
    DBGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, GetPersonalName());
  } else {
    DBGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
  }
}

AnsiString TSelectPersonalForm::GetPersonalName()
{
  return Data->GetPersonalName();
}


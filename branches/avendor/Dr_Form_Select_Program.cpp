//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Select_Program.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Select"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TSelectProgramForm::TSelectProgramForm(TComponent* Owner): TSelectForm(Owner)
{
}

void __fastcall TSelectProgramForm::DBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  DBGrid->Canvas->FillRect(Rect);
  DBGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, Data->GetDateValue(Column->Field->AsInteger));
}


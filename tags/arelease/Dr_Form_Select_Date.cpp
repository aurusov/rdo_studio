//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Select_Date.h"
#include "Dr_Form_Date.h"
#include "DataModule.h"

#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Select"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TSelectDateForm *SelectDateForm = NULL;
//---------------------------------------------------------------------------
__fastcall TSelectDateForm::TSelectDateForm(TComponent* Owner, const bool firstProgram):
  TSelectForm(Owner),
  firstProgram(firstProgram)
{
  SelectDateForm = this;
  if (firstProgram) {
    DBGrid->DataSource = Data->DateTableDS;
  } else {
    DBGrid->DataSource = Data->ShowFreeDateDS;
    Data->Update_ShowFreeDate();
  }
  UpdateOkB();
}

__fastcall TSelectDateForm::~TSelectDateForm()
{
  Data->ShowFreeDate->Close();
  SelectDateForm = NULL;
}

void __fastcall TSelectDateForm::DBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  if (Column->ID == 0) {
    TDateTime date = Column->Field->AsDateTime;
    AnsiString str;
    if ((int)date) {
      str = date.FormatString(LoadStr(sDateMask));
    } else {
      str = "";
    }
    DBGrid->Canvas->FillRect(Rect);
    DBGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
  } else {
    DBGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
  }
}

void __fastcall TSelectDateForm::DateEditBClick(TObject *Sender)
{
  new TDateForm(NULL);
  try {
    DateForm->ShowModal();
    delete DateForm;
  } catch (Exception& e) {
    delete DateForm;
    throw;
  }
}

void TSelectDateForm::UpdateOkB()
{
  bool flag;
  if (firstProgram) {
    flag = Data->DateTable->IsEmpty();
  } else {
    flag = Data->ShowFreeDate->IsEmpty();
  }
  OkB->Enabled = !flag;
}

int TSelectDateForm::GetDateID()
{
  if (firstProgram) {
    return Data->DateTable->FieldByName("DateID")->AsInteger;
  } else {
    return Data->ShowFreeDate->FieldByName("DateID")->AsInteger;
  }
}


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Personal.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TPersonalForm *PersonalForm = NULL;
//---------------------------------------------------------------------------
__fastcall TPersonalForm::TPersonalForm(TComponent* Owner): THintForm(Owner)
{
  PersonalForm = this;
  Data->PersonalDSStateChange(NULL);
}

__fastcall TPersonalForm::~TPersonalForm()
{
  PersonalForm = NULL;
}

void __fastcall TPersonalForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TPersonalForm::CancelExitBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TPersonalForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->PersonalDS))
                       Data->Personal->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->Personal->Cancel(); break;
  }
}

void __fastcall TPersonalForm::SaveBClick(TObject *Sender)
{
  Data->Personal->Post();
}

void __fastcall TPersonalForm::InsBClick(TObject *Sender)
{
  Data->Personal->Append();
  LastName->SetFocus();
}

void __fastcall TPersonalForm::DelBClick(TObject *Sender)
{
  if (ShowQueryMessage(sRecordDelete, MB_YESNO) == IDYES)
    Data->Personal->Delete();
}

void __fastcall TPersonalForm::CancelBClick(TObject *Sender)
{
  Data->Personal->Cancel();
}

void __fastcall TPersonalForm::DBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  if (Column->ID == 0) {
    AnsiString str = str = Data->GetPersonalName();
    DBGrid->Canvas->FillRect(Rect);
    DBGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
  } else {
    DBGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
  }
}


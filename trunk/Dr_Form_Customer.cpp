//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Customer.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TCustomerForm *CustomerForm = NULL;
//---------------------------------------------------------------------------
__fastcall TCustomerForm::TCustomerForm(TComponent* Owner): THintForm(Owner)
{
  CustomerForm = this;
  Data->CustomersDSStateChange(NULL);
}

__fastcall TCustomerForm::~TCustomerForm()
{
  CustomerForm = NULL;
}

void __fastcall TCustomerForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TCustomerForm::CancelExitBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TCustomerForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->CustomersDS))
                       Data->Customers->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->Customers->Cancel(); break;
  }
}

void __fastcall TCustomerForm::SaveBClick(TObject *Sender)
{
  Data->Customers->Post();
}

void __fastcall TCustomerForm::InsBClick(TObject *Sender)
{
  Data->Customers->Append();
  CustomerName->SetFocus();
}

void __fastcall TCustomerForm::DelBClick(TObject *Sender)
{
  if (ShowQueryMessage(sRecordDelete, MB_YESNO) == IDYES)
    Data->Customers->Delete();
}

void __fastcall TCustomerForm::CancelBClick(TObject *Sender)
{
  Data->Customers->Cancel();
}


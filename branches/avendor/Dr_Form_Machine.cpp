//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Machine.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TMachineForm *MachineForm = NULL;
//---------------------------------------------------------------------------
__fastcall TMachineForm::TMachineForm(TComponent* Owner): THintForm(Owner)
{
  MachineForm = this;
  Data->MachinesDSStateChange(NULL);
}

__fastcall TMachineForm::~TMachineForm()
{
  MachineForm = NULL;
}

void __fastcall TMachineForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TMachineForm::CancelExitBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TMachineForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->MachinesDS))
                       Data->Machines->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->Machines->Cancel(); break;
  }
}

void __fastcall TMachineForm::SaveBClick(TObject *Sender)
{
  Data->Machines->Post();
}

void __fastcall TMachineForm::InsBClick(TObject *Sender)
{
  Data->Machines->Append();
  MachineName->SetFocus();
}

void __fastcall TMachineForm::DelBClick(TObject *Sender)
{
  if (ShowQueryMessage(sRecordDelete, MB_YESNO) == IDYES)
    Data->Machines->Delete();
}

void __fastcall TMachineForm::CancelBClick(TObject *Sender)
{
  Data->Machines->Cancel();
}


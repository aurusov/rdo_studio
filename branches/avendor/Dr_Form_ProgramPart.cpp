//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_ProgramPart.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TProgramPartForm::TProgramPartForm(TComponent* Owner): THintForm(Owner)
{
}

void __fastcall TProgramPartForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TProgramPartForm::CancelBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TProgramPartForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->ProgramPartDS))
                       Data->ProgramPart->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->ProgramPart->Cancel(); break;
  }
}


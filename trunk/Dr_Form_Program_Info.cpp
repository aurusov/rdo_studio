//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Program_Info.h"
#include "Dr_Form_Select_Date.h"
#include "Dr_Form_Select_Personal.h"
#include "DataModule.h"

#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TProgramInfoForm *ProgramInfoForm = NULL;
//---------------------------------------------------------------------------
__fastcall TProgramInfoForm::TProgramInfoForm(TComponent* Owner): THintForm(Owner)
{
  ProgramInfoForm = this;
}

__fastcall TProgramInfoForm::~TProgramInfoForm()
{
  ProgramInfoForm = NULL;
}

void __fastcall TProgramInfoForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TProgramInfoForm::CancelBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TProgramInfoForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->ProgramsDS))
                       Data->Programs->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->Programs->Cancel(); break;
  }
}

void __fastcall TProgramInfoForm::FormShow(TObject *Sender)
{
  UpdateDateValue();
}

void TProgramInfoForm::UpdateDateValue()
{
  ProgDate->Caption = Data->GetDateValue();
  ProgDateB->Left = ProgDate->Left + ProgDate->Width + 7;
}

void __fastcall TProgramInfoForm::ProgDateBClick(TObject *Sender)
{
  TSelectDateForm* form = new TSelectDateForm(NULL, firstProgram);
  form->bDesktopCenter = false;
  try {
    TPoint p;
    p.x = 0;
    p.y = ProgDateB->Height + 4;
    p = ProgDateB->ClientToScreen(p);
    form->Left = p.x;
    form->Top  = p.y;
    if (form->ShowModal() == mrOk) {
      Data->Programs->Edit();
      Data->Programs->FieldByName("DateID")->AsInteger = form->GetDateID();
      UpdateDateValue();
    }
    delete form;
  } catch (Exception& e) {
    delete form;
    throw;
  }
}

void __fastcall TProgramInfoForm::CompanyDirectorBClick(TObject *Sender)
{
  TSelectPersonalForm* form = new TSelectPersonalForm(NULL);
  form->bDesktopCenter = false;
  try {
    TSpeedButton* button = (TSpeedButton*)Sender;
    TPoint p;
    p.x = 0;
    p.y = button->Height + 4;
    p = button->ClientToScreen(p);
    form->Left = p.x;
    form->Top  = p.y;
    if (form->ShowModal() == mrOk) {
      AnsiString name = button->Name;
      name.Delete(name.Length(), 1);
      Data->Programs->Edit();
      TDBEdit* edit = (TDBEdit*)FindComponent(name);
      edit->Field->AsString = form->GetPersonalName();
      edit->SetFocus();
      edit->SelectAll();
    }
    delete form;
  } catch (Exception& e) {
    delete form;
    throw;
  }
}


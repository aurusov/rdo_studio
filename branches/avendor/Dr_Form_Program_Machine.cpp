//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Program_Machine.h"
#include "Form_Rubber.h"
#include "DataModule.h"

#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TMachineInProgForm *MachineInProgForm = NULL;
//---------------------------------------------------------------------------
__fastcall TMachineInProgForm::TMachineInProgForm(TComponent* Owner): THintForm(Owner)
{
  MachineInProgForm = this;
  Data->MachinesAfterScroll(NULL);
}

__fastcall TMachineInProgForm::~TMachineInProgForm()
{
  MachineInProgForm = NULL;
}

void __fastcall TMachineInProgForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TMachineInProgForm::CancelBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TMachineInProgForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->MachInProgDS))
                       Data->MachInProg->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->MachInProg->Cancel(); break;
  }
}

void __fastcall TMachineInProgForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if (Key == VK_ESCAPE) Close();
  if (Key == VK_RETURN && dynamic_cast<TCustomGrid*>(ActiveControl)) OkBClick(NULL);
}

void __fastcall TMachineInProgForm::MachInProgGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  AnsiString str = LoadStr(sUnknowValue);
  int machine_id = Data->MachInProg->FieldByName("MachineID")->AsInteger;
  TLocateOptions locate;
  locate.Clear();
  Data->LockMachine = true;
  Data->Machines->DisableControls();

  try {
    void* bookmark = Data->Machines->GetBookmark();
    bool flag = Data->Machines->FieldByName("MachineID")->AsInteger == machine_id;
    if (!flag) flag = Data->Machines->Locate("MachineID", machine_id, locate);
    if (flag) {
      switch (Column->ID) {
        case 0: str = Data->Machines->FieldByName("MachineName")->AsString; break;
        case 1: {
          AnsiString s;
          s = Data->Machines->FieldByName("Effort")->AsString;
          if (s.Length()) str = s;
          s = Data->Machines->FieldByName("Floors")->AsString;
          if (s.Length()) str += ", " + s;
          s = Data->Machines->FieldByName("FloorHeight")->AsString;
          if (s.Length()) str += ", " + s;
          s = Data->Machines->FieldByName("LoadSides")->AsString;
          if (s.Length()) str += ", " + s;
          s = Data->Machines->FieldByName("PlateLength")->AsString;
          if (s.Length()) {
            AnsiString s2 = Data->Machines->FieldByName("PlateWidth")->AsString;
            if (s2.Length()) str += ", " + s + "x" + s2;
          }
          break;
        }
      }
    }
    Data->Machines->GotoBookmark(bookmark);
    Data->Machines->EnableControls();
    Data->LockMachine = false;

  } catch (Exception& e) {
    Data->Machines->EnableControls();
    Data->LockMachine = false;
    throw;
  }

  MachInProgGrid->Canvas->FillRect(Rect);
  MachInProgGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
}

void __fastcall TMachineInProgForm::MachineGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  switch (Column->ID) {
    case 1: {
      AnsiString str = "";
      AnsiString s;
      s = Data->Machines->FieldByName("Effort")->AsString;
      if (s.Length()) str = s;
      s = Data->Machines->FieldByName("Floors")->AsString;
      if (s.Length()) str += ", " + s;
      s = Data->Machines->FieldByName("FloorHeight")->AsString;
      if (s.Length()) str += ", " + s;
      s = Data->Machines->FieldByName("LoadSides")->AsString;
      if (s.Length()) str += ", " + s;
      s = Data->Machines->FieldByName("PlateLength")->AsString;
      if (s.Length()) {
        AnsiString s2 = Data->Machines->FieldByName("PlateWidth")->AsString;
        if (s2.Length()) str += ", " + s + "x" + s2;
      }
      MachineGrid->Canvas->FillRect(Rect);
      MachineGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
      break;
    }
    default: MachineGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
  }
}

void __fastcall TMachineInProgForm::InsBClick(TObject *Sender)
{
  Data->MachInProg->Last();
  Data->MachInProg->Append();
  Data->MachInProg->FieldByName("ProgramID")->AsInteger = RubberStudio->activeProgram_ID;
  Data->MachInProg->FieldByName("MachineID")->AsInteger = Data->Machines->FieldByName("MachineID")->AsInteger;
  Data->MachInProg->Post();
}

void __fastcall TMachineInProgForm::DelBClick(TObject *Sender)
{
  Data->MachInProg->Delete();
}

void __fastcall TMachineInProgForm::SelectAllBClick(TObject *Sender)
{
  TLocateOptions locate;
  locate.Clear();
  Data->LockMachine = true;
  Data->Machines->DisableControls();
  try {
    void* bookmark = Data->Machines->GetBookmark();

    // Очистить список используемого оборудования
    Data->MachInProg->First();
    while ( !Data->MachInProg->Eof ) {
      if ( Data->MachInProg->FieldByName("ProgramID")->AsInteger == RubberStudio->activeProgram_ID ) {
        Data->MachInProg->Delete();
      } else {
        Data->MachInProg->Next();
      }
    }

    // Добавить все известное оборудование в список используемого оборудования
    Data->Machines->First();
    while ( !Data->Machines->Eof ) {
      Data->MachInProg->Append();
      Data->MachInProg->FieldByName("ProgramID")->AsInteger = RubberStudio->activeProgram_ID;
      Data->MachInProg->FieldByName("MachineID")->AsInteger = Data->Machines->FieldByName("MachineID")->AsInteger;
      Data->MachInProg->Post();
      Data->Machines->Next();
    }

    Data->Machines->GotoBookmark(bookmark);
    Data->Machines->EnableControls();
    Data->LockMachine = false;
    Data->MachinesAfterScroll(NULL);

  } catch (Exception& e) {
    Data->Machines->EnableControls();
    Data->LockMachine = false;
    throw;
  }
}


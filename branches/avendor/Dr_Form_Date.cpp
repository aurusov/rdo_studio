//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Date.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TDateForm *DateForm = NULL;
//---------------------------------------------------------------------------
__fastcall TDateForm::TDateForm(TComponent* Owner): THintForm(Owner)
{
  DateForm = this;
  Data->DateTableDSStateChange(NULL);
  Data->DateTableAfterScroll(NULL);
}

__fastcall TDateForm::~TDateForm()
{
  DateForm = NULL;
}

void __fastcall TDateForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TDateForm::CancelExitBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TDateForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->DateTableDS))
                       Data->DateTable->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->DateTable->Cancel(); break;
  }
}

void __fastcall TDateForm::SaveBClick(TObject *Sender)
{
  Data->DateTable->Post();
}

void __fastcall TDateForm::InsBClick(TObject *Sender)
{
  Data->DateTable->Append();
  DateValue->SetFocus();
}

void __fastcall TDateForm::DelBClick(TObject *Sender)
{
  if (ShowQueryMessage(sRecordDelete, MB_YESNO) == IDYES)
    Data->DateTable->Delete();
}

void __fastcall TDateForm::CancelBClick(TObject *Sender)
{
  Data->DateTable->Cancel();
}

void __fastcall TDateForm::DBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  switch (Column->ID) {
    // Дата
    case 0 : {
      TDateTime date = Column->Field->AsDateTime;
      AnsiString str;
      if ((int)date) {
        str = date.FormatString(LoadStr(sDateMask));
      } else {
        str = "";
      }
      DBGrid->Canvas->FillRect(Rect);
      DBGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
      break;
    }
    // Производственная программа
    case 1 : {
      AnsiString str;
      if (!Data->DateTable->IsEmpty()) {
        int res_id = Data->GetProgramCountByDate(Data->DateTable->FieldByName("DateID")->AsInteger) ? sHaveProgramByDate : sNoProgramByDate;
        str = LoadStr(res_id);
      } else {
        str = "";
      }
      DBGrid->Canvas->FillRect(Rect);
      DBGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
      break;
    }
    // Остальные поля таблицы
    default: DBGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
  }
}


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_RawMaterial.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TRawMaterialForm *RawMaterialForm = NULL;
//---------------------------------------------------------------------------
__fastcall TRawMaterialForm::TRawMaterialForm(TComponent* Owner): THintForm(Owner)
{
  RawMaterialForm = this;
  Data->RawMaterialDSStateChange(NULL);
}

__fastcall TRawMaterialForm::~TRawMaterialForm()
{
  RawMaterialForm = NULL;
}

void __fastcall TRawMaterialForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TRawMaterialForm::CancelExitBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TRawMaterialForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->RawMaterialDS))
                       Data->RawMaterial->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->RawMaterial->Cancel(); break;
  }
}

void __fastcall TRawMaterialForm::SaveBClick(TObject *Sender)
{
  Data->RawMaterial->Post();
}

void __fastcall TRawMaterialForm::InsBClick(TObject *Sender)
{
  Data->RawMaterial->Append();
  RawMaterialName->SetFocus();
}

void __fastcall TRawMaterialForm::DelBClick(TObject *Sender)
{
  if (ShowQueryMessage(sRecordDelete, MB_YESNO) == IDYES)
    Data->RawMaterial->Delete();
}

void __fastcall TRawMaterialForm::CancelBClick(TObject *Sender)
{
  Data->RawMaterial->Cancel();
}

void __fastcall TRawMaterialForm::DBGridCostDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  switch (Column->ID) {
    // Дата
    case 0 : {
      int date_id = Column->Field->AsInteger;
      AnsiString str = "";
      TLocateOptions locate;
      locate.Clear();
      if (Data->DateTable->Locate("DateID", date_id, locate)) {
        TDateTime date = Data->DateTable->FieldByName("DateValue")->AsDateTime;
        if ((int)date)
          str = date.FormatString(LoadStr(sDateMask));
      }
      DBGridCost->Canvas->FillRect(Rect);
      DBGridCost->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
      break;
    }
    // Остальные поля таблицы
    default: DBGridCost->DefaultDrawColumnCell(Rect, DataCol, Column, State);
  }
}


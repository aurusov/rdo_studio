//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_RawMaterialInProduct.h"
#include "Dr_Form_Select_RawMaterial.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TRawMaterialInProductForm *RawMaterialInProductForm = NULL;
//---------------------------------------------------------------------------
__fastcall TRawMaterialInProductForm::TRawMaterialInProductForm(TComponent* Owner): THintForm(Owner)
{
  RawMaterialInProductForm = this;
  AnsiString str = Data->Products->FieldByName("ProductName")->AsString;
  if (!str.Length()) str = LoadStr(sUnknowValue);
  Caption = MiscFormat("%s %s", Caption.c_str(), str.c_str());
  Data->RawMaterialInProductDSStateChange(NULL);
  UpdateRMName();
}

__fastcall TRawMaterialInProductForm::~TRawMaterialInProductForm()
{
  RawMaterialInProductForm = NULL;
}

void __fastcall TRawMaterialInProductForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TRawMaterialInProductForm::CancelExitBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TRawMaterialInProductForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->RawMaterialInProductDS))
                       Data->RawMaterialInProduct->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->RawMaterialInProduct->Cancel(); break;
  }
}

void __fastcall TRawMaterialInProductForm::SaveBClick(TObject *Sender)
{
  Data->RawMaterialInProduct->Post();
}

void __fastcall TRawMaterialInProductForm::InsBClick(TObject *Sender)
{
  Data->RawMaterialInProduct->Append();
  RawMaterialID->SetFocus();
}

void __fastcall TRawMaterialInProductForm::DelBClick(TObject *Sender)
{
  if (ShowQueryMessage(sRecordDelete, MB_YESNO) == IDYES)
    Data->RawMaterialInProduct->Delete();
}

void __fastcall TRawMaterialInProductForm::CancelBClick(TObject *Sender)
{
  Data->RawMaterialInProduct->Cancel();
}

void __fastcall TRawMaterialInProductForm::DBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  switch (Column->ID) {
    // Наименование
    case 0 : {
      AnsiString str;
      int rm_id = Column->Field->AsInteger;
      TLocateOptions locate;
      locate.Clear();
      if (Data->RawMaterial->Locate("RawMaterialID", rm_id, locate)) {
        str = Data->RawMaterial->FieldByName("RawMaterialName")->AsString;
      } else {
        str = "";
      }
      DBGrid->Canvas->FillRect(Rect);
      DBGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
      break;
    }
    // Вес
    case 1 : {
      if (Column->Field->AsFloat == 0.0)
        DBGrid->Canvas->Font->Color = DBGrid->Canvas->Brush->Color;
    }
    // Остальные поля таблицы (Комментарий)
    default: DBGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
  }
}

void __fastcall TRawMaterialInProductForm::RawMaterialIDBClick(TObject *Sender)
{
  TSelectRawMaterialForm* form = new TSelectRawMaterialForm(NULL);
  form->bDesktopCenter = false;
  try {
    TPoint p;
    p.x = 0;
    p.y = RawMaterialIDB->Height + 4;
    p = RawMaterialIDB->ClientToScreen(p);
    form->Left = p.x;
    form->Top  = p.y;
    if (form->ShowModal() == mrOk) {
      Data->RawMaterialInProduct->Edit();
      Data->RawMaterialInProduct->FieldByName("RawMaterialID")->AsInteger = Data->RawMaterial->FieldByName("RawMaterialID")->AsInteger;
      UpdateRMName();
      RawMaterialID->SetFocus();
    }
    delete form;
  } catch (Exception& e) {
    delete form;
    throw;
  }
}

void TRawMaterialInProductForm::UpdateRMName()
{
  AnsiString str;
  if (Data->RawMaterialInProduct->IsEmpty()) {
    str = "";
  } else {
    int rm_id = Data->RawMaterialInProduct->FieldByName("RawMaterialID")->AsInteger;
    TLocateOptions locate;
    locate.Clear();
    if (Data->RawMaterial->Locate("RawMaterialID", rm_id, locate)) {
      str = Data->RawMaterial->FieldByName("RawMaterialName")->AsString;
    } else {
      str = LoadStr(sUnknowValue);
    }
  }
  RawMaterialID->Text = str;
}


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Product.h"
#include "Dr_Form_RawMaterialInProduct.h"
#include "Form_Rubber.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TProductForm *ProductForm = NULL;
//---------------------------------------------------------------------------
__fastcall TProductForm::TProductForm(TComponent* Owner):
  THintForm(Owner),
  add_key(true)
{
  Data->LocateProduct();
  ProductForm = this;
  PFCylindricClick(NULL);
  Data->ProductsDSStateChange(NULL);
  find_str = "";
  Timer->Enabled = true;
}

__fastcall TProductForm::~TProductForm()
{
  ProductForm = NULL;
  RubberStudio->PrDBGrid->Repaint();
}

void __fastcall TProductForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TProductForm::CancelExitBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TProductForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch (ModalResult) {
    case mrOk    : try {
                     if (Data->CanPost(Data->ProductsDS))
                       Data->Products->Post();
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
    case mrCancel: Data->Products->Cancel(); break;
  }
}

void __fastcall TProductForm::SaveBClick(TObject *Sender)
{
  Data->Products->Post();
}

void __fastcall TProductForm::InsBClick(TObject *Sender)
{
  Data->Products->Append();
  ProductName->SetFocus();
}

void __fastcall TProductForm::DelBClick(TObject *Sender)
{
  if (ShowQueryMessage(sRecordDelete, MB_YESNO) == IDYES)
    Data->Products->Delete();
}

void __fastcall TProductForm::CancelBClick(TObject *Sender)
{
  Data->Products->Cancel();
}

void __fastcall TProductForm::DBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  TCanvas* canvas = DBGrid->Canvas;
  AnsiString str = LoadStr(sUnknowValue);
  switch (Column->ID) {
    // Прессформа
    case 2: {
      str = MiscFormat("%d/%d  ", Data->Products->FieldByName("PFNumber")->AsInteger, Data->Products->FieldByName("PFPlaces")->AsInteger);
      if (Data->Products->FieldByName("PFCylindric")->AsBoolean) {
        TRect rect = Rect;
        canvas->FillRect(rect);
        rect.left += 2;
        rect.top  += 2;
        canvas->TextOut(rect.left, rect.top, str);
        rect.left += canvas->TextWidth(str);
        str = '\306';
        AnsiString fontname = canvas->Font->Name;
        canvas->Font->Name = "Symbol";
        canvas->TextOut(rect.left, rect.top, str);
        rect.left += canvas->TextWidth(str) + 1;
        canvas->Font->Name = fontname;
        str = MiscFormat("%dx%d", Data->Products->FieldByName("PFDiameter")->AsInteger, Data->Products->FieldByName("PFHeight")->AsInteger);
        canvas->TextOut(rect.left, rect.top, str);
        return;
      } else {
        str = MiscFormat("%s%dx%dx%d", str.c_str(), Data->Products->FieldByName("PFLength")->AsInteger, Data->Products->FieldByName("PFWidth")->AsInteger, Data->Products->FieldByName("PFHeight")->AsInteger);
      }
      break;
    }
    // 5-й разряд
    case 12: {
      int res_id = Column->Field->AsBoolean ? sYesValueUpperCase : sNoValueLowerCase;
      str = LoadStr(res_id);
      break;
    }
    // Остальные поля таблицы
    default: DBGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State); return;
  }
  canvas->FillRect(Rect);
  canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
}

void __fastcall TProductForm::PFCylindricClick(TObject *Sender)
{
  bool flag = PFCylindric->Checked;
  PFLength->Visible    = !flag;
  PFLengthS->Visible   = !flag;
  PFSizeX1L->Visible   = !flag;
  PFWidth->Visible     = !flag;
  PFWidthS->Visible    = !flag;
  PFSizeDL->Visible    = flag;
  PFDiameter->Visible  = flag;
  PFDiameterS->Visible = flag;
}

void __fastcall TProductForm::RMDBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  switch (Column->ID) {
    // Наименование
    case 0 : {
      int rm_id = Column->Field->AsInteger;
      TLocateOptions locate;
      locate.Clear();
      AnsiString str;
      if (Data->RawMaterial->Locate("RawMaterialID", rm_id, locate)) {
        str = Data->RawMaterial->FieldByName("RawMaterialName")->AsString;
      } else {
        str = "";
      }
      RMDBGrid->Canvas->FillRect(Rect);
      RMDBGrid->Canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
      break;
    }
    // Вес
    case 1 : {
      if (Column->Field->AsFloat == 0.0)
        RMDBGrid->Canvas->Font->Color = RMDBGrid->Canvas->Brush->Color;
    }
    // Остальные поля таблицы (Комментарий)
    default: RMDBGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State);
  }
}

void __fastcall TProductForm::RMBClick(TObject *Sender)
{
  new TRawMaterialInProductForm(NULL);
  try {
    RawMaterialInProductForm->ShowModal();
    delete RawMaterialInProductForm;
  } catch (Exception& e) {
    delete RawMaterialInProductForm;
    throw;
  }
}

void __fastcall TProductForm::FormKeyPress(TObject *Sender, char &Key)
{
  if ( DBGrid->Focused() ) {
    if ( !add_key || Key == 8 ) {
      find_str = "";
      add_key = true;
    }
    if ( (unsigned char)Key >= 32 ) {
      Timer->Enabled = false;
      find_str += Key;

      TLocateOptions locate;
      locate.Clear();
      locate << loCaseInsensitive << loPartialKey;
      if ( DBGrid->SelectedIndex == 1 ) {
        Data->Products->Locate("TechMap", find_str, locate);
      } else {
        Data->Products->Locate("ProductName", find_str, locate);
      }

      Timer->Enabled = true;
    }

    if ( Key == 8 || (unsigned char)Key >= 32 )
      Caption = MiscFormat( LoadStr(sFindProductName).c_str(), find_str.c_str() );
  }
}

void __fastcall TProductForm::TimerTimer(TObject *Sender)
{
  add_key = false;
}


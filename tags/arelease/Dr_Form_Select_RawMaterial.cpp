//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Select_RawMaterial.h"
#include "Dr_Form_RawMaterial.h"
#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Select"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TSelectRawMaterialForm::TSelectRawMaterialForm(TComponent* Owner): TSelectForm(Owner)
{
}

void __fastcall TSelectRawMaterialForm::RawMaterialEditBClick(TObject *Sender)
{
  new TRawMaterialForm(NULL);
  try {
    RawMaterialForm->ShowModal();
    delete RawMaterialForm;
  } catch (Exception& e) {
    delete RawMaterialForm;
    throw;
  }
}


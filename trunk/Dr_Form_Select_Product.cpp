//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Select_Product.h"
#include "Form_Rubber.h"
#include "DataModule.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Select"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TSelectProductForm *SelectProductForm = NULL;
//---------------------------------------------------------------------------
__fastcall TSelectProductForm::TSelectProductForm(TComponent* Owner):
  TSelectForm(Owner),
  add_key(true)
{
  Data->LocateProduct();
  SelectProductForm = this;
  find_str = "";
  Timer->Enabled = true;
}

__fastcall TSelectProductForm::~TSelectProductForm()
{
  SelectProductForm = NULL;
  RubberStudio->PrDBGrid->Repaint();
}

void __fastcall TSelectProductForm::FormKeyPress(TObject *Sender, char &Key)
{
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

void __fastcall TSelectProductForm::TimerTimer(TObject *Sender)
{
  add_key = false;
}


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Select.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TSelectForm::TSelectForm(TComponent* Owner): THintForm(Owner)
{
}

void __fastcall TSelectForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TSelectForm::CancelBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TSelectForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if (Key == VK_ESCAPE) Close();
  if (Key == VK_RETURN && dynamic_cast<TCustomGrid*>(ActiveControl)) OkBClick(NULL);
}

void __fastcall TSelectForm::DBGridDblClick(TObject *Sender)
{
  OkBClick(NULL);
}


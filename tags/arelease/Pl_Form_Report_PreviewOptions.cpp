//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Pl_Form_Report_PreviewOptions.h"
#include "Report_Consts.h"
#include "Misc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TPreviewOptionsForm::TPreviewOptionsForm(TComponent* Owner)
        : THintForm(Owner),
        Report(NULL),
        Preview(NULL),
        CanUpdate(false)
{
}

__fastcall TPreviewOptionsForm::TPreviewOptionsForm(TPlBaseReport* Rpt, TPreviewForm* Prv)
        : THintForm((void*)NULL),
        Report(Rpt),
        Preview(Prv),
        CanUpdate(false)
{
  PagesChange(NULL);
  GetOptions();
}

__fastcall TPreviewOptionsForm::~TPreviewOptionsForm()
{
}

void TPreviewOptionsForm::GetOptions()
{
  ShadowWidthEd->Text       = Report->ShadowWidth;
  RepBordWidthEd->Text      = Report->ReportBorderWidth;
  RepCantWidthEd->Text      = Report->PreviewCantWidth;
  ShadExample->Brush->Color = Report->ShadowColor;
  CantExample->Brush->Color = Report->PreviewCantColor;
  RoundFonts->Checked       = Report->RoundFonts;
  BlackWhiteBox->Checked    = Report->BlackWhite;
  OffsetLeftEd->Text        = Report->OffsetLeft/10;
  OffsetTopEd->Text         = Report->OffsetTop/10;
  OffsetRightEd->Text       = Report->OffsetRight/10;
  OffsetBotEd->Text         = Report->OffsetBottom/10;
  PrintHeaderBox->Checked   = Report->PrintHeader;
  PrintDateBox->Checked     = Report->PrintPrintDate;
  PageNumbersBox->Checked   = Report->PrintPageNumbers;
  PrintFooterBox->Checked   = Report->PrintFooter;
  CanUpdate = true;
  SetHeaderFooterEnabled();
}

void TPreviewOptionsForm::SetHeaderFooterEnabled()
{
  if (PrintFooterBox->Checked && !PageNumbersBox->Checked && !PrintDateBox->Checked)
    PrintFooterBox->Checked = false;
  PrintDateBox->Enabled   = PrintFooterBox->Checked;
  PageNumbersBox->Enabled = PrintFooterBox->Checked;
}

void TPreviewOptionsForm::SetOptions()
{
  Report->ShadowWidth       = ShadowWidthEd->Text.ToInt();
  Report->ReportBorderWidth = RepBordWidthEd->Text.ToInt();
  Report->PreviewCantWidth  = RepCantWidthEd->Text.ToInt();
  Report->ShadowColor       = ShadExample->Brush->Color;
  Report->PreviewCantColor  = CantExample->Brush->Color;
  Report->RoundFonts        = RoundFonts->Checked;
  Report->BlackWhite        = BlackWhiteBox->Checked;
  Report->OffsetLeft        = RoundDouble(OffsetLeftEd->Text.ToDouble()*10);
  Report->OffsetTop         = RoundDouble(OffsetTopEd->Text.ToDouble()*10);
  Report->OffsetRight       = RoundDouble(OffsetRightEd->Text.ToDouble()*10);
  Report->OffsetBottom      = RoundDouble(OffsetBotEd->Text.ToDouble()*10);
  Report->PrintHeader       = PrintHeaderBox->Checked;
  Report->PrintPrintDate    = PrintDateBox->Checked;
  Report->PrintPageNumbers  = PageNumbersBox->Checked;
  Report->PrintFooter       = PrintFooterBox->Checked;
  if (CheckOffsets()) {
    Report->ResetPreviewParams();
    Report->Init();
    Preview->ResizeReport();
    Preview->doSetStatus();
    Preview->SetPageButtons();
  }
}

bool TPreviewOptionsForm::CheckOffsets()
{
  bool result = true;
  if (Report->OffsetLeft < Report->PrinterOffsetLeft || Report->OffsetTop < Report->PrinterOffsetTop || Report->OffsetRight < Report->PrinterOffsetRight || Report->OffsetBottom < Report->PrinterOffsetBottom) {
    int res = MessageBox(Handle, LoadStr(sOffsetsError).c_str(), Application->Title.c_str(), MB_YESNOCANCEL | MB_ICONWARNING);
    if (res == IDYES) {
      result = false;
      if (Report->OffsetLeft < Report->PrinterOffsetLeft)
        OffsetLeftEd->Text = Report->PrinterOffsetLeft/10;
      if (Report->OffsetTop < Report->PrinterOffsetTop)
        OffsetTopEd->Text = Report->PrinterOffsetTop/10;
      if (Report->OffsetRight < Report->PrinterOffsetRight)
        OffsetRightEd->Text = Report->PrinterOffsetRight/10;
      if (Report->OffsetBottom < Report->PrinterOffsetBottom)
        OffsetBotEd->Text = Report->PrinterOffsetBottom/10;
      ModalResult = mrNone;
    } else
      if (res == IDCANCEL) {
        result = false;
        ModalResult = mrNone;
      }
  }
  return result;
}

void TPreviewOptionsForm::ChangeValue(TEdit *Edit, double ToAdd)
{
  double Value = Edit->Text.ToDouble();
  Value += ToAdd;
  Edit->Text = Value;
  Edit->SelectAll();
}
void __fastcall TPreviewOptionsForm::ShadExampleMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Shift.Contains(ssLeft)) {
    TShape* sender = dynamic_cast<TShape*>(Sender);
    if (sender) {
      TColorDialog* dlg = NULL;
      try {
        dlg = new TColorDialog(NULL);
        dlg->Color = sender->Brush->Color;
        if (dlg->Execute())
          sender->Brush->Color = dlg->Color;
        delete dlg;
        dlg = NULL;
      } catch (Exception& e) {
        if (dlg)
          delete dlg;
      }
    }
  }
}

void __fastcall TPreviewOptionsForm::ShadowWidthEdKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  TEdit* edit = dynamic_cast<TEdit*>(Sender);
  if (edit) {
    if (Key == VK_UP)
      ChangeValue(edit, 1);
    if (Key == VK_DOWN)
      ChangeValue(edit, -1);
    if (Key == VK_UP || Key == VK_DOWN)
      Key = 0;
  }        
}
//---------------------------------------------------------------------------
void __fastcall TPreviewOptionsForm::CSpinButton5DownClick(TObject *Sender)
{
  TCSpinButton* but = dynamic_cast<TCSpinButton*>(Sender);
  if (but) {
    TEdit* edit = dynamic_cast<TEdit*>(but->FocusControl);
    if (edit)
      ChangeValue(edit, -1);
  }        
}
//---------------------------------------------------------------------------
void __fastcall TPreviewOptionsForm::CSpinButton5UpClick(TObject *Sender)
{
  TCSpinButton* but = dynamic_cast<TCSpinButton*>(Sender);
  if (but) {
    TEdit* edit = dynamic_cast<TEdit*>(but->FocusControl);
    if (edit)
      ChangeValue(edit, 1);
  }        
}

void __fastcall TPreviewOptionsForm::CSpinButton1DownClick(TObject *Sender)
{
  TCSpinButton* but = dynamic_cast<TCSpinButton*>(Sender);
  if (but) {
    TEdit* edit = dynamic_cast<TEdit*>(but->FocusControl);
    if (edit)
      ChangeValue(edit, -0.1);
  }
}

void __fastcall TPreviewOptionsForm::CSpinButton1UpClick(TObject *Sender)
{
  TCSpinButton* but = dynamic_cast<TCSpinButton*>(Sender);
  if (but) {
    TEdit* edit = dynamic_cast<TEdit*>(but->FocusControl);
    if (edit)
      ChangeValue(edit, 0.1);
  }
}

void __fastcall TPreviewOptionsForm::OffsetTopEdKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  TEdit* edit = dynamic_cast<TEdit*>(Sender);
  if (edit) {
    if (Key == VK_UP)
      ChangeValue(edit, 0.1);
    if (Key == VK_DOWN)
      ChangeValue(edit, -0.1);
    if (Key == VK_UP || Key == VK_DOWN)
      Key = 0;
  }
}

void __fastcall TPreviewOptionsForm::PrintFooterBoxClick(TObject *Sender)
{
  if (!CanUpdate)
    return;
  if (PrintFooterBox->Checked && !PageNumbersBox->Enabled && !PrintDateBox->Enabled) {
    PrintDateBox->Checked = true;
    PageNumbersBox->Checked = true;
  }
  SetHeaderFooterEnabled();
}

void __fastcall TPreviewOptionsForm::PageNumbersBoxClick(TObject *Sender)
{
  if (!CanUpdate)
    return;
  SetHeaderFooterEnabled();
}

void __fastcall TPreviewOptionsForm::PagesChange(TObject *Sender)
{
  OkB->Parent = Pages->ActivePage;
  CancelB->Parent = Pages->ActivePage;
}

void __fastcall TPreviewOptionsForm::FormKeyPress(TObject *Sender,
      char &Key)
{
  switch (Key) {
    case VK_ESCAPE: Key = 0; ModalResult = mrCancel; return;
    case VK_RETURN: Key = 0; ModalResult = mrOk; return;
  }
}

void __fastcall TPreviewOptionsForm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
  if (ModalResult == mrOk)
    SetOptions();
}

void __fastcall TPreviewOptionsForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TPreviewOptionsForm::CancelBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}



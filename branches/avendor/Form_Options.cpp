//---------------------------------------------------------------------------
#include <vcl.h>
#include "inifiles.hpp"
#pragma hdrstop

#include "Form_Options.h"
#include "Misc.h"
#include "Main.h"
#include "ConstRes.h"

#define KoefHeight 64
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TOptionsForm::TOptionsForm(void) : TForm((void*)NULL),
            rgn(NULL),
            OkB(NULL),
            CancelB(NULL),
            ModelsCheck(NULL),
            PresentCheck(NULL),
            ShowOptionsCheck(NULL),
            AutoStartPresCheck(NULL)
{
  rgn = CreateRoundRectRgn(0, 0, Width, KoefHeight * 4, KoefHeight, KoefHeight);
  SetWindowRgn(Handle, rgn, true);
  BigShape->Left   = 0;
  BigShape->Top    = 0;
  BigShape->Width  = ClientWidth - 1;
  BigShape->Height = KoefHeight * 4 - 1;

  LangLab->Caption = GetString(sLang);
  ModShowLab->Caption = GetString(sModDelay);
  PresentLab->Caption = GetString(sPresDelay);
  ModDelL->Caption = GetString(sDelay);
  PresDelL->Caption = GetString(sDelay);
  ModDelSL->Caption = GetString(sSec);
  PresDelSL->Caption = GetString(sSec);

  CancelB = new THighlightText(this);
  CancelB->Color         = (TColor)0x00815F1C;
  CancelB->Caption       = GetString(sCancel);
  CancelB->NormColor   = (TColor)0x00D0D7D7;
  CancelB->HighlColor  = clWhite;
  CancelB->HighlCursor = crHandPoint;
  CancelB->NormStyle   << fsBold;
  CancelB->HighlStyle  << fsBold << fsUnderline;
  CancelB->OnClick       = CancelBClick;
  Canvas->Font->Assign(CancelB->Font);
  Canvas->Font->Style    = CancelB->NormStyle;
  CancelB->Left          = Width - 60 - Canvas->TextWidth(CancelB->Caption);
  CancelB->Top           = BottomShape->Top + BottomShape->Pen->Width + (BottomShape->Height - CancelB->Height)/2;
  CancelB->Parent        = this;

  OkB = new THighlightText(this);
  OkB->Color         = (TColor)0x00815F1C;
  OkB->Caption       = "Ok";
  OkB->NormColor   = (TColor)0x00D0D7D7;
  OkB->HighlColor  = clWhite;
  OkB->HighlCursor = crHandPoint;
  OkB->NormStyle   << fsBold;
  OkB->HighlStyle  << fsBold << fsUnderline;
  OkB->OnClick       = OkBClick;
  Canvas->Font->Assign(OkB->Font);
  Canvas->Font->Style = CancelB->NormStyle;
  OkB->Left           = CancelB->Left - Canvas->TextWidth(OkB->Caption) - 40;
  OkB->Top            = BottomShape->Top + BottomShape->Pen->Width + (BottomShape->Height - OkB->Height)/2;
  OkB->Parent         = this;

  Title->Caption = GetString(sOptions);

  LangBox->ItemIndex = 2;

  TFontStyles fs;
  fs.Clear();

  ModelsCheck = new TPaulCheckBox(this);
  ModelsCheck->BackColor = this->Color;
  ModelsCheck->Font->Color = (TColor)0x00604715;
  ModelsCheck->Font->Style = fs;
  ModelsCheck->DisabledColor = clGrayText;
  ModelsCheck->SetBounds(152, 55, 253, 17);
  ModelsCheck->Caption = GetString(sUseDefMod);
  ModelsCheck->OnClick = ModelsCheckClick;

  PresentCheck = new TPaulCheckBox(this);
  PresentCheck->BackColor = this->Color;
  PresentCheck->Font->Color = (TColor)0x00604715;
  PresentCheck->Font->Style = fs;
  PresentCheck->DisabledColor = clGrayText;
  PresentCheck->SetBounds(152, 120, 253, 17);
  PresentCheck->Caption = GetString(sUseDefPages);
  PresentCheck->OnClick = PresentCheckClick;

  AutoStartPresCheck = new TPaulCheckBox(this);
  AutoStartPresCheck->BackColor = this->Color;
  AutoStartPresCheck->Font->Color = (TColor)0x00604715;
  AutoStartPresCheck->Font->Style = fs;
  AutoStartPresCheck->DisabledColor = clGrayText;
  AutoStartPresCheck->SetBounds(152, 160, 253, 17);
  AutoStartPresCheck->Caption = GetString(sAutoStartPres);

  ShowOptionsCheck = new TPaulCheckBox(this);
  ShowOptionsCheck->BackColor = this->Color;
  ShowOptionsCheck->Font->Color = (TColor)0x00604715;
  fs << fsBold;
  ShowOptionsCheck->Font->Style = fs;
  ShowOptionsCheck->DisabledColor = clGrayText;
  ShowOptionsCheck->Caption = GetString(sShowOpt);
  Canvas->Font->Assign(ShowOptionsCheck->Font);
  int width = Canvas->TextWidth(ShowOptionsCheck->Caption) + 22;
  if (width > Title->Width)
    width = Title->Width;
  ShowOptionsCheck->SetBounds(Title->Left + (Title->Width - width)/2, 193, width, 17);

  ModelsShape->Left = ModDelL->Left + ModDelL->Width + 2;
  ModelsDelay->Left = ModelsShape->Left + 1;
  ModDelSL->Left = ModelsShape->Left + ModelsShape->Width + 2;

  PresentShape->Left = PresDelL->Left + PresDelL->Width + 2;
  PresentDelay->Left = PresentShape->Left + 1;
  PresDelSL->Left = PresentShape->Left + PresentShape->Width + 2;

  GetOptions();
}

//---------------------------------------------------------------------------
__fastcall TOptionsForm::~TOptionsForm(void)
{
  if (OkB)                delete OkB;
  if (CancelB)            delete CancelB;
  if (ModelsCheck)        delete ModelsCheck;
  if (PresentCheck)       delete PresentCheck;
  if (ShowOptionsCheck)   delete ShowOptionsCheck;
  if (AutoStartPresCheck) delete AutoStartPresCheck;
  DeleteObject(rgn);
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::WndProc(Messages::TMessage &Message)
{
  TForm::WndProc(Message);
  switch (Message.Msg) {

    case WM_LBUTTONDOWN  :
    case WM_LBUTTONDBLCLK: {
      FMouseDown = true;
      dX = -Message.LParamLo;
      dY = -Message.LParamHi;
      break;
    }

    case WM_LBUTTONUP    : FMouseDown = false; break;

    case WM_MOUSEMOVE    : {
      if (FMouseDown) {
        int x = Message.LParamLo;
        int y = Message.LParamHi;
        int screen_x = GetSystemMetrics(SM_CXSCREEN);
        int screen_y = GetSystemMetrics(SM_CYSCREEN);
        if ( !screen_x ) screen_x = 10000;
        if ( !screen_y ) screen_y = 10000;
        while ( x > screen_x ) x -= 32768;
        while ( y > screen_y ) y -= 32768;
        Left += x + dX;
        Top  += y + dY;
      }
      break;
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::CancelBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::LangBoxDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* canvas = LangBox->Canvas;
  TColor oldbrColor = canvas->Brush->Color;
  TColor oldpenColor = canvas->Pen->Color;
  TFontStyles oldfont = canvas->Font->Style;

  if (State.Contains(odSelected) || State.Contains(odFocused)) {
    canvas->Brush->Color = (TColor)0x00815F1C;
    canvas->Pen->Color = (TColor)0x00D0D7D7;
    TFontStyles fs;
    fs.Clear();
    fs << fsBold;
    canvas->Font->Style = fs;
    canvas->Font->Color = (TColor)0x00F4F4F2;
  }

  canvas->FillRect(Rect);
  AnsiString caption = LangBox->Items->Strings[Index];
  InflateRect(&Rect, -4, 0);
  DrawText(canvas->Handle, caption.c_str(), caption.Length(), &Rect, DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
  canvas->Brush->Color = oldbrColor;
  canvas->Pen->Color = oldpenColor;
  canvas->Font->Style = oldfont;
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_RETURN && OkB->Enabled) {
    Key = 0;
    ModalResult = mrOk;
  }

  if (Key == VK_ESCAPE) {
    Key = 0;
    ModalResult = mrCancel;
  }
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::ModelsCheckClick(TObject *Sender)
{
  if (ModelsCheck->Checked) {
    ModDelL->Font->Color = clGrayText;
    ModDelSL->Font->Color = clGrayText;
    ModelsShape->Pen->Color = clGrayText;
    ModelsDelay->Enabled = false;

    try {
      StrToInt(ModelsDelay->Text);
    } catch (Exception& e) {
      ModelsDelay->Text = Form1->ModelsDelay;
    }
  } else {
    ModDelL->Font->Color = (TColor)0x00604715;
    ModDelSL->Font->Color = (TColor)0x00604715;
    ModelsShape->Pen->Color = (TColor)0x00815F1C;
    ModelsDelay->Enabled = true;
  }
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::PresentCheckClick(TObject *Sender)
{
  if (PresentCheck->Checked) {
    PresDelL->Font->Color = clGrayText;
    PresDelSL->Font->Color = clGrayText;
    PresentShape->Pen->Color = clGrayText;
    PresentDelay->Enabled = false;

    try {
      StrToInt(PresentDelay->Text);
    } catch (Exception& e) {
      PresentDelay->Text = Form1->PagesDelay;
    }
  } else {
    PresDelL->Font->Color = (TColor)0x00604715;
    PresDelSL->Font->Color = (TColor)0x00604715;
    PresentShape->Pen->Color = (TColor)0x00815F1C;
    PresentDelay->Enabled = true;
 }
}

//---------------------------------------------------------------------------
void TOptionsForm::GetOptions()
{
  TIniFile* lng = NULL;
  try {
    AnsiString path = GetNormalDirFromDir(ExtractFilePath(Application->ExeName)) + "data\\RAO_Explorer.lng";
    lng = new TIniFile(path);
    lng->ReadSections(LangBox->Items);
    delete lng; lng = NULL;
  } catch (Exception& e) {
    if (lng) delete lng;
  }
  int index = LangBox->Items->IndexOf(Form1->Language);
  if (index == -1) index = 0;
  LangBox->ItemIndex = index;
  ModelsDelay->Text = Form1->ModelsDelay;
  PresentDelay->Text = Form1->PagesDelay;
  ModelsCheck->Checked = Form1->UseModelsDelays;
  ModelsCheckClick(ModelsCheck);
  PresentCheck->Checked = Form1->UsePagesDelays;
  PresentCheckClick(PresentCheck);
  ShowOptionsCheck->Checked = Form1->ShowOptionsMenu;
  AutoStartPresCheck->Checked = Form1->AutoRunPresentation;
}

//---------------------------------------------------------------------------
void __fastcall TOptionsForm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
  if (ModalResult == mrOk) {
    try {
      Form1->Language = LangBox->Items->Strings[LangBox->ItemIndex];

      try {
        Form1->ModelsDelay = StrToInt(ModelsDelay->Text);
      } catch (Exception& e) {
        CanClose = false;
        e.Message = Format(GetString(sIvalidModDelay), OPENARRAY(TVarRec, (ModelsDelay->Text)));
        ModelsDelay->SelectAll();
        ModelsDelay->SetFocus();
        throw;
      }

      try {
        Form1->PagesDelay = StrToInt(PresentDelay->Text);
      } catch (Exception& e) {
        CanClose = false;
        e.Message = Format(GetString(sIvalidPresDelay), OPENARRAY(TVarRec, (PresentDelay->Text)));
        PresentDelay->SelectAll();
        PresentDelay->SetFocus();
        throw;
      }

      Form1->UseModelsDelays = ModelsCheck->Checked;
      Form1->UsePagesDelays = PresentCheck->Checked;
      Form1->AutoRunPresentation = AutoStartPresCheck->Checked;
      Form1->ShowOptionsMenu = ShowOptionsCheck->Checked;
    } catch (Exception& e) {
      CanClose = false;
      throw;
    }
  }
}


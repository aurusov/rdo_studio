//---------------------------------------------------------------------------
#include <vcl.h>
#include <dialogs.hpp>
#pragma hdrstop

#include "DrR_Exception.h"

#include "Pl_Form_ReportPreview.h"
#include "Pl_Form_Report_PreviewOptions.h"
#include "Report_Consts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern bool PrintDialogShowed;
//---------------------------------------------------------------------------
__fastcall TPreviewForm::TPreviewForm()
        : TForm((void*)NULL),
        CanResize(false),
        FPage(0),
        LastScaleValue(0),
        ScaleType(st_ZoomFit),
        NewWidth(0),
        NewHeight(0),
        ClHeight(0),
        ClWidth(0),
        LastItemIndex(-1)
{
  int instance = (int)HInstance;
  PrintB->Glyph->LoadFromResourceName(instance, "IDB_PRINT");
  PrintSetupB->Glyph->LoadFromResourceName(instance, "IDB_PRINTSETUP");
  ZoomFitB->Glyph->LoadFromResourceName(instance, "IDB_ZOOMFIT");
  ZoomPageWidthB->Glyph->LoadFromResourceName(instance, "IDB_ZOOMPAGEWIDTH");
  NoZoomB->Glyph->LoadFromResourceName(instance, "IDB_ZOOMFULL");
  OptionsB->Glyph->LoadFromResourceName(instance, "IDB_REPORTOPTIONS");
  PrevPageB->Glyph->LoadFromResourceName(instance, "IDB_PREVPAGE");
  NextPageB->Glyph->LoadFromResourceName(instance, "IDB_NEXTPAGE");
  TStrings* items = ScaleBox->Items;
  items->Add("300%");
  items->Add("250%");
  items->Add("150%");
  items->Add("100%");
  items->Add("75%");
  items->Add("50%");
  items->Add("25%");
  items->Add("10%");
  items->Add(LoadStr(sZoomWidthBox));
  items->Add(LoadStr(sZoomFitBox));
  ScaleBox->ItemIndex = -1;
  ScaleBox->ItemIndex = 9;
  LastItemIndex = 9;
  ScaleBox->Top = PrintB->Top + (PrintB->Height - ScaleBox->Height)/2;
}


__fastcall TPreviewForm::~TPreviewForm()
{
  Report->Previewing = false;
}

void __fastcall TPreviewForm::WndProc(Messages::TMessage &Message)
{
  TForm::WndProc(Message);
  switch (Message.Msg) {
    case WM_SIZE          : if (CanResize) ResizeReport(); break;
    case WM_DISPLAYCHANGE : {
      Report->GetDisplayParams();
      ResizeReport();
      break;
    }
    case WM_SETTINGCHANGE : {
      if (!PrintDialogShowed) {
        Printer()->Refresh();
        double lastscale = ScaleKoef;
        Printer()->PrinterIndex = -1;
        Report->FPrinter = Printer();
        Report->FPrinter->Orientation = Report->Orientation;
        Report->Init();
        Report->FullWidth = Report->DisplayPixelsPerMmX*Report->PageWidthMm;
        Report->CorrectOffsets();
        ResizeReport();
        ScaleKoef = lastscale;
        ResizeReport();
        SetStatusButtons();
      }
      break;
    }
  }

}

// Перерисовка отчета
void TPreviewForm::ResizeReport()
{
  ScrollBox->HorzScrollBar->Position = 0;
  ScrollBox->VertScrollBar->Position = 0;
  ScrollBox->HorzScrollBar->Visible = false;
  ScrollBox->VertScrollBar->Visible = false;
  ClWidth = ScrollBox->ClientWidth;
  ClHeight = ScrollBox->ClientHeight;
  ScrollBox->HorzScrollBar->Visible = true;
  ScrollBox->VertScrollBar->Visible = true;
  try {
    switch (ScaleType) {
      case st_ZoomFit : {
        ScrollBox->HorzScrollBar->Visible = false;
        ScrollBox->VertScrollBar->Visible = false;
        float kx, ky;
        ky = (float)ClHeight / ((float)Report->PrinterPageWidth*Report->KWidth);
        kx = (float)ClWidth / ((float)Report->PrinterPageWidth);
        if (kx > ky) {
          NewHeight = ClHeight - Report->Decrement;
          NewWidth  = NewHeight / Report->KWidth;
        } else {
          NewWidth = ClWidth - Report->Decrement;
          NewHeight = NewWidth * Report->KWidth;
        };
        LastScaleValue = (NewWidth/Report->FullWidth)*100;
        LastScaleType = ScaleType;
        if (ScaleBox->ItemIndex != -1)
          LastItemIndex = ScaleBox->ItemIndex;
        break;
      }
      case st_ZoomFitPageWidth : {
        ScrollBox->VertScrollBar->Visible = true;
        ScrollBox->HorzScrollBar->Visible = false;
        NewWidth = ClWidth - Report->Decrement;
        NewHeight = NewWidth*Report->KWidth;
        int val = NewHeight + Report->Decrement;
        if (val > ClHeight) {
          NewWidth -= GetSystemMetrics(SM_CXVSCROLL);
          NewHeight = NewWidth*Report->KWidth;
          val = NewHeight + Report->Decrement;
          if (val > ClHeight)
            ClWidth -= GetSystemMetrics(SM_CXVSCROLL);
          else {
            NewHeight = ClHeight - Report->Decrement;
            NewWidth = NewHeight / Report->KWidth;
          }
        }
        LastScaleValue = (NewWidth/Report->FullWidth)*100;
        LastScaleType = ScaleType;
        if (ScaleBox->ItemIndex != -1)
          LastItemIndex = ScaleBox->ItemIndex;
        break;
      }
      case st_NoZoom : {
        ReportImage->Left   = Report->ReportBorderWidth;
        ReportImage->Top    = Report->ReportBorderWidth;
        NewWidth = Report->DisplayPixelsPerMmX*Report->PageWidthMm;
        NewHeight = NewWidth*Report->KWidth;
        LastScaleValue = 100;
        LastScaleType = ScaleType;
        if (ScaleBox->ItemIndex != -1)
          LastItemIndex = ScaleBox->ItemIndex;
        break;
      }
      case st_Custom: {
        ReportImage->Left   = Report->ReportBorderWidth;
        ReportImage->Top    = Report->ReportBorderWidth;
        NewWidth = Report->DisplayPixelsPerMmX*Report->PageWidthMm*ScaleKoef;
        NewHeight = NewWidth*Report->KWidth;
        break;
      }
    }
    Report->UpdateReportPreview(NewWidth, NewWidth*Report->KWidth, FPage);
    doSetEnvironement();
  } catch (Exception& e) {
    throw;
  }
}

// Страница целиком
void TPreviewForm::doZoomFit()
{
  ScrollBox->HorzScrollBar->Visible = false;
  ScrollBox->VertScrollBar->Visible = false;
  double kx, ky;
  try {
    ky = (float)ScrollBox->ClientHeight / ((float)Report->PrinterPageWidth*Report->KWidth);
    kx = (float)ScrollBox->ClientWidth / ((float)Report->PrinterPageWidth);
    int NewHeight, NewWidth;
    if (kx > ky) {
      NewHeight = ScrollBox->ClientHeight - Report->Decrement;
      NewWidth  = NewHeight / Report->KWidth;
    } else {
      NewWidth = ScrollBox->ClientWidth - Report->Decrement;
      NewHeight = NewWidth * Report->KWidth;
    };
    Report->UpdateReportPreview(NewWidth, NewHeight, FPage);
    doSetEnvironement();
  } catch (Exception& e) {
    throw;
  }
}

// По ширине страницы
void TPreviewForm::doZoomPageWidth()
{
  ScrollBox->VertScrollBar->Visible = true;
  ScrollBox->HorzScrollBar->Visible = false;
  int NewWidth = ScrollBox->ClientWidth - Report->Decrement;
  int NewHeight = NewWidth*Report->KWidth;
  if (NewHeight > ScrollBox->ClientHeight) {
    NewWidth -= GetSystemMetrics(SM_CXVSCROLL);
    NewHeight = NewWidth*Report->KWidth;
  }
  Report->UpdateReportPreview(NewWidth, NewHeight, FPage);
  doSetEnvironement();
}

// Страница целиком
void TPreviewForm::doSetEnvironement()
{
  try {
    Border->Height = ReportImage->Height + 2*Report->PreviewCantWidth;
    Border->Width = ReportImage->Width + 2*Report->PreviewCantWidth;
    Shadow->Height = Border->Height;
    Shadow->Width = Border->Width;

    Border->Left = (ClWidth - Border->Width - Report->Decrement) / 2 + Report->ReportBorderWidth;
    if (Border->Left < Report->ReportBorderWidth)
      Border->Left = Report->ReportBorderWidth;
    PreviewBevel->Left = Border->Left - Report->ReportBorderWidth;
    Border->Top = (ClHeight - Border->Height - Report->Decrement) / 2 + Report->ReportBorderWidth;
    if (Border->Top < Report->ReportBorderWidth)
      Border->Top = Report->ReportBorderWidth;
    PreviewBevel->Top = Border->Top - Report->ReportBorderWidth;

    ReportImage->Left = Border->Left + Report->PreviewCantWidth;
    ReportImage->Top = Border->Top + Report->PreviewCantWidth;
    Shadow->Left = Border->Left + Report->ShadowWidth;
    Shadow->Top = Border->Top + Report->ShadowWidth;
    PreviewBevel->Width = Border->Width + 2*Report->ReportBorderWidth + Report->ShadowWidth;
    PreviewBevel->Height = Border->Height + 2*Report->ReportBorderWidth + Report->ShadowWidth;
  } catch (Exception& e) {
    throw;
  }
}

void __fastcall TPreviewForm::FormShow(TObject *Sender)
{
  Report->Previewing = true;
  FPage = 1;
  Report->Init();
  Report->FullWidth = Report->DisplayPixelsPerMmX*Report->PageWidthMm;
  ResizeReport();
  SetPageButtons();
  doSetStatus();
  bool printerexists = (Report->FPrinter && Report->FPrinter->Printers && Report->FPrinter->Printers->Count);
  PrintB->Enabled = printerexists;
  PrintSetupB->Enabled = printerexists;
  if (Screen->Cursor == crHourGlass)
    Screen->Cursor = crDefault;
}


void __fastcall TPreviewForm::PrintBClick(TObject *Sender)
{
  Report->Print(true);
}

void __fastcall TPreviewForm::PrintSetupBClick(TObject *Sender)
{
  TPrinterSetupDialog* dlg = NULL;
  try {
    dlg = new TPrinterSetupDialog(NULL);
    if (dlg && dlg->Execute()) {
      PrintDialogShowed = true;
      Report->FPrinter = Printer();
      Report->Orientation = Report->FPrinter->Orientation;
      //Report->GetPrinterParams();
      Report->Init();
      Report->FullWidth = Report->DisplayPixelsPerMmX*Report->PageWidthMm;
      Report->CorrectOffsets();
      ResizeReport();
      SetPageButtons();
      doSetStatus();
    }
    delete dlg;
    dlg = NULL;
  } catch (Exception& e) {
    delete dlg;
  }
}

void __fastcall TPreviewForm::OptionsBClick(TObject *Sender)
{
  TPreviewOptionsForm* dlg = NULL;
  try {
    dlg = new TPreviewOptionsForm(Report, this);
    dlg->Caption = LoadStr(sOptionsCaption);
    dlg->ShowModal();
    delete dlg;
    dlg = NULL;
  } catch (Exception& e) {
    if (dlg)
      delete dlg;
  }
}

void __fastcall TPreviewForm::PrevPageBClick(TObject *Sender)
{
  Page--;
}

void __fastcall TPreviewForm::NextPageBClick(TObject *Sender)
{
  Page++;
}

void TPreviewForm::SetPageButtons()
{
  if (Report->Pages == 1)
  {
    NextPageB->Enabled = false;
    PrevPageB->Enabled = false;
    return;
  }
  NextPageB->Enabled = true;
  PrevPageB->Enabled = true;
  if (FPage == Report->Pages)
    NextPageB->Enabled = false;
  if (FPage == 1)
    PrevPageB->Enabled = false;
}

void TPreviewForm::SetPage(int PageNumber)
{
  if (FPage != PageNumber) {
    Report->DrawTo = ReportImage->Picture->Bitmap->Canvas;
    Report->DrawReportPage(PageNumber);
    Report->DrawPrintableArea();
    // Рисуем область отчета
    Report->RectAtMm(0, 0, Report->PageWidthMm - Report->OffsetLeft - Report->OffsetRight, Report->PageHeightMm - Report->OffsetTop - Report->OffsetBottom, clWhite, bsClear, clBlack, psDot, 1);
    FPage = PageNumber;
    SetPageButtons();
    doSetStatus();
  }
}

void TPreviewForm::doSetStatus()
{
  AnsiString str = LoadStr(sPages);
  TVarRec v[] = { FPage, Report->Pages };
  StatusBar->Panels->Items[0]->Text = Format(str, v, ARRAYSIZE(v) - 1);
  str = LoadStr(sPaperSize);
  TVarRec v1[] = { Report->PageHeightMm, Report->PageWidthMm };
  StatusBar->Panels->Items[1]->Text = Format(str, v1, ARRAYSIZE(v1) - 1);
}

void TPreviewForm::SetStatusButtons()
{
  doSetStatus();
  SetPageButtons();
}
void __fastcall TPreviewForm::ScaleBoxChange(TObject *Sender)
{
  try {
    if (ScaleBox->ItemIndex != -1) {
      AnsiString str = ScaleBox->Items->Strings[ScaleBox->ItemIndex];
      switch (ScaleBox->ItemIndex) {
        case 8: {
          ScaleType = st_ZoomFitPageWidth;
          ZoomPageWidthB->Down = true;
          break;
        }
        case 9: {
          ScaleType = st_ZoomFit;
          ZoomFitB->Down = true;
          break;
        }
        case 3: {
          ScaleType = st_NoZoom;
          NoZoomB->Down = true;
          break;
        }
        default: {
          ScaleType = st_Custom;
          ZoomFitB->Down = false;
          ZoomPageWidthB->Down = false;
          NoZoomB->Down = false;
          break;
        }
      }
      int Val = 100;
      if (ScaleType == st_Custom) {
        if (*str.AnsiLastChar() == '%')
          str.Delete(str.Length(), 1);
        Val = str.ToInt();
      }
      if (Val < 10 || Val > 300) {
        AnsiString str = LoadStr(sScaleRangeError);
        TVarRec v[] = { "10%", "300%" };
        throw RubberError(Format(str, v, ARRAYSIZE(v) - 1));
      }
      ScaleKoef = ((double)Val)/100.0;
      if (Val != LastScaleValue || ScaleType != LastScaleType) {
        ResizeReport();
        LastScaleValue = Val;
        LastScaleType = ScaleType;
        if (ScaleBox->ItemIndex != -1)
          LastItemIndex = ScaleBox->ItemIndex;
      }
    }
  } catch (Exception& e) {
    AnsiString str = LastScaleValue;
    ScaleType = LastScaleType;
    str += "%";
    ScaleBox->Text = str;
    if (LastScaleType != st_Custom && LastItemIndex != -1)
      ScaleBox->ItemIndex = LastItemIndex;
    ScaleBox->SelectAll();
    switch (ScaleType) {
      case st_ZoomFit          : ZoomFitB->Down = true; break;
      case st_ZoomFitPageWidth : ZoomPageWidthB->Down = true; break;
      case st_NoZoom           : NoZoomB->Down = true; break;
    }
    throw;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPreviewForm::ScaleBoxKeyPress(TObject *Sender, char &Key)
{
  try {
    if (Key == VK_RETURN) {
      ScaleType = st_Custom;
      ZoomFitB->Down = false;
      ZoomPageWidthB->Down = false;
      NoZoomB->Down = false;
      AnsiString str = ScaleBox->Text;
      if (str.Length()) {
        if (*str.AnsiLastChar() == '%') {
          str.Delete(str.Length(), 1);
        } else {
          str = str.Trim();
          ScaleBox->Text = str + "%";
        }
        str = str.Trim();
        AnsiString comparestr = str + "%";
        if (comparestr == ScaleBox->Items->Strings[3]) {
          ScaleType = st_NoZoom;
          NoZoomB->Down = true;
          ScaleBox->ItemIndex = 3;
        }
        int Val = str.ToInt();
        if (Val < 10 || Val > 300) {
          AnsiString str = LoadStr(sScaleRangeError);
          TVarRec v[] = { "10%", "300%" };
          throw RubberError(Format(str, v, ARRAYSIZE(v) - 1));
        }
        ScaleKoef = ((double)Val)/100.0;
        ScaleBox->SelectAll();
        if (ScaleKoef != LastScaleValue || ScaleType != LastScaleType) {
          ResizeReport();
          LastScaleValue = Val;
          LastScaleType = ScaleType;
          if (ScaleBox->ItemIndex != -1)
            LastItemIndex = ScaleBox->ItemIndex;
        }
      }
    } else
      ScaleBox->ItemIndex = -1;
  } catch (Exception& e) {
    AnsiString str = LastScaleValue;
    ScaleType = LastScaleType;
    str += "%";
    ScaleBox->Text = str;
    if (LastScaleType != st_Custom && LastItemIndex != -1)
      ScaleBox->ItemIndex = LastItemIndex;
    ScaleBox->SelectAll();
    switch (ScaleType) {
      case st_ZoomFit          : ZoomFitB->Down = true; break;
      case st_ZoomFitPageWidth : ZoomPageWidthB->Down = true; break;
      case st_NoZoom           : NoZoomB->Down = true; break;
    }
    throw;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPreviewForm::ZoomFitBClick(TObject *Sender)
{
  ScaleType = st_ZoomFit;
  ScaleBox->ItemIndex = 9;
  ResizeReport();
}
//---------------------------------------------------------------------------

void __fastcall TPreviewForm::ZoomPageWidthBClick(TObject *Sender)
{
  ScaleType = st_ZoomFitPageWidth;
  ScaleBox->ItemIndex = 8;
  ResizeReport();
}
//---------------------------------------------------------------------------

void __fastcall TPreviewForm::NoZoomBClick(TObject *Sender)
{
  ScaleType = st_NoZoom;
  ScaleBox->ItemIndex = 3;
  ResizeReport();
}
//---------------------------------------------------------------------------

void __fastcall TPreviewForm::FormMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
  Handled = true;        
}
//---------------------------------------------------------------------------


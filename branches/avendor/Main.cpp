//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "Main.h"
#include "Form_Options.h"
#include "ConstRes.h"
#include "Misc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner),
            FCurMenuItemPos(0),
            FCurrentPath(""),
            FPicViewer(NULL),
            FMainPicViewer(NULL),
            FRAOProcess(0),
            Processes(NULL),
            SelectedMenu(mkNone),
            bShowRunned(false),
            FPageCount(0),
            FCurPage(0),
            FAutoRunPresentation(false),
            bPresentationRunned(false),
            FLanguage(""),
            FShowOptionsMenu(true),
            FUseModelsDelays(true),
            FModelsDelay(30),
            FUsePagesDelays(true),
            FPagesDelay(30),
            FCurModelDelay(0),
            FCurPageDelay(0),
            FStartUp(true)
{
  Application->OnException = AppException;
  FCurrentPath = GetNormalDirFromDir(ExtractFilePath(Application->ExeName));
  FImagesPath  = GetNormalDirFromDir(FCurrentPath + "data\\images");
  CreateMenuItem(MenuAbout, AboutImage, 0, "");
  CreateMenuItem(MenuExplore, ExploreImage, 1, "");
  CreateMenuItem(MenuShow, ShowImage, 2, "");
  CreateMenuItem(MenuPresentation, PresentImage, 3, "");
  CreateMenuItem(MenuExit, ExitImage, 4, "");
  CreateMenuItem(MenuOptions, OptionsImage, 11, "");
  MenuOptions->OnClick = OptionsClick;

  FPicViewer = new TPictureViewer(PicView, PicView->Color, FrameShape->Pen->Color);
  FPicViewer->UpdateImage("");

  FMainPicViewer = new TPictureViewer(MainPicView, MainPicView->Color, FrameShape->Pen->Color);
  FMainPicViewer->UpdateImage("");

  ModelsL->Caption = "";

  NavModelRun = new THighlightText(this);
  NavModelRun->Font->Name = "Tahoma";
  NavModelRun->Font->Size = 14;
  NavModelRun->NormColor = (TColor)0x000000D5;
  NavModelRun->HighlCursor = crHandPoint;
  NavModelRun->HighlStyle << fsUnderline;
  NavModelRun->HighlColor = clRed;
  NavModelRun->Color = NavShape->Brush->Color;
  NavModelRun->Parent = this;
  NavModelRun->OnClick = RunClick;

  NavModelSource = new THighlightText(ViewSourcePanel);
  NavModelSource->Font->Name = "Tahoma";
  NavModelSource->Font->Size = 14;
  NavModelSource->NormColor = (TColor)0x00932100;
  NavModelSource->HighlCursor = crHandPoint;
  NavModelSource->HighlStyle << fsUnderline;
  NavModelSource->HighlColor = clBlue;
  NavModelSource->Color = NavShape->Brush->Color;
  NavModelSource->Parent = ViewSourcePanel;
  NavModelSource->OnClick = ViewSourceClick;

  CreateSourceLabel(NavModelPAT, "PAT");
  CreateSourceLabel(NavModelRTP, "RTP");
  CreateSourceLabel(NavModelRSS, "RSS");
  CreateSourceLabel(NavModelOPR, "OPR");
  CreateSourceLabel(NavModelFRM, "FRM");
  CreateSourceLabel(NavModelFUN, "FUN");
  CreateSourceLabel(NavModelDPT, "DPT");
  CreateSourceLabel(NavModelSMR, "SMR");
  CreateSourceLabel(NavModelPMD, "PMD");
  CreateSourceLabel(NavModelPMW, "PMV");
  CreateSourceLabel(NavModelTRC, "TRC");

  NavStopShow = new THighlightText(this);
  NavStopShow->Font->Name = "Tahoma";
  NavStopShow->Font->Size = 14;
  NavStopShow->NormColor = (TColor)0x000000D5;
  NavStopShow->HighlCursor = crHandPoint;
  NavStopShow->HighlStyle << fsUnderline;
  NavStopShow->HighlColor = clRed;
  NavStopShow->Color = NavShape->Brush->Color;
  NavStopShow->Visible = false;
  NavStopShow->Parent = this;
  NavStopShow->OnClick = StopShowClick;

  EMail = new THighlightText(BriefPanel);
  EMail->Color = BriefPanel->Color;
  EMail->NormColor   = (TColor)0x00932100;
  EMail->HighlColor  = clBlue;
  EMail->HighlCursor = crHandPoint;
  EMail->NormStyle   << fsBold;
  EMail->HighlStyle  << fsBold << fsUnderline;
  EMail->OnClick       = EMailClick;
  EMail->Parent        = BriefPanel;

  Web = new THighlightText(BriefPanel);
  Web->Color = BriefPanel->Color;
  Web->NormColor   = (TColor)0x00932100;
  Web->HighlColor  = clBlue;
  Web->HighlCursor = crHandPoint;
  Web->NormStyle   << fsBold;
  Web->HighlStyle  << fsBold << fsUnderline;
  Web->OnClick       = WebClick;
  Web->Parent        = BriefPanel;

  CreateHotImage(GoBegin, PresentNav, 5, 5, "");
  GoBegin->OnClick = BeginClick;
  CreateHotImage(PageForward, PresentNav, 7, 7, "");
  PageForward->OnClick = ForwardClick;
  CreateHotImage(PageBackward, PresentNav, 6, 6, "");
  PageBackward->OnClick = BackwardClick;
  CreateHotImage(GoEnd, PresentNav, 8, 8, "");
  GoEnd->OnClick = EndClick;
  CreateHotImage(RunStop, PresentNav, 9, 9, "");
  RunStop->OnClick = RunPresentClick;

  LoadSettings();

  LONG style = GetWindowLong(Handle, GWL_STYLE);
  style ^= WS_OVERLAPPEDWINDOW;
  SetWindowLong(Handle, GWL_STYLE, style);
  SetWindowPos(Handle, HWND_BOTTOM, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);

  Processes = new TList();

  MenuClick(MenuAbout);
  FStartUp = false;
}

//---------------------------------------------------------------------------
__fastcall TForm1::~TForm1()
{
  StopRAOs();
  if (Processes) delete Processes;
  SaveSettings();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateMenuItem(System::TObject* Sender, const bool HighLighted)
{
  if (dynamic_cast<THighlightText*>(Sender)) {
    THighlightText* control = (THighlightText*)Sender;
    TImageList* list = Images;
    if (HighLighted || control->StayHighlighted) list = HotImages;
    control->Image->Canvas->FillRect(TRect(0, 0, control->Image->Width, control->Image->Height));
    list->Draw(control->Image->Canvas, 0, 0, control->Tag, true);
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::MenuClick(System::TObject* Sender)
{
  try {
    LockWindowUpdate(Handle);
    if (dynamic_cast<THighlightText*>(Sender)) {
      for (int i = 0; i < ControlCount; i++)
        if (dynamic_cast<THighlightText*>(Controls[i]))
          ((THighlightText*)Controls[i])->StayHighlighted = false;
      THighlightText* control = (THighlightText*)Sender;

      if (control != MenuOptions) {
        MenuL->Caption = control->Caption;
        control->StayHighlighted = true;
      }

      if (control == MenuExit) Close();

      if (control == MenuAbout && MenuAbout->StayHighlighted) {
        if (SelectedMenu != mkAbout) {
          HideAll();
          SelectedMenu = mkAbout;
          if (!bPresentationRunned) {
            BriefPanel->Visible = true;
            bool transp;
            AnsiString path = GetFileName(nkBrief, transp);
            FMainPicViewer->UpdateImage(path, transp);
            LockWindowUpdate(NULL);
            RichEdit->Lines->LoadFromFile(FBriefPath + "\\brief.rtf");
          } else {
            ShowTypedMessage(GetString(sStopPresPInfo), MB_ICONWARNING);
          }
        }
      }

      if (control == MenuExplore) {
        if (SelectedMenu != mkExplore) {
          SelectedMenu = mkExplore;
          HideAll();
          if (!bPresentationRunned) {
            if (!bShowRunned) {
              int model = -1;
              if (ModelsTree->Items->Count > 1)
                model = ModelsTree->Selected->Index;
              LoadModels();
              if (model != -1)
                ModelsTree->Selected = ModelsTree->Items->Item[0]->Item[model];
            } else {
              ShowTypedMessage(GetString(sStopModToExplore), MB_ICONWARNING);
            }
            ModelsPanel->Visible = true;
            ModelsTree->SetFocus();
          } else {
            ShowTypedMessage(GetString(sStopPresToExplore), MB_ICONWARNING);
          }
        }
      }

      if (control == MenuShow) {
        if (!bShowRunned) {
          SelectedMenu = mkModelsShow;
          HideAll();
          if (!bPresentationRunned) {
            if (SelectedMenu != mkExplore)
              LoadModels();
            if (ModelsTree->Items->Count > 1) {
              ModelsPanel->Visible = true;
              ModelsTree->SetFocus();
              NavStopShow->Visible = true;
              bShowRunned = true;
              if (ModelsTree->Selected == ModelsTree->Items->Item[0])
                ModelsTree->Selected = ModelsTree->Items->Item[0]->Item[0];
              FCurModel = ModelsTree->Selected->Index;
              Timer->Interval = FCurModelDelay * 1000;
              RunClick(NULL);
              Timer->Enabled = true;
            }
          } else {
            ShowTypedMessage(GetString(sStopPresToReview), MB_ICONWARNING);
          }
        } else {
          ShowTypedMessage(GetString(sReviewRunning), MB_ICONINFORMATION);
        }
      }

      if (control == MenuPresentation) {
        if (SelectedMenu != mkPresentation) {
          SelectedMenu = mkPresentation;
          if (!bPresentationRunned) {
            HideAll();
            if (!bShowRunned) {
              PresentPanel->Visible = true;
              TIniFile* info = NULL;
              TStringList* list;
              try {
                info = new TIniFile(FPresentPath + "presentation.ini");
                list = new TStringList();
                info->ReadSections(list);
                FPageCount = list->Count;
                delete list; list = NULL;
                delete info; info = NULL;
              } catch (Exception& e) {
                if (list) delete list;
                if (info) delete info;
              }
              if (FPageCount) {
                PresentNav->Visible = true;
                ViewPage(0);
                if (FAutoRunPresentation)
                  RunPresentClick(RunStop);
              }
            } else {
              ShowTypedMessage(GetString(sStopRevToPres), MB_ICONWARNING);
            }
          } else {
            ShowTypedMessage(GetString(sPresentRunning), MB_ICONINFORMATION);
          }
        }
      }
    }

    LockWindowUpdate(NULL);
  } catch (Exception& e) {
    LockWindowUpdate(NULL);
    throw;
  }
}

//---------------------------------------------------------------------------
void TForm1::CreateMenuItem(THighlightText* &Item, TImage* &Image, const int ImageIndex, const AnsiString Caption)
{
  Item = new THighlightText(this);
  Item->Caption = Caption;
  Item->Font->Name = "Verdana";
  Item->Font->Size = 14;
  Item->NormColor = (TColor)0x00737373;
  Item->HighlCursor = crHandPoint;
  Image->Left = MenuShape->Left + 10;
  Item->Left = Image->Left + Image->Width + 5;
  Image->Top  = FCurMenuItemPos;
  Item->Top  = Image->Top + (Image->Height - Item->Height)/2;
  Item->HighlColor = clBlue;
  Item->Color = MenuShape->Brush->Color;
  Item->Parent = this;
  Item->Image = Image;
  Item->Tag = ImageIndex;
  Item->OnHighLight = UpdateMenuItem;
  Item->OnClick = MenuClick;
  FCurMenuItemPos = Image->Top + Image->Height + 20;
}

//---------------------------------------------------------------------------
void TForm1::SetMenuItemPos(THighlightText* &Item)
{
  if (Item && Item->Image) {
    Item->Image->Left = MenuShape->Left + 10;
    Item->Left = Item->Image->Left + Item->Image->Width + 5;
    Item->Image->Top  = FCurMenuItemPos;
    Item->Top  = Item->Image->Top + (Item->Image->Height - Item->Height)/2;
    FCurMenuItemPos = Item->Image->Top + Item->Image->Height + 20;
  }
}

//---------------------------------------------------------------------------
void TForm1::LoadModels()
{
  TIniFile* models = NULL;
  TStringList* sections = NULL;
  try {
    models = new TIniFile(FModelsPath + "models.ini");
    sections = new TStringList();
    models->ReadSections(sections);
    ModelsTree->Items->Item[0]->DeleteChildren();
    for (int i = 0; i < sections->Count; i++) {
      TTreeNode* node = ModelsTree->Items->AddChild(ModelsTree->Items->Item[0], sections->Strings[i]);
      node->ImageIndex = 1;
      node->SelectedIndex = 2;
    }
    ModelsTree->Items->Item[0]->Expand(false);
    delete sections; sections = NULL;
    delete models; models = NULL;
    ModelsTree->Selected = ModelsTree->Items->Item[0];
  } catch (Exception &e) {
    if (models) delete models;
    if (sections) delete sections;
    throw;
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ModelsTreeChange(TObject *Sender, TTreeNode *Node)
{
  try {
    LockWindowUpdate(Handle);

    if (SelectedMenu == mkExplore && ModelsTree->Selected != ModelsTree->Items->Item[0] && !bShowRunned) {
      NavModelRun->Visible = true;
      ViewSourcePanel->Visible = true;
    } else {
      NavModelRun->Visible = false;
      ViewSourcePanel->Visible = false;
    }
    SourcePanel->Visible = false;
    ModelsRichEdit->WordWrap = true;
    ModelsRichEdit->PlainText = false;
    ModelsRichEdit->Font->Name = "Verdana";
    LockWindowUpdate(NULL);
  } catch (Exception &e) {
    LockWindowUpdate(NULL);
  }

  if (Node == ModelsTree->Items->Item[0]) {
    ModelL->Caption = "";
    FPicViewer->UpdateImage("");
    PicView->Visible = false;
    ModelsL->Visible = true;
    ModelsRichEdit->Lines->LoadFromFile(FModelsPath + "models.rtf");
    return;
  };

  ModelsL->Visible = false;
  TIniFile* models = NULL;

  if (!FUseModelsDelays)
    FCurModelDelay = FModelsDelay;

  try {
    models = new TIniFile(FModelsPath + "models.ini");
    FCurModelPath = FModelsPath + models->ReadString(Node->Text, "Folder", "") + "\\";
    AnsiString path = FImagesPath + models->ReadString(Node->Text, "Image", "");
    bool transp = models->ReadBool(Node->Text, "TranspImage", false);
    FPicViewer->UpdateImage(path, transp);
    PicView->Visible = true;
    ModelL->Caption = models->ReadString(Node->Text, "Name", "");
    ModelsRichEdit->Lines->LoadFromFile(FCurModelPath + Node->Text + ".rtf");
    if (FUseModelsDelays)
      FCurModelDelay = models->ReadInteger(Node->Text, "Delay", FModelsDelay);
    delete models; models = NULL;
  } catch (Exception &e) {
    if (models) delete models;
    throw;
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::BriefPanelResize(TObject *Sender)
{
  int width = MainPicView->Width + BMSTUL->Width + 10;

  MainPicView->Top = 0;
  MainPicView->Left = (BriefPanel->Width - width)/2;

  BMSTUL->Left = MainPicView->Left + MainPicView->Width + 10;
  DeptL->Left = BMSTUL->Left;
  TelL->Left = BMSTUL->Left;
  EmailL->Left = BMSTUL->Left;
  WebL->Left = BMSTUL->Left;

  BMSTUL->Top = MainPicView->Top;
  DeptL->Top = BMSTUL->Top + BMSTUL->Height + 10;
  TelL->Top = DeptL->Top + DeptL->Height + 5;
  EmailL->Top = TelL->Top + TelL->Height + 5;
  WebL->Top = EmailL->Top + EmailL->Height + 5;;

  if (EMail) {
    EMail->Left          = EmailL->Left + EmailL->Width + 3;
    EMail->Top           = EmailL->Top;
  }

  if (Web) {
    Web->Left          = WebL->Left + WebL->Width + 3;
    Web->Top           = WebL->Top;
  }

  RichEdit->Top = MainPicView->Top + MainPicView->Height + 20;
  RichEdit->Left = 20;
  RichEdit->Width = BriefPanel->Width - 40;
  RichEdit->Height = BriefPanel->Height - RichEdit->Top;

  if (FMainPicViewer)
    FMainPicViewer->UpdateSize();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
  SetWindowPos(Handle, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOMOVE);

  MenuShape->Left = FrameShape->Pen->Width;
  MenuShape->Top = FrameShape->Pen->Width;
  MenuShape->Height = Height - 2*FrameShape->Pen->Width;
  MenuShape->Width = Width / 4;

  CapShape->Top = 30;
  CapShape->Left = 0;
  CapShape->Width = Width;
  CapShape->Height = 50;

  NavShape->Top = Height - 30 - 50;
  NavShape->Left = 0;
  NavShape->Width = Width;
  NavShape->Height = 50;

  CapLab->Top = CapShape->Top + (CapShape->Height - CapLab->Height)/2;
  CapLab->Left = MenuShape->Left + (MenuShape->Width - CapLab->Width)/2;

  RAOImage->Top  = CapShape->Top + CapShape->Height + 20;
  int space = MenuShape->Width - (RAOImage->Width + OpL->Width);
  RAOImage->Left = MenuShape->Left + space / 3;
  ResL->Left = RAOImage->Left + RAOImage->Width + space / 3;
  ActL->Left = ResL->Left;
  OpL->Left  = ResL->Left;
  space = RAOImage->Height - ResL->Height - ActL->Height - OpL->Height;
  ResL->Top = RAOImage->Top;
  ActL->Top = ResL->Top + ResL->Height + space / 2;
  OpL->Top = ActL->Top + ActL->Height + space / 2;

  MenuL->Top = CapShape->Top + (CapShape->Height - MenuL->Height)/2;
  MenuL->Left = MenuShape->Left + MenuShape->Width + 20;
  MenuL->Width = Width - 2*FrameShape->Pen->Width - MenuShape->Width - 40;

  FCurMenuItemPos = OpL->Top + OpL->Height + 40;

  SetMenuItemPos(MenuAbout);
  SetMenuItemPos(MenuExplore);
  SetMenuItemPos(MenuShow);
  SetMenuItemPos(MenuPresentation);

  FCurMenuItemPos = NavShape->Top - ExitImage->Height - 20;
  SetMenuItemPos(MenuExit);

  FCurMenuItemPos = PresentImage->Top + PresentImage->Height + (ExitImage->Top - OptionsImage->Height - (PresentImage->Top + PresentImage->Height))/2;
  SetMenuItemPos(MenuOptions);

  int top = CapShape->Top + CapShape->Height + 20;
  int left = MenuShape->Left + MenuShape->Width + 20;
  int width = Width - (MenuShape->Left + MenuShape->Width + 2*FrameShape->Pen->Width + 40);
  int height = NavShape->Top - top - 20;
  BriefPanel->SetBounds(left, top, width, height);
  ModelsPanel->SetBounds(left, top, width, height);
  PresentPanel->SetBounds(left, top, width, height);
  PresentNav->SetBounds(left, NavShape->Top + NavShape->Pen->Width, width, NavShape->Height - 2*NavShape->Pen->Width);
  ViewSourcePanel->SetBounds(ModelsPanel->Left + ModelsRichEdit->Left, NavShape->Top + NavShape->Pen->Width, Width - (ModelsPanel->Left + ModelsRichEdit->Left) - FrameShape->Pen->Width, NavShape->Height - 2*NavShape->Pen->Width);
  SourcePanel->SetBounds(ModelsPanel->Left + ModelsRichEdit->Left, NavShape->Top + NavShape->Pen->Width, ModelsRichEdit->Width, NavShape->Height - 2*NavShape->Pen->Width);

  if (NavStopShow) {
    NavStopShow->Top = NavShape->Top + (NavShape->Height - NavStopShow->Height)/2;
    NavStopShow->Left = ModelsPanel->Left + (ModelsPanel->Width - NavStopShow->Width)/2;
  }

  if (NavModelRun) {
    NavModelRun->Left = ModelsPanel->Left + ModelsTree->Left + (ModelsTree->Width - NavModelRun->Width)/2;
    NavModelRun->Top  = NavShape->Top + (NavShape->Height - NavModelRun->Height)/2;
    if (SelectedMenu == mkExplore && ModelsTree->Selected != ModelsTree->Items->Item[0])
      NavModelRun->Visible = true;
    else
      NavModelRun->Visible = false;
  }

  BottomL->Left = Width - BottomL->Width - 10;
  BottomL->Top = NavShape->Top + NavShape->Height + (Height - NavShape->Top - NavShape->Height - BottomL->Height - FrameShape->Pen->Width)/2;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
  FormResize(this);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ModelsPanelResize(TObject *Sender)
{
  ModelsTree->Top = 0;
  ModelsTree->Left = 0;
  ModelsTree->Height = ModelsPanel->Height;
  ModelsTree->Width = ModelsPanel->Width / 4;

  PicView->Top = ModelsTree->Top;
  PicView->Left = ModelsTree->Left + ModelsTree->Width + 20;
  PicView->Height = ModelsPanel->Height / 3;

  VertDivider->Top = ModelsTree->Top + 10;
  VertDivider->Left = ModelsTree->Left + ModelsTree->Width + (20 - VertDivider->Width)/2;
  VertDivider->Height = ModelsTree->Height - 20;

  ModelsRichEdit->Top = PicView->Top + PicView->Height + 20;
  ModelsRichEdit->Left = PicView->Left;
  ModelsRichEdit->Width = ModelsPanel->Width - ModelsTree->Left - ModelsTree->Width - 20;
  ModelsRichEdit->Height = ModelsPanel->Height - ModelsRichEdit->Top;

  HorsDivider->Left = VertDivider->Left - 5;
  HorsDivider->Top = PicView->Top + PicView->Height + (20 - HorsDivider->Height)/2;
  HorsDivider->Width = ModelsRichEdit->Left + ModelsRichEdit->Width - HorsDivider->Left - 10;

  PicView->Width = ModelsRichEdit->Width / 2;

  ModelL->Left = PicView->Left + PicView->Width + 10;
  ModelL->Top = PicView->Top;
  ModelL->Width = ModelsPanel->Width - ModelL->Left;
  ModelL->Height = PicView->Height;

  if (FPicViewer)
    FPicViewer->UpdateSize();

  ModelsL->Left = PicView->Left;
  ModelsL->Top = HorsDivider->Top - ModelsL->Height - 10;
  ModelsL->Width = ModelsPanel->Width - ModelsL->Left;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::RunClick(System::TObject* Sender)
{
  AnsiString comline = FCurrentPath + "data\\rdodemo.exe" + " " + FCurModelPath + "demo\\" + ModelsTree->Selected->Text + ".dmf";
  AnsiString curdir = FCurModelPath + "demo";

  STARTUPINFO start;
  ZeroMemory(&start, sizeof(STARTUPINFO));
  start.cb = sizeof(STARTUPINFO);
  start.dwFlags = STARTF_USESHOWWINDOW;
  start.wShowWindow = SW_SHOWMAXIMIZED;

  PROCESS_INFORMATION pi;

  if (CreateProcess(NULL, comline.c_str(), NULL, NULL, true, NORMAL_PRIORITY_CLASS, NULL, curdir.c_str(), &start, &pi))
    Processes->Add((void*)pi.hProcess);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ViewSourceClick(System::TObject* Sender)
{
  try {
    LockWindowUpdate(Handle);
    ViewSourcePanel->Visible = false;
    SourcePanel->Visible = true;
    SourceClick(NavModelPAT);
    LockWindowUpdate(NULL);
  } catch (Exception &e) {
    LockWindowUpdate(NULL);
  }
}

//---------------------------------------------------------------------------
void TForm1::CreateSourceLabel(THighlightText* &Label, const AnsiString Caption)
{
  Label = new THighlightText(SourcePanel);
  Label->Caption = Caption;
  Label->Font->Name = NavModelSource->Font->Name;
  Label->Font->Size = 8;
  Label->NormStyle << fsBold;
  Label->NormColor = NavModelSource->NormColor;
  Label->HighlCursor = crHandPoint;
  Label->HighlStyle << fsUnderline << fsBold;
  Label->HighlColor = NavModelSource->HighlColor;
  Label->Color = SourcePanel->Brush->Color;
  Label->Parent = SourcePanel;
  Label->OnClick = SourceClick;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::SourceClick(System::TObject* Sender)
{
  if (dynamic_cast<THighlightText*>(Sender)) {
    for (int i = 0; i < SourcePanel->ControlCount; i++)
      if (dynamic_cast<THighlightText*>(SourcePanel->Controls[i]))
        ((THighlightText*)SourcePanel->Controls[i])->StayHighlighted = false;

    THighlightText* control = (THighlightText*)Sender;
    control->StayHighlighted = true;

    ModelsRichEdit->WordWrap = false;
    ModelsRichEdit->Font->Name = "Courier New";
    ModelsRichEdit->PlainText = true;

    AnsiString filename = FCurModelPath + "source\\" + ModelsTree->Selected->Text + "." + control->Caption;
    if (FileExists(filename))
      ModelsRichEdit->Lines->LoadFromFile(filename);
    else
      ModelsRichEdit->Lines->Clear();
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::StopShowClick(System::TObject* Sender)
{
  StopRAOs();
  Timer->Enabled = false;
  NavStopShow->Visible = false;
  bShowRunned = false;

  switch (SelectedMenu) {
    case mkExplore :
    case mkModelsShow : {
      int selmod = ModelsTree->Selected->Index;
      SelectedMenu = mkNone;
      MenuClick(MenuExplore);
      ModelsTree->Selected = ModelsTree->Items->Item[0]->Item[selmod];
      break;
    }

    case mkPresentation: {
      SelectedMenu = mkNone;
      MenuClick(MenuPresentation);
      break;
    }
  }
}

//---------------------------------------------------------------------------
void TForm1::StopRAOs()
{
  if (Processes) {
    int count = Processes->Count;
    for (int i = 0; i < count; i++) {
      DWORD code;
      GetExitCodeProcess((HANDLE)Processes->Items[i], &code);
      if (code == STILL_ACTIVE)
          TerminateProcess((HANDLE)Processes->Items[i], code);
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::TimerTimer(TObject *Sender)
{
  Timer->Enabled = false;
  StopRAOs();
  FCurModel++;
  if (FCurModel == ModelsTree->Items->Item[0]->Count)
    FCurModel = 0;
  ModelsTree->Selected = ModelsTree->Items->Item[0]->Item[FCurModel];
  Timer->Interval = FCurModelDelay * 1000;
  RunClick(NULL);
  Timer->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::EMailClick(System::TObject* Sender)
{
  AnsiString email = "mailto:" + EMail->Caption;
  ShellExecute(Application->Handle, "open", email.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::WebClick(System::TObject* Sender)
{
  ShellExecute(Application->Handle, "open", Web->Caption.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::WndProc(Messages::TMessage &Message)
{
  TForm::WndProc(Message);
  switch (Message.Msg) {
    case WM_DISPLAYCHANGE : {
      FormResize(this);
      break;
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ForwardClick(System::TObject* Sender)
{
  ViewPage(FCurPage + 1);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::BackwardClick(System::TObject* Sender)
{
  ViewPage(FCurPage - 1);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::BeginClick(System::TObject* Sender)
{
  ViewPage(0);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::EndClick(System::TObject* Sender)
{
  ViewPage(FPageCount - 1);
}

//---------------------------------------------------------------------------
void TForm1::UpdatePresentButtons()
{
  if (!bPresentationRunned) {
    GoEnd->Visible = (FCurPage != FPageCount - 1);
    PageForward->Visible = (FCurPage != FPageCount - 1);
    PageBackward->Visible = (FCurPage != 0);
    GoBegin->Visible = (FCurPage != 0);
  }
  CurPageL->Caption = Format(GetString(sPageOf), OPENARRAY(TVarRec, (FCurPage + 1, FPageCount)));
}

//---------------------------------------------------------------------------
void TForm1::ViewPage(const int Number)
{
  if (bPresentationRunned)
    PresentTimer->Enabled = false;
  FCurPage = Number;
  UpdatePresentButtons();
  TIniFile* ini = NULL;
  AnsiString file = "";
  if (!FUsePagesDelays)
    FCurPageDelay = FPagesDelay;

  try {
    ini = new TIniFile(FPresentPath + "presentation.ini");
    AnsiString sec = "Page_" + IntToStr(FCurPage + 1);
    file = ini->ReadString(sec, "FileName", "");
    if (FUsePagesDelays)
      FCurPageDelay = ini->ReadInteger(sec, "Delay", 0);
    delete ini; ini = NULL;
  } catch (Exception& e) {
    if (ini) delete ini;
  }

  AnsiString path = FPresentPath + file;
  if (FileExists(path))
    PresentRichEdit->Lines->LoadFromFile(path);
  else
    PresentRichEdit->Lines->Clear();
  PresentTimer->Interval = FCurPageDelay * 1000;

  if (bPresentationRunned) {
    PresentTimer->Enabled = true;
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::PresentNavResize(TObject *Sender)
{
  int space = (PresentNav->Width - (PageForward->Width + PageBackward->Width + RunStop->Width + GoEnd->Width + GoBegin->Width + CurPageL->Width))/5;
  GoBegin->Left = 0;
  PageBackward->Left = GoBegin->Left + GoBegin->Width + space;
  RunStop->Left = PageBackward->Left + PageBackward->Width + space;
  PageForward->Left = RunStop->Left + RunStop->Width + space;
  GoEnd->Left = PageForward->Left + PageForward->Width + space;
  CurPageL->Left = GoEnd->Left + GoEnd->Width + space;
  GoBegin->Top = (PresentNav->Height - GoBegin->Height)/2;
  PageBackward->Top = GoBegin->Top;
  RunStop->Top = GoBegin->Top;
  PageForward->Top = GoBegin->Top;
  GoEnd->Top = GoBegin->Top;
  CurPageL->Top = (PresentNav->Height - CurPageL->Height)/2;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ModelsTreeDblClick(TObject *Sender)
{
  if (ModelsTree->Selected != ModelsTree->Items->Item[0] && !bShowRunned)
    RunClick(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ViewSourcePanelResize(TObject *Sender)
{
  if (NavModelSource) {
    NavModelSource->Left = (ViewSourcePanel->Width - NavModelSource->Width)/2;
    NavModelSource->Top  = (ViewSourcePanel->Height - NavModelSource->Height)/2;
  }

  if (SelectedMenu == mkExplore && ModelsTree->Selected != ModelsTree->Items->Item[0])
    ViewSourcePanel->Visible = true;
  else
    ViewSourcePanel->Visible = false;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::SourcePanelResize(TObject *Sender)
{
  int space = (SourcePanel->Width - 2 * SourcePanel->BevelWidth - NavModelPAT->Width - NavModelRTP->Width - NavModelRSS->Width
               - NavModelOPR->Width - NavModelFRM->Width - NavModelFUN->Width - NavModelDPT->Width
               - NavModelSMR->Width - NavModelPMD->Width - NavModelPMW->Width
               - NavModelTRC->Width)/10;

  if (NavModelPAT) {
    NavModelPAT->Top = (SourcePanel->Height - NavModelPAT->Height)/2;
    NavModelPAT->Left = 0;
  }

  if (NavModelRTP) {
    NavModelRTP->Top = (SourcePanel->Height - NavModelRTP->Height)/2;
    NavModelRTP->Left = NavModelPAT->Left + NavModelPAT->Width + space;
  }

  if (NavModelRSS) {
    NavModelRSS->Top = (SourcePanel->Height - NavModelRSS->Height)/2;
    NavModelRSS->Left = NavModelRTP->Left + NavModelRTP->Width + space;
  }

  if (NavModelOPR) {
    NavModelOPR->Top = (SourcePanel->Height - NavModelOPR->Height)/2;
    NavModelOPR->Left = NavModelRSS->Left + NavModelRSS->Width + space;
  }

  if (NavModelFRM) {
    NavModelFRM->Top = (SourcePanel->Height - NavModelFRM->Height)/2;
    NavModelFRM->Left = NavModelOPR->Left + NavModelOPR->Width + space;
  }

  if (NavModelFUN) {
    NavModelFUN->Top = (SourcePanel->Height - NavModelFUN->Height)/2;
    NavModelFUN->Left = NavModelFRM->Left + NavModelFRM->Width + space;
  }

  if (NavModelDPT) {
    NavModelDPT->Top = (SourcePanel->Height - NavModelDPT->Height)/2;
    NavModelDPT->Left = NavModelFUN->Left + NavModelFUN->Width + space;
  }

  if (NavModelSMR) {
    NavModelSMR->Top = (SourcePanel->Height - NavModelSMR->Height)/2;
    NavModelSMR->Left = NavModelDPT->Left + NavModelDPT->Width + space;
  }

  if (NavModelPMD) {
    NavModelPMD->Top = (SourcePanel->Height - NavModelPMD->Height)/2;
    NavModelPMD->Left = NavModelSMR->Left + NavModelSMR->Width + space;
  }

  if (NavModelPMW) {
    NavModelPMW->Top = (SourcePanel->Height - NavModelPMW->Height)/2;
    NavModelPMW->Left = NavModelPMD->Left + NavModelPMD->Width + space;
  }

  if (NavModelTRC) {
    NavModelTRC->Top = (SourcePanel->Height - NavModelTRC->Height)/2;
    NavModelTRC->Left = NavModelPMW->Left + NavModelPMW->Width + space;
  }
}

//---------------------------------------------------------------------------
void TForm1::CreateHotImage(THighlightImage* &Image, TWinControl* AParent, const int NormIndex, const int HighlIndex, const AnsiString AHint)
{
  Image = new THighlightImage(AParent);
  Image->Images = Images;
  Image->HotImages = HotImages;
  Image->NormCursor = crDefault;
  Image->HighlCursor = crHandPoint;
  Image->NormImageIndex = NormIndex;
  Image->HighlImageIndex = HighlIndex;
  Image->Height = Images->Height;
  Image->Width = Images->Width;
  Image->ParentShowHint = false;
  Image->ShowHint = true;
  Image->Hint = AHint;
  Image->Parent = AParent;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::RunPresentClick(System::TObject* Sender)
{
  if (FPageCount > 1) {
    GoBegin->Visible = false;
    PageBackward->Visible = false;
    PageForward->Visible = false;
    GoEnd->Visible = false;

    bPresentationRunned = true;
    PresentTimer->Enabled = true;

    RunStop->NormImageIndex++;
    RunStop->HighlImageIndex++;
    RunStop->OnClick = StopPresentClick;
    RunStop->Hint = GetString(sStopPresent);
    RunStop->UpdateImage(true);
  }
}

void __fastcall TForm1::StopPresentClick(System::TObject* Sender)
{
  PresentTimer->Enabled = false;
  bPresentationRunned = false;
  int selmod = -1;
  if (ModelsTree->Selected != ModelsTree->Items->Item[0])
    selmod = ModelsTree->Selected->Index;
  switch (SelectedMenu) {

    case mkAbout : {
      SelectedMenu = mkNone;
      MenuClick(MenuAbout);
      break;
    }

    case mkExplore : {
      SelectedMenu = mkNone;
      MenuClick(MenuExplore);
      if (selmod != -1)
        ModelsTree->Selected = ModelsTree->Items->Item[0]->Item[selmod];
      break;
    }

    case mkModelsShow : {
      SelectedMenu = mkNone;
      MenuClick(MenuShow);
      if (selmod != -1)
        ModelsTree->Selected = ModelsTree->Items->Item[0]->Item[selmod];
      break;
    }

    case mkPresentation: {
      UpdatePresentButtons();
      break;
    }
  }

  RunStop->NormImageIndex--;
  RunStop->HighlImageIndex--;
  RunStop->OnClick = RunPresentClick;
  RunStop->Hint = GetString(sRunPresent);
  RunStop->UpdateImage(true);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::PresentTimerTimer(TObject *Sender)
{
  if (FCurPage == FPageCount - 1)
    FCurPage = -1;
  ViewPage(FCurPage + 1);
}

//---------------------------------------------------------------------------
void TForm1::ShowOptions()
{
  TOptionsForm* opt = NULL;
  try {
    opt = new TOptionsForm();
    opt->ShowModal();
    delete opt; opt = NULL;
  } catch (Exception &e) {
    if (opt) delete opt;
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::OptionsClick(System::TObject* Sender)
{
  if (bPresentationRunned) {
    ShowTypedMessage(GetString(sStopPresOpt), MB_ICONWARNING);
    StopPresentClick(RunStop);
  }

  if (bShowRunned) {
    ShowTypedMessage(GetString(sStopRevOpt), MB_ICONWARNING);
    StopShowClick(NavStopShow);
  }

  ShowOptions();
}

//---------------------------------------------------------------------------
void TForm1::HideAll()
{
  BriefPanel->Visible = false;

  if (!bPresentationRunned) {
    PresentNav->Visible = false;
    PresentPanel->Visible = false;
  }

  ModelsPanel->Visible = false;
  SourcePanel->Visible = false;

  if (NavModelRun)
    NavModelRun->Visible = false;

  ViewSourcePanel->Visible = false;

  if (NavStopShow && !bShowRunned)
    NavStopShow->Visible = false;
}

//---------------------------------------------------------------------------
void TForm1::LoadSettings()
{
  TIniFile* ini = NULL;
  try {
    ini = new TIniFile(FCurrentPath + "RAO_Explorer.ini");
    Language = ini->ReadString("Options", "Language", "English");
    ShowOptionsMenu = ini->ReadBool("Options", "ShowOptionsMenu", true);
    ModelsDelay = ini->ReadInteger("Options", "ModelsDelay", 30);
    UseModelsDelays = ini->ReadBool("Options", "UseModelsDelays", true);
    PagesDelay = ini->ReadInteger("Options", "PagesDelay", 30);
    UsePagesDelays = ini->ReadBool("Options", "UsePagesDelays", true);
    AutoRunPresentation = ini->ReadBool("Options", "AutoRunPresentation", false);
    delete ini; ini = NULL;
  } catch (Exception& e) {
    if (ini) delete ini;
    throw;
  }
}

//---------------------------------------------------------------------------
void TForm1::SaveSettings()
{
  bool oncd = true;
  AnsiString path = FCurrentPath + "RAO_Explorer.ini";
  if (FileExists(path)) {
    int attr = FileGetAttr(path);
    if ((!(attr & faReadOnly)) || ((attr & faReadOnly) && (FileSetAttr(path, attr ^ faReadOnly) == 0)))
      oncd = false;
  } else {
    oncd = false;
  }
  if (oncd) return;

  TIniFile* ini = NULL;
  try {
    ini = new TIniFile(path);
    ini->WriteString("Options", "Language", FLanguage);
    ini->WriteBool("Options", "ShowOptionsMenu", FShowOptionsMenu);
    ini->WriteInteger("Options", "ModelsDelay", FModelsDelay);
    ini->WriteBool("Options", "UseModelsDelays", FUseModelsDelays);
    ini->WriteInteger("Options", "PagesDelay", FPagesDelay);
    ini->WriteBool("Options", "UsePagesDelays", FUsePagesDelays);
    ini->WriteBool("Options", "AutoRunPresentation", FAutoRunPresentation);
    delete ini; ini = NULL;
  } catch (Exception& e) {
    if (ini) delete ini;
  }
}

//---------------------------------------------------------------------------
void TForm1::SetShowOptionsMenu(const bool Value)
{

  if (FShowOptionsMenu != Value) {
    FShowOptionsMenu = Value;
    if (FShowOptionsMenu) {
      OptionsImage->Visible = true;
      MenuOptions->Visible = true;
    } else {
      if (!FStartUp)
        ShowTypedMessage(GetString(sShowOptWarn) + "\r\n\r\n[Options]\r\nShowOptionsMenu=1\r\n\r\n" + GetString(sShowOptWarn2), MB_ICONINFORMATION);
      OptionsImage->Visible = false;
      MenuOptions->Visible = false;
    }
  }
}

//---------------------------------------------------------------------------
void TForm1::SetUseModelsDelays(const bool Value)
{
  if (FUseModelsDelays != Value) {
    FUseModelsDelays = Value;
  }
}

//---------------------------------------------------------------------------
void TForm1::SetUsePagesDelays(const bool Value)
{
  if (FUsePagesDelays != Value) {
    FUsePagesDelays = Value;
  }
}

//---------------------------------------------------------------------------
void TForm1::SetModelsDelay(const int Value)
{
  if (FModelsDelay != Value) {
    FModelsDelay = Value;
  }
}

//---------------------------------------------------------------------------
void TForm1::SetPagesDelay(const int Value)
{
  if (FPagesDelay != Value) {
    FPagesDelay = Value;
  }
}

//---------------------------------------------------------------------------
void TForm1::SetAutoRunPresentation(const bool Value)
{
  if (FAutoRunPresentation != Value) {
    FAutoRunPresentation = Value;
  }
}

//---------------------------------------------------------------------------
void TForm1::SetLanguage(const AnsiString Value)
{
  if (FLanguage != Value) {
    FLanguage = Value;
    TIniFile* lng = NULL;
    AnsiString folder = "";

    try {
      lng = new TIniFile(FCurrentPath + "data\\RAO_Explorer.lng");
      folder = lng->ReadString(FLanguage, "Folder", "");
      delete lng; lng = NULL;
    } catch (Exception& e) {
      if (lng) delete lng;
      throw;
    }

    FLangPath = GetNormalDirFromDir(FCurrentPath + "data\\" + folder);
    FBriefPath = GetNormalDirFromDir(FLangPath + "brief");
    FModelsPath = GetNormalDirFromDir(FLangPath + "models");
    FPresentPath = GetNormalDirFromDir(FLangPath + "presentation");
    LockWindowUpdate(Handle);

    try {
      SetCaptions();
      bool transp;
      AnsiString path = GetFileName(nkRAO, transp);
      LoadTransparentBitmap(RAOImage, path, transp);
      FormResize(this);
      BriefPanelResize(BriefPanel);
      ModelsPanelResize(ModelsPanel);
      PresentNavResize(PresentNav);
      ViewSourcePanelResize(ViewSourcePanel);
      SourcePanelResize(SourcePanel);
      SelectedMenu = mkNone;
      MenuClick(MenuAbout);
      LockWindowUpdate(NULL);
    } catch (Exception& e) {
      LockWindowUpdate(NULL);
      throw;
    }
  }
}

//---------------------------------------------------------------------------
void TForm1::SetCaptions()
{
  CapLab->Caption = GetString(sCD);
  ResL->Caption = GetString(sResources);
  ActL->Caption = GetString(sActions);
  OpL->Caption = GetString(sOperations);

  AnsiString str = GetString(sBMSTU);
  AnsiString str2 = GetString(sBMSTU2);
  if (str2.data())
    BMSTUL->Caption = str + "\r\n" + str2;
  else
    BMSTUL->Caption = str;

  DeptL->Caption = GetString(sDept) + "\r\n" + GetString(sDept2);
  TelL->Caption = GetString(sMoscow);
  BottomL->Caption = GetString(sBMSTUBottom);

  ModelsTree->Items->Item[0]->Text = GetString(sModels);

  MenuAbout->Caption = GetString(sProductInfo);
  MenuExplore->Caption = GetString(sExploreModels);
  MenuShow->Caption = GetString(sReviewModels);
  MenuPresentation->Caption = GetString(sMoreInfo);
  MenuExit->Caption = GetString(sExit);
  MenuOptions->Caption = GetString(sOptions);

  ModelsL->Caption = GetString(sModSum);

  NavModelRun->Caption = GetString(sRun);
  NavModelSource->Caption = GetString(sViewSource);
  NavStopShow->Caption = GetString(sStopShow);

  EMail->Caption       = GetString(sEMail);
  Web->Caption         = GetString(sWeb);

  GoBegin->Hint = GetString(sFirstPage);
  PageForward->Hint = GetString(sForward);
  PageBackward->Hint = GetString(sBackward);
  GoEnd->Hint = GetString(sLastPage);
  RunStop->Hint = GetString(sRunPresent);
  CurPageL->Caption = Format(GetString(sPageOf), OPENARRAY(TVarRec, (200, 200)));

  FPicViewer->UpdateStrings();
  FMainPicViewer->UpdateStrings();
}

//---------------------------------------------------------------------------
AnsiString TForm1::GetFileName(TFileNameKind File, bool &Transparent)
{
  TIniFile* ini = NULL;
  AnsiString res = "";

  try {
    ini = new TIniFile(FLangPath + "images.ini");
    switch (File) {
      case nkBrief : {
        res = ini->ReadString("Images", "Brief", "");
        Transparent = ini->ReadBool("Images", "TranspBrief", true);
        break;
      }
      case nkRAO   : {
        res = ini->ReadString("Images", "RAO", "");
        Transparent = ini->ReadBool("Images", "TranspRAO", true);
        break;
      }
    }
    delete ini; ini = NULL;
  } catch (Exception& e) {
    if (ini) delete ini;
    throw;
  }

  return FImagesPath + res;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::AppException(TObject *Sender, Exception *E)
{
  ShowTypedMessage(E->Message, MB_ICONSTOP);
}

void __fastcall TForm1::ModelsTreeCustomDrawItem(TCustomTreeView *Sender,
      TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw)
{
  if (State.Contains(cdsSelected))
  {
    TFontStyles fs;
    fs.Clear();
    fs << fsBold;
    ModelsTree->Canvas->Font->Style = fs;
    ModelsTree->Canvas->Brush->Color = (TColor)0x00815F1C;
  }
  DefaultDraw = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ModelsTreeKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN && !bShowRunned) {
    if (ModelsTree->Selected != ModelsTree->Items->Item[0])
      RunClick(NULL);
    Key = 0;
  }
}
//---------------------------------------------------------------------------


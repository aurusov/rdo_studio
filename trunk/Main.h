//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>

#include "HighlightText.h"
#include "Dr_PictureViewer.h"
#include "HighlightImage.h"
#include <ImgList.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:
  TShape *MenuShape;
  TShape *NavShape;
  TShape *CapShape;
  TShape *FrameShape;
  TLabel *CapLab;
  TImage *RAOImage;
  TLabel *ResL;
  TLabel *ActL;
  TLabel *OpL;
  TImageList *Images;
  TImageList *HotImages;
  TImage *AboutImage;
  TImage *ExploreImage;
  TImage *ShowImage;
  TImage *PresentImage;
  TImage *ExitImage;
  TLabel *MenuL;
  TPanel *BriefPanel;
  TRichEdit *RichEdit;
  TPanel *ModelsPanel;
  TRichEdit *ModelsRichEdit;
  TTreeView *ModelsTree;
  TPanel *PicView;
  TShape *VertDivider;
  TShape *HorsDivider;
  TLabel *ModelL;
  TLabel *ModelsL;
  TTimer *Timer;
  TLabel *BottomL;
  TLabel *BMSTUL;
  TLabel *DeptL;
  TLabel *TelL;
  TLabel *EmailL;
  TPanel *PresentPanel;
  TRichEdit *PresentRichEdit;
  TPanel *PresentNav;
  TLabel *CurPageL;
  TPanel *ViewSourcePanel;
  TPanel *SourcePanel;
  TPanel *MainPicView;
  TImageList *TreeImages;
  TTimer *PresentTimer;
  TImage *OptionsImage;
  TLabel *WebL;
  TImageList *Transp;
  void __fastcall ModelsTreeChange(TObject *Sender, TTreeNode *Node);
  void __fastcall BriefPanelResize(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ModelsPanelResize(TObject *Sender);
  void __fastcall TimerTimer(TObject *Sender);
  void __fastcall PresentNavResize(TObject *Sender);
  void __fastcall ModelsTreeDblClick(TObject *Sender);
  void __fastcall ViewSourcePanelResize(TObject *Sender);
  void __fastcall SourcePanelResize(TObject *Sender);
  void __fastcall PresentTimerTimer(TObject *Sender);
  void __fastcall ModelsTreeCustomDrawItem(TCustomTreeView *Sender,
    TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall ModelsTreeKeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift);
private:
typedef enum {
  mkNone         = 0,
  mkAbout        = 1,
  mkExplore      = 2,
  mkModelsShow   = 3,
  mkPresentation = 4
} TMenuItemKind;

typedef enum {
  nkNone         = 0,
  nkBrief        = 1,
  nkRAO          = 2
} TFileNameKind;

  THighlightText* MenuExplore;
  THighlightText* MenuShow;
  THighlightText* MenuAbout;
  THighlightText* MenuPresentation;
  THighlightText* MenuExit;
  THighlightText* MenuOptions;
  void __fastcall UpdateMenuItem(System::TObject* Sender, const bool HighLighted);
  void __fastcall MenuClick(System::TObject* Sender);
  void __fastcall OptionsClick(System::TObject* Sender);
  void SetMenuItemPos(THighlightText* &Item);
  void HideAll();

  AnsiString FCurrentPath;
  AnsiString FImagesPath;
  AnsiString FLangPath;
  AnsiString FBriefPath;
  AnsiString FModelsPath;
  AnsiString FPresentPath;

  void CreateMenuItem(THighlightText* &Item, TImage* &Image, const int ImageIndex, const AnsiString Caption);
  int FCurMenuItemPos;

  void LoadModels();
  AnsiString FCurModelPath;
  int FCurModelDelay;

  TPictureViewer* FPicViewer;
  TPictureViewer* FMainPicViewer;

  THighlightText* NavModelRun;
  void __fastcall RunClick(System::TObject* Sender);

  THighlightText* NavModelSource;
  void __fastcall ViewSourceClick(System::TObject* Sender);

  THighlightText* NavModelPAT;
  THighlightText* NavModelRTP;
  THighlightText* NavModelRSS;
  THighlightText* NavModelOPR;
  THighlightText* NavModelFRM;
  THighlightText* NavModelFUN;
  THighlightText* NavModelDPT;
  THighlightText* NavModelSMR;
  THighlightText* NavModelPMD;
  THighlightText* NavModelPMW;
  THighlightText* NavModelTRC;
  void CreateSourceLabel(THighlightText* &Label, const AnsiString Caption);
  void __fastcall SourceClick(System::TObject* Sender);

  THighlightText* NavStopShow;
  void __fastcall StopShowClick(System::TObject* Sender);
  void StopRAOs();

  HANDLE FRAOProcess;
  TList* Processes;

  TMenuItemKind SelectedMenu;
  bool bShowRunned;
  int FCurModel;

  THighlightText* EMail;
  void __fastcall EMailClick(System::TObject* Sender);
  THighlightText* Web;
  void __fastcall WebClick(System::TObject* Sender);

  int FPageCount;
  int FCurPage;
  THighlightImage* PageForward;
  THighlightImage* PageBackward;
  THighlightImage* GoEnd;
  THighlightImage* GoBegin;
  THighlightImage* RunStop;
  void CreateHotImage(THighlightImage* &Image, TWinControl* AParent, const int NormIndex, const int HighlIndex, const AnsiString AHint = "");
  void __fastcall ForwardClick(System::TObject* Sender);
  void __fastcall BackwardClick(System::TObject* Sender);
  void __fastcall BeginClick(System::TObject* Sender);
  void __fastcall EndClick(System::TObject* Sender);
  void __fastcall RunPresentClick(System::TObject* Sender);
  void __fastcall StopPresentClick(System::TObject* Sender);
  void UpdatePresentButtons();
  void ViewPage(const int Number);
  bool bPresentationRunned;
  int FCurPageDelay;

  void ShowOptions();

  bool FStartUp;
  AnsiString FLanguage;
  void SetLanguage(const AnsiString Value);
  bool FShowOptionsMenu;
  void SetShowOptionsMenu(const bool Value);
  bool FUseModelsDelays;
  void SetUseModelsDelays(const bool Value);
  int FModelsDelay;
  void SetModelsDelay(const int Value);
  bool FUsePagesDelays;
  void SetUsePagesDelays(const bool Value);
  int FPagesDelay;
  void SetPagesDelay(const int Value);
  bool FAutoRunPresentation;
  void SetAutoRunPresentation(const bool Value);
  void LoadSettings();
  void SaveSettings();
  void SetCaptions();
  AnsiString GetFileName(TFileNameKind File, bool &Transparent);

  void __fastcall AppException(TObject *Sender, Exception *E);
protected:
  void __fastcall WndProc(Messages::TMessage &Message);
public:
  __fastcall TForm1(TComponent* Owner);
  __fastcall ~TForm1();
  __property AnsiString Language = { read=FLanguage, write=SetLanguage };
  __property bool ShowOptionsMenu = { read=FShowOptionsMenu, write=SetShowOptionsMenu };
  __property bool UseModelsDelays = { read=FUseModelsDelays, write=SetUseModelsDelays };
  __property int ModelsDelay = { read=FModelsDelay, write=SetModelsDelay };
  __property bool UsePagesDelays = { read=FUsePagesDelays, write=SetUsePagesDelays};
  __property int PagesDelay = { read=FPagesDelay, write=SetPagesDelay };
  __property bool AutoRunPresentation = { read=FAutoRunPresentation, write=SetAutoRunPresentation };
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

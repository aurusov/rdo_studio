//---------------------------------------------------------------------------
#ifndef Pl_Form_ReportPreviewH
#define Pl_Form_ReportPreviewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>

#include "Pl_Base_Report.h"
//---------------------------------------------------------------------------
typedef enum {
  st_ZoomFit          = 0,
  st_ZoomFitPageWidth = 1,
  st_NoZoom           = 2,
  st_Custom           = 3
} TScaleType;

class TPreviewForm : public TForm
{
friend class TPreviewOptionsForm;
__published:
  TToolBar *ToolBar;
  TSpeedButton *NoZoomB;
  TSpeedButton *ZoomPageWidthB;
  TSpeedButton *ZoomFitB;
  TToolButton *ToolButton1;
  TSpeedButton *PrintB;
  TSpeedButton *PrintSetupB;
  TScrollBox *ScrollBox;
  TShape *Shadow;
  TImage *ReportImage;
  TStatusBar *StatusBar;
  TShape *Border;
  TBevel *PreviewBevel;
  TToolButton *ToolButton2;
  TSpeedButton *OptionsB;
  TToolButton *ToolButton3;
  TSpeedButton *PrevPageB;
  TSpeedButton *NextPageB;
  TComboBox *ScaleBox;
  TToolButton *ToolButton4;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall PrintBClick(TObject *Sender);
  void __fastcall PrintSetupBClick(TObject *Sender);
  void __fastcall OptionsBClick(TObject *Sender);
  void __fastcall PrevPageBClick(TObject *Sender);
  void __fastcall NextPageBClick(TObject *Sender);
  void __fastcall ScaleBoxChange(TObject *Sender);
  void __fastcall ScaleBoxKeyPress(TObject *Sender, char &Key);
  void __fastcall ZoomFitBClick(TObject *Sender);
  void __fastcall ZoomPageWidthBClick(TObject *Sender);
  void __fastcall NoZoomBClick(TObject *Sender);
  void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
    int WheelDelta, TPoint &MousePos, bool &Handled);
private:
  int FPage;
  int NewWidth;
  int NewHeight;
  int ClHeight;
  int ClWidth;
  double ScaleKoef;
  int LastScaleValue;
  TScaleType ScaleType;
  int LastItemIndex;
  TScaleType LastScaleType;
  void SetPage(int PageNumber);
  void doZoomFit();
  void doZoomPageWidth();
  void doSetEnvironement();
  void SetPageButtons();
  void doSetStatus();
protected:
  void __fastcall WndProc(Messages::TMessage &Message);
public:
  bool CanResize;
  TPlBaseReport* Report;
  void ResizeReport();
  void SetStatusButtons();
  __fastcall TPreviewForm();
  __fastcall ~TPreviewForm();
  __property int Page = { read=FPage, write=SetPage };
};
//---------------------------------------------------------------------------
#endif

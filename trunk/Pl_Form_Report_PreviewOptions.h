//---------------------------------------------------------------------------
#ifndef Pl_Form_Report_PreviewOptionsH
#define Pl_Form_Report_PreviewOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include "Dr_Form_Hint.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "Pl_Base_Report.h"
#include "Pl_Form_ReportPreview.h"
#include "DrButton.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TPreviewOptionsForm : public THintForm
{
__published:	// IDE-managed Components
  TPageControl *Pages;
  TTabSheet *PreviewSheet;
  TLabel *ShadowWidthL;
  TLabel *RepBordWidthL;
  TLabel *RepCantWidthL;
  TLabel *ShadowColorL;
  TLabel *CantColorL;
  TShape *ShadExample;
  TShape *CantExample;
  TEdit *ShadowWidthEd;
  TEdit *RepBordWidthEd;
  TEdit *RepCantWidthEd;
  TCheckBox *RoundFonts;
  TCSpinButton *CSpinButton6;
  TCSpinButton *CSpinButton7;
  TCSpinButton *CSpinButton5;
  TCheckBox *BlackWhiteBox;
  TTabSheet *ReportSheet;
  TGroupBox *Margins;
  TLabel *LeftOffsetL;
  TLabel *OffsetTopL;
  TLabel *OffsetBotL;
  TLabel *OffsetRightL;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TEdit *OffsetLeftEd;
  TEdit *OffsetTopEd;
  TEdit *OffsetBotEd;
  TEdit *OffsetRightEd;
  TCSpinButton *CSpinButton1;
  TCSpinButton *CSpinButton2;
  TCSpinButton *CSpinButton3;
  TCSpinButton *CSpinButton4;
  TCheckBox *PrintFooterBox;
  TCheckBox *PrintHeaderBox;
  TCheckBox *PageNumbersBox;
  TCheckBox *PrintDateBox;
  TDrButton *OkB;
  TDrButton *CancelB;
  TShape *ShadowWidthShape;
  TShape *RepBordShape;
  TShape *RepCantShape;
  TShape *OffsetTopShape;
  TShape *OffsetBottomShape;
  TShape *OffsetLeftShape;
  TShape *OffsetRightShape;
  void __fastcall ShadExampleMouseDown(TObject *Sender,
    TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall ShadowWidthEdKeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift);
  void __fastcall CSpinButton5DownClick(TObject *Sender);
  void __fastcall CSpinButton5UpClick(TObject *Sender);
  void __fastcall CSpinButton1DownClick(TObject *Sender);
  void __fastcall CSpinButton1UpClick(TObject *Sender);
  void __fastcall OffsetTopEdKeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift);
  void __fastcall PrintFooterBoxClick(TObject *Sender);
  void __fastcall PageNumbersBoxClick(TObject *Sender);
  void __fastcall PagesChange(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
private:
  TPlBaseReport* Report;
  TPreviewForm* Preview;
  bool CanUpdate;
  void GetOptions();
  void SetOptions();
  bool CheckOffsets();
  void ChangeValue(TEdit *Edit, double ToAdd);
  void SetHeaderFooterEnabled();
public:
  __fastcall TPreviewOptionsForm(TComponent* Owner);
  __fastcall TPreviewOptionsForm(TPlBaseReport* Rpt, TPreviewForm* Prv);
  __fastcall ~TPreviewOptionsForm();
};
//---------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------

#ifndef Form_OptionsH
#define Form_OptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "HighlightText.h"
#include "Pl_CheckBox.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TOptionsForm : public TForm
{
__published:
  TLabel *Title;
  TShape *TopShape;
  TShape *BottomShape;
  TShape *BigShape;
  TListBox *LangBox;
  TShape *Shape1;
  TLabel *LangLab;
  TLabel *ModShowLab;
  TLabel *PresentLab;
  TShape *Shape2;
  TEdit *ModelsDelay;
  TShape *ModelsShape;
  TLabel *ModDelL;
  TLabel *ModDelSL;
  TShape *PresentShape;
  TEdit *PresentDelay;
  TLabel *PresDelSL;
  TLabel *PresDelL;
  void __fastcall LangBoxDrawItem(TWinControl *Control, int Index,
    TRect &Rect, TOwnerDrawState State);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:
  HRGN rgn;

  THighlightText* OkB;
  void __fastcall OkBClick(TObject *Sender);

  THighlightText* CancelB;
  void __fastcall CancelBClick(TObject *Sender);

  bool FMouseDown;
  int dX, dY;
  void __fastcall WndProc(Messages::TMessage &Message);

  TPaulCheckBox* ModelsCheck;
  void __fastcall ModelsCheckClick(TObject *Sender);
  TPaulCheckBox* PresentCheck;
  void __fastcall PresentCheckClick(TObject *Sender);

  TPaulCheckBox* ShowOptionsCheck;
  TPaulCheckBox* AutoStartPresCheck;

  void GetOptions();
public:
  __fastcall TOptionsForm(void);
  __fastcall ~TOptionsForm(void);
};
//---------------------------------------------------------------------------
#endif

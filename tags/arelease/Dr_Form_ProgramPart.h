//---------------------------------------------------------------------------
#ifndef Dr_Form_ProgramPartH
#define Dr_Form_ProgramPartH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Dr_Form_Hint.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include "DrButton.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TProgramPartForm : public THintForm
{
__published:
  TLabel *CorrectionDateL;
  TDBEdit *CorrectionDate;
  TShape *CorrectionDateS;
  TDBCheckBox *LockPart;
  TDrButton *OkB;
  TDrButton *CancelB;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
public:
  __fastcall TProgramPartForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif


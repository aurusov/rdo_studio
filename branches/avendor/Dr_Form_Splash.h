//---------------------------------------------------------------------------
#ifndef Dr_Form_SplashH
#define Dr_Form_SplashH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Dr_Form_Hint.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TSplashForm : public THintForm
{
__published:
  TProgressBar *ProgressBar;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
public:
  __fastcall TSplashForm(TComponent* Owner);
  __fastcall ~TSplashForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashForm *SplashForm;
//---------------------------------------------------------------------------
#endif


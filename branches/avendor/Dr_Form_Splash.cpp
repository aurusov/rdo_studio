//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
                                
#include "Dr_Form_Splash.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TSplashForm *SplashForm = NULL;
//---------------------------------------------------------------------------
__fastcall TSplashForm::TSplashForm(TComponent* Owner): THintForm(Owner)
{
  SplashForm = this;
}

__fastcall TSplashForm::~TSplashForm()
{
  SplashForm = NULL;
}

void __fastcall TSplashForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  Action = caNone;
}


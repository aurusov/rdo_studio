//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Dr_Form_Plane_Info.h"
#include "Form_Rubber.h"

#include "DrR_Exception.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Dr_Form_Hint"
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TPlaneInfo::TPlaneInfo(TComponent* Owner): THintForm(Owner)
{
  PrecisionTBChange( NULL );
}

void __fastcall TPlaneInfo::OkBClick(TObject *Sender)
{
  ModalResult = mrOk;
}

void __fastcall TPlaneInfo::CancelBClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

void __fastcall TPlaneInfo::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  switch ( ModalResult ) {
    case mrOk    : try {
                     if ( PrecisionTB->Position == 0 ) {
                       double d1 = Koef1E->Text.ToDouble();
                       double d2 = Koef2E->Text.ToDouble();
                       double d3 = Koef3E->Text.ToDouble();
                       double d4 = Koef4E->Text.ToDouble();
                       if ( d1 < 0.0 || d2 < 0.0 || d3 < 0.0 || d4 < 0.0 ) throw RubberError( sErrorPlaneKoef );
                     }
                   } catch (EDatabaseError &e) {
                     CanClose = false;
                     throw;
                   }
                   break;
  }
}

void __fastcall TPlaneInfo::PrecisionTBChange(TObject *Sender)
{
  PrecisionL3->Caption = MiscFormat( "%d%", PrecisionTB->Position );
  bool flag = PrecisionTB->Position == 0;
  KoefL->Enabled     = flag;
  Koef1E->Enabled    = flag;
  Koef1L->Enabled    = flag;
  Koef2E->Enabled    = flag;
  Koef2L->Enabled    = flag;
  Koef3E->Enabled    = flag;
  Koef3L->Enabled    = flag;
  Koef4E->Enabled    = flag;
  Koef4L->Enabled    = flag;
  KoefLoadB->Enabled = flag;
}

void __fastcall TPlaneInfo::KoefLoadBClick(TObject *Sender)
{
  double k1 = 0.0;
  double k2 = 0.0;
  double k3 = 0.0;
  double k4 = 0.0;

  RubberStudio->ImportPlaneKoef( k1, k2, k3, k4 );

  Koef1E->Text = k1;
  Koef2E->Text = k2;
  Koef3E->Text = k3;
  Koef4E->Text = k4;
}


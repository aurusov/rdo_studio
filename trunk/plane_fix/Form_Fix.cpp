//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Form_Fix.h"
#include "data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TFormFix *FormFix;
//---------------------------------------------------------------------------
_fastcall TFormFix::TFormFix(TComponent* Owner): TForm(Owner)
{
}

void __fastcall TFormFix::ButtonClick(TObject *Sender)
{
  Close();
}

void __fastcall TFormFix::FormPaint(TObject *Sender)
{
  if ( !Memo->Lines->Count ) {

    Application->ProcessMessages();

    try {

      for ( int i = 0; i < Planning->ComponentCount; i++ ) {
        if ( dynamic_cast<TTable*>(Planning->Components[i]) ) {
          TTable* table = dynamic_cast<TTable*>(Planning->Components[i]);
          AnsiString s;
          Memo->Lines->Add( s.sprintf("fixed %s...", table->Name.c_str() ) );
          table->Active = true;
          DbiRegenIndexes( table->Handle );
          table->Active = false;
          Memo->Lines->Strings[ Memo->Lines->Count - 1 ] = s.sprintf("fixed %s... \t[ok]", table->Name.c_str() );
          Application->ProcessMessages();
        }
      }
      Memo->Lines->Add( "" );
      Memo->Lines->Add( "Восстановление прошло успешно." );

    } catch ( Exception& ) {
      Memo->Lines->Add( "" );
      Memo->Lines->Add( "Возникла ошибка." );
      Memo->Lines->Add( "Вы должны закрыть программу Rubber или перегрузить компьютер. После этого запустите данную программу снова." );
    }
  }

}


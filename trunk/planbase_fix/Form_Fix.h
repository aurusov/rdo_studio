//---------------------------------------------------------------------------
#ifndef Form_FixH
#define Form_FixH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormFix : public TForm
{
__published:
  TMemo *Memo;
  TPanel *Panel;
  TButton *Button;
  void __fastcall ButtonClick(TObject *Sender);
  void __fastcall FormPaint(TObject *Sender);
public:
  __fastcall TFormFix(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFix *FormFix;
//---------------------------------------------------------------------------
#endif

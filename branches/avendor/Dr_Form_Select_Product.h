//---------------------------------------------------------------------------
#ifndef Dr_Form_Select_ProductH
#define Dr_Form_Select_ProductH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Dr_Form_Select.h"
#include "DrButton.h"
#include <Buttons.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TSelectProductForm : public TSelectForm
{
__published:
  TTimer *Timer;
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall TimerTimer(TObject *Sender);

private:
  AnsiString find_str;
  bool add_key;

public:
  __fastcall TSelectProductForm(TComponent* Owner);
  __fastcall ~TSelectProductForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TSelectProductForm *SelectProductForm;
//---------------------------------------------------------------------------
#endif


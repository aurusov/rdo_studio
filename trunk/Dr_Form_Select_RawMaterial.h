//---------------------------------------------------------------------------
#ifndef Dr_Form_Select_RawMaterialH
#define Dr_Form_Select_RawMaterialH
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
class TSelectRawMaterialForm : public TSelectForm
{
__published:
  TDrButton *RawMaterialEditB;
  void __fastcall RawMaterialEditBClick(TObject *Sender);
public:
  __fastcall TSelectRawMaterialForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif


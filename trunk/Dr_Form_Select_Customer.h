//---------------------------------------------------------------------------
#ifndef Dr_Form_Select_CustomerH
#define Dr_Form_Select_CustomerH
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
class TSelectCustomerForm : public TSelectForm
{
__published:
public:
  __fastcall TSelectCustomerForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif


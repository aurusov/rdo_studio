//---------------------------------------------------------------------------
#ifndef Dr_Form_MachineH
#define Dr_Form_MachineH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Dr_Form_Hint.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include "DrButton.h"
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TMachineForm : public THintForm
{
__published:
  TDBGrid *DBGrid;
  TShape *DBGridS;
  TBevel *Bevel1;
  TBevel *Bevel2;
  TDrButton *SaveB;
  TDrButton *InsB;
  TDrButton *DelB;
  TDrButton *CancelB;
  TDrButton *OkB;
  TDrButton *CancelExitB;
  TLabel *MachineNameL;
  TDBEdit *MachineName;
  TShape *MachineNameS;
  TLabel *PressureL;
  TDBEdit *Pressure;
  TShape *PressureS;
  TLabel *EffortL;
  TDBEdit *Effort;
  TShape *EffortS;
  TLabel *FloorsL;
  TDBEdit *Floors;
  TShape *FloorsS;
  TLabel *FloorHeightL;
  TDBEdit *FloorHeight;
  TShape *FloorHeightS;
  TLabel *LoadSidesL;
  TDBEdit *LoadSides;
  TShape *LoadSidesS;
  TLabel *PlateSizeL;
  TDBEdit *PlateLength;
  TShape *PlateLengthS;
  TDBEdit *PlateWidth;
  TShape *PlateWidthS;
  TLabel *PlateSizeXL;
  TLabel *CommentsL;
  TBevel *CommentsBevel;
  TDBMemo *MachineComment;
  TShape *MachineCommentS;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelExitBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall SaveBClick(TObject *Sender);
  void __fastcall InsBClick(TObject *Sender);
  void __fastcall DelBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
public:
  __fastcall TMachineForm(TComponent* Owner);
  __fastcall ~TMachineForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TMachineForm *MachineForm;
//---------------------------------------------------------------------------
#endif


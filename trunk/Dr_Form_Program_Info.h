//---------------------------------------------------------------------------
#ifndef Dr_Form_Program_InfoH
#define Dr_Form_Program_InfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "Dr_Form_Hint.h"
#include "DrButton.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TProgramInfoForm : public THintForm
{
__published:
  TDrButton *OkB;
  TDrButton *CancelB;
  TDBEdit *CompanyName;
  TShape *CompanyNameS;
  TLabel *ProgDateL;
  TLabel *ProgDate;
  TDrButton *ProgDateB;
  TLabel *CompanyDirectorL;
  TDBEdit *CompanyDirector;
  TLabel *CompanyNameL;
  TBevel *Bevel1;
  TBevel *Bevel2;
  TLabel *CompanyEngineerL;
  TLabel *CompanyCommercialChiefL;
  TLabel *CompanyProductionChiefL;
  TLabel *CompanyDepartmentChiefL;
  TLabel *CompanyTechnologistL;
  TLabel *CompanyEconomistL;
  TShape *CompanyDirectorS;
  TDBEdit *CompanyEngineer;
  TShape *CompanyEngineerS;
  TDBEdit *CompanyCommercialChief;
  TShape *CompanyCommercialChiefS;
  TDBEdit *CompanyProductionChief;
  TShape *CompanyProductionChiefS;
  TDBEdit *CompanyDepartmentChief;
  TShape *CompanyDepartmentChiefS;
  TDBEdit *CompanyTechnologist;
  TShape *CompanyTechnologistS;
  TDBEdit *CompanyEconomist;
  TShape *CompanyEconomistS;
  TShape *CompanyEngineerBS;
  TDrButton *CompanyDirectorB;
  TDrButton *CompanyEngineerB;
  TDrButton *CompanyCommercialChiefB;
  TDrButton *CompanyProductionChiefB;
  TDrButton *CompanyDepartmentChiefB;
  TDrButton *CompanyTechnologistB;
  TDrButton *CompanyEconomistB;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ProgDateBClick(TObject *Sender);
  void __fastcall CompanyDirectorBClick(TObject *Sender);
public:
  __fastcall TProgramInfoForm(TComponent* Owner);
  __fastcall ~TProgramInfoForm();

  bool firstProgram;
  void UpdateDateValue();
};
//---------------------------------------------------------------------------
extern PACKAGE TProgramInfoForm *ProgramInfoForm;
//---------------------------------------------------------------------------
#endif


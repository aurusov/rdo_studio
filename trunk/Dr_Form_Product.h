//---------------------------------------------------------------------------
#ifndef Dr_Form_ProductH
#define Dr_Form_ProductH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include "Dr_Form_Hint.h"
#include <Buttons.hpp>
#include "DrButton.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TProductForm : public THintForm
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
  TLabel *ProductNameL;
  TDBEdit *ProductName;
  TShape *ProductNameS;
  TLabel *TechMapL;
  TDBEdit *TechMap;
  TShape *TechMapS;
  TLabel *WeightL;
  TDBEdit *Weight;
  TShape *WeightS;
  TLabel *ZagWeightL;
  TDBEdit *ZagWeight;
  TShape *ZagWeightS;
  TLabel *LoadTimeL;
  TDBEdit *LoadTime;
  TShape *LoadTimeS;
  TLabel *VulcanTimeL;
  TDBEdit *VulcanTime;
  TShape *VulcanTimeS;
  TLabel *VulcanTemperatureL;
  TDBEdit *VulcanTemperature;
  TShape *VulcanTemperatureS;
  TLabel *PFPressureOn100L;
  TDBEdit *PFPressureOn100;
  TShape *PFPressureOn100S;
  TLabel *RubNameL;
  TDBEdit *RubName;
  TShape *RubNameS;
  TLabel *VulcanNormaL;
  TDBEdit *VulcanNorma;
  TShape *VulcanNormaS;
  TLabel *CutNormaL;
  TDBEdit *CutNorma;
  TShape *CutNormaS;
  TDBEdit *CostForOrder;
  TShape *CostForOrderS;
  TLabel *CostForOrderL;
  TDBCheckBox *Vulcan5Class;
  TBevel *PFBevel;
  TLabel *PFL;
  TDBCheckBox *PFCylindric;
  TLabel *PFSizeL;
  TDBEdit *PFLength;
  TLabel *PFSizeX1L;
  TLabel *PFSizeX2L;
  TLabel *PFSizeDL;
  TShape *PFLengthS;
  TDBEdit *PFWidth;
  TShape *PFWidthS;
  TDBEdit *PFHeight;
  TShape *PFHeightS;
  TDBEdit *PFDiameter;
  TShape *PFDiameterS;
  TLabel *PFNumberL;
  TDBEdit *PFNumber;
  TShape *PFNumberS;
  TLabel *PFPlacesL;
  TDBEdit *PFPlaces;
  TShape *PFPlacesS;
  TDBGrid *RMDBGrid;
  TShape *RMS;
  TLabel *RML;
  TBevel *RMBevel;
  TDrButton *RMB;
  TTimer *Timer;
  void __fastcall OkBClick(TObject *Sender);
  void __fastcall CancelExitBClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall SaveBClick(TObject *Sender);
  void __fastcall InsBClick(TObject *Sender);
  void __fastcall DelBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall DBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall PFCylindricClick(TObject *Sender);
  void __fastcall RMDBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall RMBClick(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall TimerTimer(TObject *Sender);

private:
  AnsiString find_str;
  bool add_key;

public:
  __fastcall TProductForm(TComponent* Owner);
  __fastcall ~TProductForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TProductForm *ProductForm;
//---------------------------------------------------------------------------
#endif


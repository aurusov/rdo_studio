//---------------------------------------------------------------------------
#ifndef Form_RubberH
#define Form_RubberH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include "DrButton.h"
//---------------------------------------------------------------------------
typedef enum {
  sb_Name     = 0,
  sb_TechMap  = 1,
  sb_Customer = 2
} TProgramContentsSortBy;

typedef enum {
  sb_Fio         = 0,
  sb_Press       = 1,
  sb_ProductName = 2
} TDailyTasksSortBy;
//---------------------------------------------------------------------------
class TRubberStudio : public TForm
{
__published:
  TMainMenu *MainMenu;
  TMenuItem *ProgramM;
  TMenuItem *PrNewM;
  TMenuItem *PrOpenM;
  TMenuItem *PrDelM;
  TMenuItem *N1;
  TMenuItem *PrPartNewM;
  TMenuItem *PrPartSelectM;
  TMenuItem *PrPartPropertyM;
  TMenuItem *PrPartDelM;
  TMenuItem *N2;
  TMenuItem *MachInProgM;
  TMenuItem *N3;
  TMenuItem *PrPreviewM;
  TMenuItem *PrPrintM;
  TMenuItem *PrPrintByPrPartM;
  TMenuItem *N4;
  TMenuItem *PrSortM;
  TMenuItem *PrSortByNameM;
  TMenuItem *PrSortByTecpMapM;
  TMenuItem *PrSortByCustomerM;
  TMenuItem *N9;
  TMenuItem *PrSortByPartM;
  TMenuItem *PrInfoM;
  TMenuItem *N5;
  TMenuItem *ExitM;
  TMenuItem *PlaneM;
  TMenuItem *PlaneNewM;
  TMenuItem *PlaneInfoM;
  TMenuItem *N6;
  TMenuItem *PlanePreviewM;
  TMenuItem *PlanePrintM;
  TMenuItem *N10;
  TMenuItem *DailyTaskSortM;
  TMenuItem *DTSortByFioM;
  TMenuItem *DTSortByPressM;
  TMenuItem *DTSortByProductNameM;
  TMenuItem *DBM;
  TMenuItem *DBConnectM;
  TMenuItem *DBDisconnectM;
  TMenuItem *DBReconnectM;
  TMenuItem *N7;
  TMenuItem *DateM;
  TMenuItem *N11;
  TMenuItem *PersonalM;
  TMenuItem *MachineM;
  TMenuItem *N12;
  TMenuItem *ProductM;
  TMenuItem *RawMaterialM;
  TMenuItem *N13;
  TMenuItem *CustomerM;
  TMenuItem *HelpM;
  TMenuItem *ContentsM;
  TMenuItem *SearchOnHelpM;
  TMenuItem *N8;
  TMenuItem *AboutM;
  TToolBar *ToolBar;
  TToolButton *PrNewB;
  TToolButton *PrOpenB;
  TToolButton *S1;
  TToolButton *PrPrintB;
  TToolButton *PrPreviewB;
  TToolButton *S2;
  TToolButton *PlaneNewB;
  TToolButton *PlanePrintB;
  TToolButton *PlanePreviewB;
  TToolButton *S3;
  TToolButton *ContentsB;
  TToolButton *SearchOnHelpB;
  TPageControl *PageControl;
  TTabSheet *ProgramTS;
  TTabSheet *PlaneTS;
  TBevel *Bevel1;
  TBevel *Bevel2;
  TDrButton *SaveB;
  TDrButton *InsB;
  TDrButton *DelB;
  TDrButton *CancelB;
  TDBGrid *PrDBGrid;
  TShape *PrDBGridS;
  TLabel *ProductIDL;
  TShape *ProductIDS;
  TLabel *AmountL;
  TShape *AmountS;
  TLabel *CustomerIDL;
  TShape *CustomerIDS;
  TDBEdit *Amount;
  TDrButton *ProductIDB;
  TDrButton *CustomerIDB;
  TLabel *CostL;
  TDBEdit *Cost;
  TShape *CostS;
  TEdit *ProductID;
  TEdit *CustomerID;
  TDBCheckBox *PressingOrder;
  TDBGrid *DataDailyTasksDBGrid;
  TShape *DataDailyTasksDBGridS;
  TDBGrid *DailyTasksDBGrid;
  TShape *DailyTasksDBGridS;
  TImageList *Images;
  TImageList *DisabledImages;
  TEdit *PersonalID;
  TLabel *PersonalIDL;
  TShape *PersonalIDS;
  TDrButton *PersonalIDB;
  TDBEdit *AmtDone;
  TLabel *AmtDoneL;
  TShape *AmtDoneS;
  TBevel *Bevel3;
  TBevel *Bevel4;
  TDrButton *SaveDayB;
  TDrButton *CancelDayB;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall ExitMClick(TObject *Sender);
  void __fastcall ContentsMClick(TObject *Sender);
  void __fastcall SearchOnHelpMClick(TObject *Sender);
  void __fastcall AboutMClick(TObject *Sender);
  void __fastcall DateMClick(TObject *Sender);
  void __fastcall PersonalMClick(TObject *Sender);
  void __fastcall MachineMClick(TObject *Sender);
  void __fastcall ProductMClick(TObject *Sender);
  void __fastcall RawMaterialMClick(TObject *Sender);
  void __fastcall CustomerMClick(TObject *Sender);
  void __fastcall PrNewMClick(TObject *Sender);
  void __fastcall PrOpenMClick(TObject *Sender);
  void __fastcall PrDelMClick(TObject *Sender);
  void __fastcall MachInProgMClick(TObject *Sender);
  void __fastcall PrPreviewMClick(TObject *Sender);
  void __fastcall PrPrintMClick(TObject *Sender);
  void __fastcall PrPrintBClick(TObject *Sender);
  void __fastcall PrPrintByPrPartMClick(TObject *Sender);
  void __fastcall PrSortByNameMClick(TObject *Sender);
  void __fastcall PrSortByPartMClick(TObject *Sender);
  void __fastcall PrInfoMClick(TObject *Sender);
  void __fastcall PrPartNewMClick(TObject *Sender);
  void __fastcall PrPartPropertyMClick(TObject *Sender);
  void __fastcall PrPartDelMClick(TObject *Sender);
  void __fastcall SaveBClick(TObject *Sender);
  void __fastcall InsBClick(TObject *Sender);
  void __fastcall DelBClick(TObject *Sender);
  void __fastcall CancelBClick(TObject *Sender);
  void __fastcall ProductIDBClick(TObject *Sender);
  void __fastcall CustomerIDBClick(TObject *Sender);
  void __fastcall PrDBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall PlaneNewMClick(TObject *Sender);
  void __fastcall PlanePreviewMClick(TObject *Sender);
  void __fastcall PlanePrintMClick(TObject *Sender);
  void __fastcall PlanePrintBClick(TObject *Sender);
  void __fastcall DTSortByFioMClick(TObject *Sender);
  void __fastcall PlaneInfoMClick(TObject *Sender);
  void __fastcall DataDailyTasksDBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall DailyTasksDBGridDrawColumnCell(TObject *Sender,
    const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State);
  void __fastcall PersonalIDBClick(TObject *Sender);
  void __fastcall SaveDayBClick(TObject *Sender);
  void __fastcall CancelDayBClick(TObject *Sender);
  void __fastcall DailyTasksDBGridMouseMove(TObject *Sender,
    TShiftState Shift, int X, int Y);
private:
  void __fastcall WndProc(Messages::TMessage &Message);
  void __fastcall AppException(TObject *Sender, Exception *E);
  void DrawMenuCaption(TCanvas* canvas, const AnsiString& text, bool enabled, TRect& rect, bool selected, UINT flags);
  void __fastcall OnDrawMenuItem(TObject *Sender, TCanvas *ACanvas, const TRect &ARect, bool Selected);

  int lockProgram_count;
  int lockProgramPart_count;
  int lockOrder_count;
  int lockDailyTask_count;

  void __fastcall PrPartSelectActiveM(TObject *Sender);

  bool bShowStopIcon;

  AnsiString modelDir;
  void BuildModels();
  void ConnectModels();
  int CalcDailyTasks_begin_smena;
  double Amount_sum;
  double VulcanTime_max;
  double LoadTime_max;

  TList* trace_PersonalID;
  TList* trace_MachineID;
  TList* trace_ProductID;
  void BuildDailyTasks();

  TPoint DailyTasks_point;

  bool bPrintByPrPart;

  int plane_precision;
  double plane_k1;
  double plane_k2;
  double plane_k3;
  double plane_k4;

public:
  __fastcall TRubberStudio(TComponent* Owner);
  __fastcall ~TRubberStudio();

  TProgramContentsSortBy ProgramContents_sortBy;
  TDailyTasksSortBy      DailyTasks_sortBy;
  bool isPrintByPrPart();

  int activeProgram_ID;
  void LockProgram();
  void UnlockProgram();
  bool isUnlockProgram_ID();

  int activeProgramPart_ID;
  void LockProgramPart();
  void UnlockProgramPart();
  bool isUnlockProgramPart_ID();

  int activeOrder_ID;
  void LockOrder();
  void UnlockOrder();
  bool isUnlockOrder_ID();

  int activeDailyTask;
  void LockDailyTask();
  void UnlockDailyTask();
  bool isUnlockDailyTask_ID();

  double vulcanProgramTime;
  double vulcanProgramPrice;
  double cutProgramTime;
  double cutProgramPrice;
  double customerProgramPrice;
  void CalculateAllPrice();

  void UpdateCaption();
  void RefreshStudioControls();
  void UpdateProductControls(const bool insert_record = false);
  void RefreshProgramContents();
  void RefreshOrderID();

  void UpdateDailyTaskControls();
  void RefreshDailyTasks();
  void RefreshDailyTaskID();

  int GetPlanePrecision();
  double GetPlaneKoef( const int koef_id );
  void ImportPlaneKoef( double& k1, double& k2, double& k3, double& k4 );

};
//---------------------------------------------------------------------------
extern PACKAGE TRubberStudio *RubberStudio;
//---------------------------------------------------------------------------
#endif


//---------------------------------------------------------------------------
#ifndef DataModuleH
#define DataModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TData : public TDataModule
{
__published:
  TTable *Personal;
  TDataSource *PersonalDS;
  TTable *Machines;
  TDataSource *MachinesDS;
  TTable *Products;
  TDataSource *ProductsDS;
  TTable *Customers;
  TDataSource *CustomersDS;
  TTable *Programs;
  TDataSource *ProgramsDS;
  TTable *ProgramPart;
  TDataSource *ProgramPartDS;
  TQuery *ShowProgramPart;
  TDataSource *ShowProgramPartDS;
  TTable *Orders;
  TDataSource *OrdersDS;
  TTable *MachInProg;
  TDataSource *MachInProgDS;
  TQuery *CheckMachInProg;
  TDataSource *CheckMachInProgDS;
  TQuery *AvailPers;
  TDataSource *AvailPersDS;
  TTable *DateTable;
  TDataSource *DateTableDS;
  TTable *RawMaterial;
  TDataSource *RawMaterialDS;
  TTable *RawMaterialInProduct;
  TDataSource *RawMaterialInProductDS;
  TTable *RawMaterialCost;
  TDataSource *RawMaterialCostDS;
  TQuery *ShowFreeDate;
  TDataSource *ShowFreeDateDS;
  TQuery *CheckProgramCountByDate;
  TDataSource *CheckProgramCountByDateDS;
  TQuery *ShowProgramContents;
  TDataSource *ShowProgramContentsDS;
  TTable *PlanInProg;
  TDataSource *PlanInProgDS;
  TTable *DailyTasks;
  TDataSource *DailyTasksDS;
  TQuery *ShowDailyTasksDate;
  TDataSource *ShowDailyTasksDateDS;
  TQuery *ShowDailyTasks;
  TDataSource *ShowDailyTasksDS;
  TQuery *DailyTaskGroupCount;
  TDataSource *DailyTaskGroupCountDS;
  TQuery *GetFirstCalcSmena;
  TDataSource *GetFirstCalcSmenaDS;
  TQuery *GetAmtDone;
  TDataSource *GetAmtDoneDS;
  TQuery *Virtual_Personal;
  TDataSource *Virtual_PersonalDS;
  TQuery *Model_RES_Machine;
  TDataSource *Model_RES_MachineDS;
  TQuery *Model_RES_Product;
  TDataSource *Model_RES_ProductDS;
  TQuery *ShowProgContFullGroup;

  void __fastcall DateTableDSStateChange(TObject *Sender);
  void __fastcall DateTableAfterDelete(TDataSet *DataSet);
  void __fastcall DateTableAfterPost(TDataSet *DataSet);
  void __fastcall DateTableAfterScroll(TDataSet *DataSet);
  void __fastcall PersonalDSStateChange(TObject *Sender);
  void __fastcall PersonalAfterDelete(TDataSet *DataSet);
  void __fastcall PersonalAfterPost(TDataSet *DataSet);
  void __fastcall MachinesDSStateChange(TObject *Sender);
  void __fastcall MachinesAfterDelete(TDataSet *DataSet);
  void __fastcall MachinesAfterScroll(TDataSet *DataSet);
  void __fastcall MachInProgDSStateChange(TObject *Sender);
  void __fastcall MachInProgDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action);
  void __fastcall ProductsDSStateChange(TObject *Sender);
  void __fastcall ProductsAfterDelete(TDataSet *DataSet);
  void __fastcall ProductsBeforePost(TDataSet *DataSet);
  void __fastcall ProductsPostError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action);
  void __fastcall ProductsAfterPost(TDataSet *DataSet);
  void __fastcall RawMaterialDSStateChange(TObject *Sender);
  void __fastcall RawMaterialAfterDelete(TDataSet *DataSet);
  void __fastcall RawMaterialAfterPost(TDataSet *DataSet);
  void __fastcall RawMaterialInProductDSStateChange(TObject *Sender);
  void __fastcall RawMaterialInProductAfterDelete(TDataSet *DataSet);
  void __fastcall CustomersDSStateChange(TObject *Sender);
  void __fastcall CustomersAfterDelete(TDataSet *DataSet);
  void __fastcall ProgramsDSStateChange(TObject *Sender);
  void __fastcall ProgramsAfterDelete(TDataSet *DataSet);
  void __fastcall ProgramsAfterPost(TDataSet *DataSet);
  void __fastcall ProgramsAfterScroll(TDataSet *DataSet);
  void __fastcall ProgramsDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action);
  void __fastcall ProgramPartDSStateChange(TObject *Sender);
  void __fastcall ProgramPartAfterPost(TDataSet *DataSet);
  void __fastcall ProgramPartAfterScroll(TDataSet *DataSet);
  void __fastcall ProgramPartDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action);
  void __fastcall OrdersDSStateChange(TObject *Sender);
  void __fastcall OrdersAfterDelete(TDataSet *DataSet);
  void __fastcall OrdersAfterPost(TDataSet *DataSet);
  void __fastcall ShowProgramContentsBeforeScroll(TDataSet *DataSet);
  void __fastcall ShowProgramContentsAfterScroll(TDataSet *DataSet);
  void __fastcall PlanInProgAfterScroll(TDataSet *DataSet);
  void __fastcall PlanInProgDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action);
  void __fastcall DailyTasksDSStateChange(TObject *Sender);
  void __fastcall DailyTasksAfterPost(TDataSet *DataSet);
  void __fastcall ShowDailyTasksDateBeforeScroll(TDataSet *DataSet);
  void __fastcall ShowDailyTasksDateAfterScroll(TDataSet *DataSet);
  void __fastcall ShowDailyTasksAfterScroll(TDataSet *DataSet);

public:
  __fastcall TData(TComponent* Owner);
  __fastcall ~TData();
  bool CanPost(TDataSource* DataSource);

  void Update_ShowFreeDate();
  int GetProgramCountByDate(const int DateID);
  AnsiString GetDateValue(const int date_id);
  AnsiString GetDateValue();
  AnsiString GetPersonalName();

  bool LockPersonal;
  bool LockMachine;
  bool LockProduct;

  bool LocateProduct(const int product_id);
  bool LocateProduct();
};
//---------------------------------------------------------------------------
extern PACKAGE TData *Data;
//---------------------------------------------------------------------------
#endif


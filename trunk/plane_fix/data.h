#ifndef dataH
#define dataH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

class TPlanning : public TDataModule
{
__published:
  TTable *Customers;
  TTable *DailyTasks;
  TTable *DateTable;
  TTable *MachineInProg;
  TTable *Machines;
  TTable *Orders;
  TTable *Personal;
  TTable *PlanInProg;
  TTable *Products;
  TTable *ProgramPart;
  TTable *Programs;
  TTable *RawMaterial;
  TTable *RawMaterialCost;
  TTable *RawMaterialInProduct;
public:
  __fastcall TPlanning(TComponent* Owner);
};

extern PACKAGE TPlanning *Planning;

#endif

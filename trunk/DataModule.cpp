//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DataModule.h"
#include "Dr_Form_Date.h"
#include "Dr_Form_Personal.h"
#include "Dr_Form_Machine.h"
#include "Dr_Form_Product.h"
#include "Dr_Form_Select_Product.h"
#include "Dr_Form_RawMaterial.h"
#include "Dr_Form_RawMaterialInProduct.h"
#include "Dr_Form_Customer.h"
#include "Dr_Form_Select_Date.h"
#include "Form_Rubber.h"
#include "Dr_Form_Program_Machine.h"

#include "Misc.h"
#include "ConstRes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TData *Data = NULL;
//---------------------------------------------------------------------------
__fastcall TData::TData(TComponent* Owner):
  TDataModule(Owner),
  LockPersonal(false),
  LockMachine(false),
  LockProduct(false)
{
  DateTable->Active            = true;
  Personal->Active             = true;
  Machines->Active             = true;
  Products->Active             = true;
  RawMaterial->Active          = true;
  RawMaterialCost->Active      = true;
  RawMaterialInProduct->Active = true;
  Customers->Active            = true;
  MachInProg->Active           = true;
  PlanInProg->Active           = true;
  DailyTasks->Active           = true;
  ProgramPart->Active          = true;
  Programs->Active             = true;
  Orders->Active               = true;
  AvailPers->Open();
}

__fastcall TData::~TData()
{
  if (OrdersDS->State == dsEdit || OrdersDS->State == dsInsert)
    Orders->Cancel();
  DateTable->Active            = false;
  Personal->Active             = false;
  Machines->Active             = false;
  Products->Active             = false;
  RawMaterial->Active          = false;
  RawMaterialCost->Active      = false;
  RawMaterialInProduct->Active = false;
  Customers->Active            = false;
  MachInProg->Active           = false;
  PlanInProg->Active           = false;
  DailyTasks->Active           = false;
  ProgramPart->Active          = false;
  Programs->Active             = false;
  Orders->Active               = false;
  AvailPers->Close();
  ShowProgramContents->Close();
  ShowDailyTasks->Close();
  ShowDailyTasksDate->Close();
}

bool TData::CanPost(TDataSource* DataSource)
{
  return DataSource->State == dsEdit || DataSource->State == dsInsert;
}

// ------------------------------------------------------------------
// ---------- Календарь
// ------------------------------------------------------------------
void __fastcall TData::DateTableDSStateChange(TObject *Sender)
{
  if (DateForm) {
    bool can_post = CanPost( DateTableDS );
    DateForm->SaveB->Enabled   = can_post;
    DateForm->DelB->Enabled    = !DateTable->IsEmpty();
    DateForm->CancelB->Enabled = can_post;
  }
  if (SelectDateForm)
    SelectDateForm->UpdateOkB();
}

void __fastcall TData::DateTableAfterDelete(TDataSet *DataSet)
{
  DateTableDSStateChange(NULL);
  Update_ShowFreeDate();
}

void __fastcall TData::DateTableAfterPost(TDataSet *DataSet)
{
  // Программа выпуска имеет дискрету один месяц, т.е. день месяца должен быть = 1
  // для основной части производственной программы
  TField* field = DateTable->FieldByName("DateValue");
  TDateTime date = field->AsDateTime;
  unsigned short year, month, day;
  date.DecodeDate(&year, &month, &day);
  day = 1;
  TDateTime new_date = TDateTime(year, month, day);
  if (field->AsDateTime != new_date) {
    DateTable->Edit();
    field->AsDateTime = new_date;
    DateTable->Post();
  }

  Update_ShowFreeDate();
  if (RubberStudio) {
    RubberStudio->UpdateCaption();
    RubberStudio->RefreshProgramContents();
  }
}

void __fastcall TData::DateTableAfterScroll(TDataSet *DataSet)
{
  if (DateForm) {
    int res_id = GetProgramCountByDate(DateTable->FieldByName("DateID")->AsInteger) ? sHaveProgramByDate : sNoProgramByDate;
    DateForm->Program->Caption = LoadStr(res_id);
  }
}

void TData::Update_ShowFreeDate()
{
  if (SelectDateForm) {
    ShowFreeDate->Close();
    ShowFreeDate->Prepare();
    ShowFreeDate->Open();
    SelectDateForm->UpdateOkB();
  }
}

int TData::GetProgramCountByDate(const int DateID)
{
  try {
    CheckProgramCountByDate->Close();
    CheckProgramCountByDate->ParamByName("DateID")->AsInteger = DateTable->FieldByName("DateID")->AsInteger;
    CheckProgramCountByDate->Prepare();
    CheckProgramCountByDate->Open();
    int result = CheckProgramCountByDate->IsEmpty() ? 0 : 1;
    CheckProgramCountByDate->Close();
    return result;
  } catch (Exception &e) {
    CheckProgramCountByDate->Close();
    throw;
  }
}

AnsiString TData::GetDateValue(const int date_id)
{
  AnsiString str;
  if (date_id) {
    TLocateOptions locate;
    locate.Clear();
    DateTable->Locate("DateID", date_id, locate);
    str = DateTable->FieldByName("DateValue")->AsDateTime.FormatString(LoadStr(sDateMask));
  } else {
    str = LoadStr(sUnknowDate);
  }
  return str;
}

AnsiString TData::GetDateValue()
{
  return GetDateValue(Programs->FieldByName("DateID")->AsInteger);
}

// ------------------------------------------------------------------
// ---------- Сотрудники - основная таблица
// ------------------------------------------------------------------
void __fastcall TData::PersonalDSStateChange(TObject *Sender)
{
  if (PersonalForm) {
    bool can_post = CanPost( PersonalDS );
    PersonalForm->SaveB->Enabled   = can_post;
    PersonalForm->DelB->Enabled    = !Personal->IsEmpty();
    PersonalForm->CancelB->Enabled = can_post;
  }
}

void __fastcall TData::PersonalAfterDelete(TDataSet *DataSet)
{
  PersonalDSStateChange(NULL);
}

void __fastcall TData::PersonalAfterPost(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockDailyTask_ID() && !LockPersonal)
    RubberStudio->RefreshDailyTasks();
}
AnsiString TData::GetPersonalName()
{
  AnsiString str = "";
  AnsiString s;
  // Фамилия
  str = Personal->FieldByName("LastName")->AsString;
  str = str.Trim();
  // Имя
  s = Personal->FieldByName("FirstName")->AsString;
  s = s.Trim();
  if (s.Length()) str += MiscFormat(" %c.", s[1]);
  // Отчество
  s = Personal->FieldByName("SecondName")->AsString;
  s = s.Trim();
  if (s.Length()) str += MiscFormat(" %c.", s[1]);
  return str;
}

// ------------------------------------------------------------------
// ---------- Оборудование - основная таблица
// ------------------------------------------------------------------
void __fastcall TData::MachinesDSStateChange(TObject *Sender)
{
  if (MachineForm) {
    bool can_post = CanPost( MachinesDS );
    MachineForm->SaveB->Enabled   = can_post;
    MachineForm->DelB->Enabled    = !Machines->IsEmpty();
    MachineForm->CancelB->Enabled = can_post;
  }
  MachInProgDSStateChange(NULL);
}

void __fastcall TData::MachinesAfterDelete(TDataSet *DataSet)
{
  MachinesDSStateChange(NULL);
}

void __fastcall TData::MachinesAfterScroll(TDataSet *DataSet)
{
  if (MachineInProgForm && !LockMachine) {
    CheckMachInProg->Close();
    try {
      CheckMachInProg->ParamByName("progID")->AsInteger = RubberStudio->activeProgram_ID;
      CheckMachInProg->ParamByName("machID")->AsInteger = Machines->FieldByName("MachineID")->AsInteger;
      CheckMachInProg->Prepare();
      CheckMachInProg->Open();
      bool flag = CheckMachInProg->IsEmpty();
      CheckMachInProg->Close();
      MachineInProgForm->InsB->Enabled = flag;
    } catch (Exception& e) {
      CheckMachInProg->Close();
      throw;
    }
  }
  MachInProgDSStateChange(NULL);
}

// ------------------------------------------------------------------
// ---------- Оборудование - оспользование в программе
// ------------------------------------------------------------------
void __fastcall TData::MachInProgDSStateChange(TObject *Sender)
{
  if (MachineInProgForm)
    MachineInProgForm->DelB->Enabled = !MachInProg->IsEmpty();
}

void __fastcall TData::MachInProgDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action)
{
  EDBEngineError* err = dynamic_cast<EDBEngineError*>(E);
  if (err && err->Errors[0]->ErrorCode == DBIERR_DETAILRECORDSEXIST) {
    ShowErrorMessage(sErrorDeleteMachine);
    Action = daAbort;
  }
}

// ------------------------------------------------------------------
// ---------- Номенклатура
// ------------------------------------------------------------------
void __fastcall TData::ProductsDSStateChange(TObject *Sender)
{
  if ( ProductForm ) {
    bool can_post = CanPost( ProductsDS );
    ProductForm->SaveB->Enabled   = can_post;
    ProductForm->DelB->Enabled    = !Products->IsEmpty();
    ProductForm->CancelB->Enabled = can_post;
    ProductForm->RMB->Enabled     = Products->FieldByName("ProductID")->AsInteger;
  }
}

void __fastcall TData::ProductsAfterDelete(TDataSet *DataSet)
{
  ProductsDSStateChange(NULL);
}

void __fastcall TData::ProductsBeforePost(TDataSet *DataSet)
{
  LockProduct = true;
}

void __fastcall TData::ProductsAfterPost(TDataSet *DataSet)
{
  try {
    if ( RubberStudio || !LockProduct ) {
      RubberStudio->RefreshProgramContents();
      RubberStudio->UpdateProductControls();
    }
    LockProduct = false;
  } catch (Exception& e) {
    Data->LockProduct = false;
    throw;
  }
}

void __fastcall TData::ProductsPostError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action)
{
  LockProduct = false;
}

bool TData::LocateProduct(const int product_id)
{
  if (ProductForm || SelectProductForm || LockProduct) return false;
  if (Products->FieldByName("ProductID")->AsInteger == product_id) return true;
  TLocateOptions locate;
  locate.Clear();
  return Products->Locate("ProductID", product_id, locate);
}

bool TData::LocateProduct()
{
  return LocateProduct(ShowProgramContents->FieldByName("ProductID")->AsInteger);
}

// ------------------------------------------------------------------
// ---------- Сырье и основные материалы - основная таблица
// ------------------------------------------------------------------
void __fastcall TData::RawMaterialDSStateChange(TObject *Sender)
{
  if (RawMaterialForm) {
    bool can_post = CanPost( RawMaterialDS );
    RawMaterialForm->SaveB->Enabled   = can_post;
    RawMaterialForm->DelB->Enabled    = !RawMaterial->IsEmpty();
    RawMaterialForm->CancelB->Enabled = can_post;
  }
}

void __fastcall TData::RawMaterialAfterDelete(TDataSet *DataSet)
{
  RawMaterialDSStateChange(NULL);
}

void __fastcall TData::RawMaterialAfterPost(TDataSet *DataSet)
{
  if (RawMaterialInProductForm)
    RawMaterialInProductForm->UpdateRMName();
}

// ------------------------------------------------------------------
// ---------- Сырье и основные материалы - использование в конкретном изделии (ProductID)
// ------------------------------------------------------------------
void __fastcall TData::RawMaterialInProductDSStateChange(TObject *Sender)
{
  if (RawMaterialInProductForm) {
    bool can_post = CanPost( RawMaterialInProductDS );
    RawMaterialInProductForm->SaveB->Enabled   = can_post;
    RawMaterialInProductForm->DelB->Enabled    = !RawMaterialInProduct->IsEmpty();
    RawMaterialInProductForm->CancelB->Enabled = can_post;
  }
}
                                     
void __fastcall TData::RawMaterialInProductAfterDelete(TDataSet *DataSet)
{
  RawMaterialInProductDSStateChange(NULL);
}

// ------------------------------------------------------------------
// ---------- Заказчики
// ------------------------------------------------------------------
void __fastcall TData::CustomersDSStateChange(TObject *Sender)
{
  if (CustomerForm) {
    bool can_post = CanPost( CustomersDS );
    CustomerForm->SaveB->Enabled   = can_post;
    CustomerForm->DelB->Enabled    = !Customers->IsEmpty();
    CustomerForm->CancelB->Enabled = can_post;
  }
}

void __fastcall TData::CustomersAfterDelete(TDataSet *DataSet)
{
  CustomersDSStateChange(NULL);
}

// ------------------------------------------------------------------
// ---------- Производственная программа - основная таблица
// ------------------------------------------------------------------
void __fastcall TData::ProgramsDSStateChange(TObject *Sender)
{
  if (RubberStudio) {
    RubberStudio->RefreshStudioControls();
    if (RubberStudio->isUnlockProgram_ID()) OrdersDSStateChange(NULL);
  }
}

void __fastcall TData::ProgramsAfterDelete(TDataSet *DataSet)
{
  ProgramsDSStateChange(NULL);
  if (RubberStudio && RubberStudio->isUnlockProgram_ID())
    RubberStudio->UpdateCaption();
}

void __fastcall TData::ProgramsAfterPost(TDataSet *DataSet)
{
  ProgramsDSStateChange(NULL);
  if (RubberStudio && RubberStudio->isUnlockProgram_ID())
    RubberStudio->UpdateCaption();
}

void __fastcall TData::ProgramsAfterScroll(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockProgram_ID()) {
    ProgramsDSStateChange(NULL);
    RubberStudio->activeProgram_ID = Programs->FieldByName("ProgramID")->AsInteger;

    RubberStudio->UpdateCaption();
    RubberStudio->RefreshProgramContents();
    RubberStudio->UpdateProductControls();
  }
}

void __fastcall TData::ProgramsDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action)
{
  EDBEngineError* err = dynamic_cast<EDBEngineError*>(E);
  if (err && err->Errors[0]->ErrorCode == DBIERR_DETAILRECORDSEXIST) {
    try {
      if (RubberStudio)
        RubberStudio->LockProgramPart();
      while (!ProgramPart->IsEmpty())
        ProgramPart->Delete();
      while (!PlanInProg->IsEmpty())
        PlanInProg->Delete();
      while (!MachInProg->IsEmpty())
        MachInProg->Delete();
      if (RubberStudio) {
        RubberStudio->UnlockProgramPart();
        RubberStudio->activeProgramPart_ID = 0;
        RubberStudio->activeOrder_ID = 0;
      }
      Action = daRetry;
    } catch (Exception &e) {
      if (RubberStudio)
        RubberStudio->UnlockProgramPart();
      Action = daFail;
      throw;
    }
  }
}

// ------------------------------------------------------------------
// ---------- Производственная программа - составные части производственной программы
// ------------------------------------------------------------------
void __fastcall TData::ProgramPartDSStateChange(TObject *Sender)
{
  if (RubberStudio && RubberStudio->isUnlockProgramPart_ID()) {
    OrdersDSStateChange(NULL);
    RubberStudio->UpdateCaption();
  }
}

void __fastcall TData::ProgramPartAfterPost(TDataSet *DataSet)
{
  ProgramPartDSStateChange(NULL);
}

void __fastcall TData::ProgramPartAfterScroll(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockProgramPart_ID()) {
    int part_id = ProgramPart->FieldByName("ProgramPartID")->AsInteger;
    RubberStudio->activeProgramPart_ID = part_id;
    RubberStudio->PrSortByNameMClick(NULL);
    RubberStudio->RefreshStudioControls();
    for (int i = 0; i < RubberStudio->PrPartSelectM->Count; i++) {
      TMenuItem* menu = RubberStudio->PrPartSelectM->Items[i];
      if (menu->Tag == part_id) {
        menu->Checked = true;
        break;
      }
    }
  }
}

void __fastcall TData::ProgramPartDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action)
{
  EDBEngineError* err = dynamic_cast<EDBEngineError*>(E);
  if (err && err->Errors[0]->ErrorCode == DBIERR_DETAILRECORDSEXIST) {
    RubberStudio->LockOrder();
    try {
      int part_id = ProgramPart->FieldByName("ProgramPartID")->AsInteger;
      Orders->First();
      while (!Orders->Eof) {
        if (Orders->FieldByName("ProgramPartID")->AsInteger == part_id) {
          Orders->Delete();
          continue;
        }
        Orders->Next();
      }
      Action = daRetry;
      RubberStudio->UnlockOrder();
      RubberStudio->RefreshProgramContents();
    } catch (Exception &e) {
      Action = daFail;
      RubberStudio->UnlockOrder();
      throw;
    }
  }
}

// ------------------------------------------------------------------
// ---------- Заказы
// ------------------------------------------------------------------
void __fastcall TData::OrdersDSStateChange(TObject *Sender)
{
  if ( RubberStudio ) {
    bool can_post = CanPost(OrdersDS);
    RubberStudio->SaveB->Enabled     = can_post;
    RubberStudio->InsB->Enabled      = !Programs->IsEmpty();
    RubberStudio->DelB->Enabled      = !Orders->IsEmpty();
    RubberStudio->CancelB->Enabled   = can_post;
    if ( RubberStudio->isUnlockOrder_ID() ) {
      RubberStudio->RefreshStudioControls();
      if (!CanPost(ProgramsDS) && !CanPost(ProgramPartDS) && !can_post && !CanPost(CustomersDS) )
        RubberStudio->RefreshProgramContents();
    }
  }
}

void __fastcall TData::OrdersAfterDelete(TDataSet *DataSet)
{
  OrdersDSStateChange(NULL);
}

void __fastcall TData::OrdersAfterPost(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockOrder_ID()) {
    int order_id = Orders->FieldByName("OrderID")->AsInteger;
    if (order_id) RubberStudio->activeOrder_ID = order_id;
    RubberStudio->UpdateProductControls();
  }
}

// ------------------------------------------------------------------
// ---------- Производственная программа - Show
// ------------------------------------------------------------------
void __fastcall TData::ShowProgramContentsBeforeScroll(TDataSet *DataSet)
{
  if (RubberStudio && CanPost(OrdersDS)) {
    RubberStudio->LockOrder();
    try {
      Orders->Post();
      RubberStudio->UnlockOrder();
      OrdersDSStateChange(NULL);
    } catch (Exception& e) {
      RubberStudio->UnlockOrder();
      throw;
    }
  }
}

void __fastcall TData::ShowProgramContentsAfterScroll(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockOrder_ID()) {
    int order_id = ShowProgramContents->FieldByName("OrderID")->AsInteger;
    TLocateOptions locate;
    locate.Clear();
    Orders->Locate("OrderID", order_id, locate);
  }
}

// ------------------------------------------------------------------
// ---------- Сменно-суточные задания (таблица, связанная с производственной программой)
// ----------------------------------------------------------------
void __fastcall TData::PlanInProgAfterScroll(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockDailyTask_ID())
    RubberStudio->RefreshDailyTasks();
}

void __fastcall TData::PlanInProgDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action)
{
  EDBEngineError* err = dynamic_cast<EDBEngineError*>(E);
  if (err && err->Errors[0]->ErrorCode == DBIERR_DETAILRECORDSEXIST) {
    try {
      while (!DailyTasks->IsEmpty())
        DailyTasks->Delete();
      Action = daRetry;
    } catch (Exception &e) {
      Action = daFail;
      throw;
    }
  }
}

// ------------------------------------------------------------------
// ---------- Сменно-суточные задания
// ----------------------------------------------------------------
void __fastcall TData::DailyTasksDSStateChange(TObject *Sender)
{
  if ( RubberStudio ) {
    bool can_post = CanPost( DailyTasksDS );
    RubberStudio->SaveDayB->Enabled   = can_post;
    RubberStudio->CancelDayB->Enabled = can_post;
    if ( RubberStudio->isUnlockDailyTask_ID() ) {
      RubberStudio->RefreshStudioControls();
      if ( !can_post ) RubberStudio->RefreshDailyTasks();
    }
  }
}

void __fastcall TData::DailyTasksAfterPost(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockDailyTask_ID()) {
    int daily_task = DailyTasks->FieldByName("PlanID")->AsInteger;
    if (daily_task) RubberStudio->activeDailyTask = daily_task;
    RubberStudio->UpdateDailyTaskControls();
  }
}

// ------------------------------------------------------------------
// ---------- Сменно-суточные задания - Show date
// ----------------------------------------------------------------
void __fastcall TData::ShowDailyTasksDateBeforeScroll(TDataSet *DataSet)
{
  if (RubberStudio && CanPost(DailyTasksDS)) {
    RubberStudio->LockDailyTask();
    try {
      DailyTasks->Post();
      RubberStudio->UnlockDailyTask();
      DailyTasksDSStateChange(NULL);
    } catch (Exception& e) {
      RubberStudio->UnlockDailyTask();
      throw;
    }
  }
}

void __fastcall TData::ShowDailyTasksDateAfterScroll(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockDailyTask_ID()) {
    RubberStudio->RefreshDailyTasks();
  }
}

// ------------------------------------------------------------------
// ---------- Сменно-суточные задания - Show
// ----------------------------------------------------------------
void __fastcall TData::ShowDailyTasksAfterScroll(TDataSet *DataSet)
{
  if (RubberStudio && RubberStudio->isUnlockOrder_ID()) {
    int daily_task = ShowDailyTasks->FieldByName("PlanID")->AsInteger;
    TLocateOptions locate;
    locate.Clear();
    DailyTasks->Locate("PlanID", daily_task, locate);
    RubberStudio->UpdateDailyTaskControls();
  }
}


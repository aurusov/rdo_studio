//---------------------------------------------------------------------------
#include <vcl.h>
#include <SysUtils.hpp>
#include <math.h>
#pragma hdrstop

#include "Form_Rubber.h"
#include "DataModule.h"
#include "Dr_Form_Date.h"
#include "Dr_Form_Personal.h"
#include "Dr_Form_Machine.h"
#include "Dr_Form_Product.h"
#include "Dr_Form_RawMaterial.h"
#include "Dr_Form_Customer.h"

#include "Dr_Form_Select_Program.h"
#include "Dr_Form_Program_Info.h"
#include "Dr_Form_Program_Machine.h"
#include "Dr_Form_ProgramPart.h"
#include "Dr_Form_Select_Product.h"
#include "Dr_Form_Select_Customer.h"
#include "Dr_Form_Select_Personal.h"
#include "Dr_Form_Plane_Info.h"

#include "Dr_Form_Splash.h"
#include "Dr_Form_About.h"
#include "DrR_Exception.h"

#include "Pl_Program_Report.h"
#include "Pl_DailyTasks_Report.h"

#include "Misc.h"
#include "ConstRes.h"
#include "Program_report_consts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DrButton"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TRubberStudio *RubberStudio = NULL;
bool PrintDialogShowed = false;

bool log_flag          = false;
bool trace_flag        = false;
bool long_smena_flag   = false;
bool new_personal_flag = false;
double fill_koef       = 0.85;
//---------------------------------------------------------------------------
typedef struct {
  int model_id;
  int db_id;
} TTraceMachineID;

typedef struct {
  int    model_id;
  int    db_id;
  int    last;
  int    tech_group_index;
  double koef_load_time_plases;
} TTraceProductID;
//---------------------------------------------------------------------------
__fastcall TRubberStudio::TRubberStudio(TComponent* Owner):
  TForm(Owner),
  lockProgram_count(0),
  lockProgramPart_count(0),
  lockOrder_count(0),
  lockDailyTask_count(0),
  activeProgram_ID(0),
  activeProgramPart_ID(0),
  activeOrder_ID(0),
  activeDailyTask(0),
  ProgramContents_sortBy(sb_Customer),
  DailyTasks_sortBy(sb_Fio),
  bPrintByPrPart(true),
  plane_precision(0),
  plane_k1(0.505882),
  plane_k2(0.6),
  plane_k3(0.768538),
  plane_k4(0.0944518),
  vulcanProgramTime(0.0),
  vulcanProgramPrice(0.0),
  cutProgramTime(0.0),
  cutProgramPrice(0.0),
  customerProgramPrice(0.0),
  trace_PersonalID(NULL),
  trace_MachineID(NULL),
  trace_ProductID(NULL),
  bShowStopIcon(false),
  CalcDailyTasks_begin_smena(0),
  Amount_sum(0.0),
  VulcanTime_max(0.0),
  LoadTime_max(0.0)
{
  Application->OnException = AppException;
  Application->Title       = LoadStr(sAppTitle);
  if (Application->HelpFile.Length())
    Application->HelpFile = GetNormalDirFromName(Application->ExeName) + ExtractFileName(Application->HelpFile);
  modelDir = MiscFormat("%smodel\\", GetNormalDirFromName(Application->ExeName).c_str());

  for (int i = 0; i <= ParamCount(); i++) {
    AnsiString str = LowerCase(ParamStr(i));
    if (str == "-log")          log_flag          = true;
    if (str == "-trace")        trace_flag        = true;
    if (str == "-long_smena")   long_smena_flag   = true;
    if (str == "-new_personal") new_personal_flag = true;
    if (str == "-fill_koef" && (i + 1 ) <= ParamCount() ) {
      try {
        fill_koef = PickOutDouble( ParamStr(i+1), 1 );
        if ( fill_koef < 0.01 || fill_koef > 1.0 )
          fill_koef = 0.85;
      } catch (Exception& e) {
        fill_koef = 0.85;
      }
    }
  }

  PrNewM->ImageIndex          = 0;
  PrNewB->ImageIndex          = 0;
  PrOpenM->ImageIndex         = 1;
  PrOpenB->ImageIndex         = 1;
  PrDelM->ImageIndex          = 2;
  PrPartNewM->ImageIndex      = 3;
  PrPartPropertyM->ImageIndex = 4;
  PrPartDelM->ImageIndex      = 5;
  MachInProgM->ImageIndex     = 6;
  PrPreviewM->ImageIndex      = 7;
  PrPreviewB->ImageIndex      = 7;
  PrPrintM->ImageIndex        = 8;
  PrPrintB->ImageIndex        = 8;
  PrInfoM->ImageIndex         = 9;
  PlaneNewM->ImageIndex       = 10;
  PlaneNewB->ImageIndex       = 10;
  PlaneInfoM->ImageIndex      = 11;
  PlanePreviewM->ImageIndex   = 12;
  PlanePreviewB->ImageIndex   = 12;
  PlanePrintM->ImageIndex     = 13;
  PlanePrintB->ImageIndex     = 13;
  DBConnectM->ImageIndex      = 14;
  DBDisconnectM->ImageIndex   = 15;
  DBReconnectM->ImageIndex    = 16;
  DateM->ImageIndex           = 17;
  PersonalM->ImageIndex       = 18;
  MachineM->ImageIndex        = 19;
  ProductM->ImageIndex        = 20;
  RawMaterialM->ImageIndex    = 21;
  CustomerM->ImageIndex       = 22;
  ExitM->ImageIndex           = 23;
  ContentsM->ImageIndex       = 24;
  ContentsB->ImageIndex       = 24;
  SearchOnHelpM->ImageIndex   = 25;
  SearchOnHelpB->ImageIndex   = 25;
  AboutM->ImageIndex          = 26;

  PrNewM->OnDrawItem               = OnDrawMenuItem;
  PrOpenM->OnDrawItem              = OnDrawMenuItem;
  PrDelM->OnDrawItem               = OnDrawMenuItem;
  PrPartNewM->OnDrawItem           = OnDrawMenuItem;
  PrPartPropertyM->OnDrawItem      = OnDrawMenuItem;
  PrPartDelM->OnDrawItem           = OnDrawMenuItem;
  MachInProgM->OnDrawItem          = OnDrawMenuItem;
  PrPreviewM->OnDrawItem           = OnDrawMenuItem;
  PrPrintM->OnDrawItem             = OnDrawMenuItem;
  PrPrintByPrPartM->OnDrawItem     = OnDrawMenuItem;
  PrSortByNameM->OnDrawItem        = OnDrawMenuItem;
  PrSortByTecpMapM->OnDrawItem     = OnDrawMenuItem;
  PrSortByCustomerM->OnDrawItem    = OnDrawMenuItem;
  PrSortByPartM->OnDrawItem        = OnDrawMenuItem;
  PrInfoM->OnDrawItem              = OnDrawMenuItem;
  ExitM->OnDrawItem                = OnDrawMenuItem;
  PlaneNewM->OnDrawItem            = OnDrawMenuItem;
  PlaneInfoM->OnDrawItem           = OnDrawMenuItem;
  PlanePreviewM->OnDrawItem        = OnDrawMenuItem;
  PlanePrintM->OnDrawItem          = OnDrawMenuItem;
  DTSortByFioM->OnDrawItem         = OnDrawMenuItem;
  DTSortByPressM->OnDrawItem       = OnDrawMenuItem;
  DTSortByProductNameM->OnDrawItem = OnDrawMenuItem;
  DBConnectM->OnDrawItem           = OnDrawMenuItem;
  DBDisconnectM->OnDrawItem        = OnDrawMenuItem;
  DBReconnectM->OnDrawItem         = OnDrawMenuItem;
  DateM->OnDrawItem                = OnDrawMenuItem;
  PersonalM->OnDrawItem            = OnDrawMenuItem;
  MachineM->OnDrawItem             = OnDrawMenuItem;
  ProductM->OnDrawItem             = OnDrawMenuItem;
  RawMaterialM->OnDrawItem         = OnDrawMenuItem;
  CustomerM->OnDrawItem            = OnDrawMenuItem;
  ContentsM->OnDrawItem            = OnDrawMenuItem;
  SearchOnHelpM->OnDrawItem        = OnDrawMenuItem;
  AboutM->OnDrawItem               = OnDrawMenuItem;

  PrPrintByPrPartM->Checked = bPrintByPrPart;
}

__fastcall TRubberStudio::~TRubberStudio()
{
  if (trace_PersonalID) {
    for (int i = 0; i < trace_PersonalID->Count; i++)
      delete trace_PersonalID->Items[i];
    delete trace_PersonalID;
  }
  if (trace_MachineID) {
    for (int i = 0; i < trace_MachineID->Count; i++)
      delete trace_MachineID->Items[i];
    delete trace_MachineID;
  }
  if (trace_ProductID) {
    for (int i = 0; i < trace_ProductID->Count; i++)
      delete trace_ProductID->Items[i];
    delete trace_ProductID;
  }
}

void __fastcall TRubberStudio::FormShow(TObject *Sender)
{
  doDesktopCenter(this);
  Data->OrdersDSStateChange(NULL);
  PrSortByNameMClick(PrSortByCustomerM);
  activeOrder_ID = 0;
  RefreshOrderID();
  Data->ShowProgramContentsAfterScroll(NULL);
  DTSortByFioMClick(DTSortByFioM);
}

void __fastcall TRubberStudio::FormResize(TObject *Sender)
{
  int border = GetSystemMetrics(SM_CYBORDER) * 2;
  Constraints->MinWidth  = ProductIDB->Left + ProductIDB->Width + Bevel2->Width + border + 80;
  Constraints->MinHeight = ToolBar->Height + ProgramTS->Top + PrDBGridS->Top + Bevel1->Height + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + border + 102;
  int clWidth = ClientWidth;
  int clHeight = ClientHeight;

  // Обработать контролы на странице производственной программы
  PrDBGrid->Width  = clWidth - 21;
  PrDBGridS->Width = PrDBGrid->Width;
  int buttonLeft = clWidth - 104;
  SaveB->Left   = buttonLeft;
  InsB->Left    = buttonLeft;
  DelB->Left    = buttonLeft;
  CancelB->Left = buttonLeft;
  Bevel1->Left  = clWidth - 132;
  Bevel2->Left  = clWidth - 162;

  ProductIDL->Top  = clHeight - 213;
  ProductID->Top   = clHeight - 217;
  ProductIDS->Top  = clHeight - 215;
  ProductIDB->Top  = clHeight - 213;

  CustomerIDL->Top = clHeight - 189;
  CustomerID->Top  = clHeight - 193;
  CustomerIDS->Top = clHeight - 191;
  CustomerIDB->Top = clHeight - 189;

  AmountL->Top = clHeight - 165;
  Amount->Top  = clHeight - 169;
  AmountS->Top = clHeight - 167;

  CostL->Top = clHeight - 141;
  Cost->Top  = clHeight - 145;
  CostS->Top = clHeight - 143;

  PressingOrder->Top = clHeight - 117;

  SaveB->Top   = clHeight - 205;
  InsB->Top    = clHeight - 173;
  DelB->Top    = clHeight - 141;
  CancelB->Top = clHeight - 90;
  Bevel1->Top  = clHeight - 217;
  Bevel2->Top  = clHeight - 101;

  PrDBGrid->Height  = Bevel1->Top - PrDBGrid->Top - 11;
  PrDBGridS->Height = PrDBGrid->Height;

  // Обработать контролы на странице сменно-суточного плана
  DailyTasksDBGrid->Width  = clWidth - DailyTasksDBGrid->Left - 14;
  DailyTasksDBGridS->Width = DailyTasksDBGrid->Width;
  SaveDayB->Left   = buttonLeft;
  CancelDayB->Left = buttonLeft;
  Bevel3->Left     = clWidth - 132;
  Bevel4->Left     = clWidth - 162;

  PersonalIDL->Top = clHeight - 153;
  PersonalID->Top  = clHeight - 157;
  PersonalIDS->Top = clHeight - 155;
  PersonalIDB->Top = clHeight - 153;

  AmtDoneL->Top = clHeight - 125;
  AmtDone->Top  = clHeight - 129;
  AmtDoneS->Top = clHeight - 127;

  SaveDayB->Top   = clHeight - 141;
  CancelDayB->Top = clHeight - 90;
  Bevel3->Top     = clHeight - 153;
  Bevel4->Top     = clHeight - 101;

  DataDailyTasksDBGrid->Height  = Bevel3->Top - DataDailyTasksDBGrid->Top - 17;
  DataDailyTasksDBGridS->Height = DataDailyTasksDBGrid->Height;
  DailyTasksDBGrid->Height      = DataDailyTasksDBGrid->Height;
  DailyTasksDBGridS->Height     = DataDailyTasksDBGrid->Height;
}

void __fastcall TRubberStudio::WndProc(Messages::TMessage &Message)
{
  switch (Message.Msg) {
    case WM_SETTINGCHANGE : {
      if (!PrintDialogShowed)
        Printer()->PrinterIndex = -1;
      break;
    }
  }
  TForm::WndProc(Message);
}

void __fastcall TRubberStudio::AppException(TObject *Sender, Exception *E)
{
  if (TranslateMessage(E->Message)) {
    ShowErrorMessage(E->Message, bShowStopIcon);
  } else {
    if (dynamic_cast<RubberError*>(E)) {
      ShowErrorMessage(E->Message, bShowStopIcon);
    } else {
      Application->ShowException(E);
    }
  }
  bShowStopIcon = false;
}

void TRubberStudio::UpdateCaption()
{
  if ( !Data->Programs->Active || !Data->ProgramPart->Active ) return;
  AnsiString form_caption = LoadStr(sUnknowValue);
  Data->ShowProgramPart->Close();
  Data->ShowProgramPart->ParamByName("progID")->AsInteger = activeProgram_ID;
  Data->ShowProgramPart->Prepare();
  Data->ShowProgramPart->Open();
  try {
    for (int i = PrPartSelectM->Count; i != 0; i--)
      PrPartSelectM->Delete(i-1);
    AnsiString menu_caption;
    int i = 1;
    Data->ShowProgramPart->First();
    while (!Data->ShowProgramPart->Eof) {
      TMenuItem* menu;
      menu = new TMenuItem(PrPartSelectM);
      bool part_main = Data->ShowProgramPart->FieldByName("MainPart")->AsBoolean;
      AnsiString date = Data->ShowProgramPart->FieldByName("CorrectionDate")->AsDateTime.FormatString("dd.mm.yyyy");
      if (part_main) {
        menu_caption = LoadStr(sMainProgramPart);
      } else {
        menu_caption = MiscFormat("%s (%s)", LoadStr(sNotMainProgramPart).c_str(), date.c_str());
      }
      if (Data->ShowProgramPart->FieldByName("LockPart")->AsBoolean) {
        int res_id = part_main ? sMainProgramPartComplete : sNotMainProgramPartComplete;
        menu_caption = MiscFormat("%s [%s]", menu_caption.c_str(), LoadStr(res_id).c_str());
      }
      int part_id = Data->ShowProgramPart->FieldByName("ProgramPartID")->AsInteger;
      if (part_id == activeProgramPart_ID) {
        if (part_main) {
          form_caption = LoadStr(sMainProgramPart_short);
        } else {
          form_caption = MiscFormat("%s %s", LoadStr(sNotMainProgramPart_short).c_str(), date.c_str());
        }
      }
      menu->Caption    = MiscFormat("%d. %s", i++, menu_caption.c_str());
      menu->Tag        = part_id;
      menu->RadioItem  = true;
      menu->OnClick    = PrPartSelectActiveM;
      menu->OnDrawItem = OnDrawMenuItem;
      PrPartSelectM->Add(menu);
      Data->ShowProgramPart->Next();
    }
    Data->ShowProgramPart->Close();
    Data->ProgramPartAfterScroll(NULL);
  } catch (Exception& e) {
    Data->ShowProgramPart->Close();
    throw;
  }
  Caption = MiscFormat(LoadStr(sCaption).c_str(), MiscFormat("- [ %s, %s ]", Data->GetDateValue().c_str(), form_caption.c_str()).c_str());
  if (ProgramInfoForm)
    ProgramInfoForm->UpdateDateValue();
  PrDBGrid->Repaint();
  DataDailyTasksDBGrid->Repaint();
  DailyTasksDBGrid->Repaint();
}

void TRubberStudio::RefreshStudioControls()
{
  bool bProgram    = true;
  bool bOrders     = true;
  bool bDailyTasks = true;
  bool bPrPartDel  = false;
  if ( Data ) {
    if ( Data->Programs->Active ) bProgram = !Data->Programs->IsEmpty();
    bOrders = !Data->CanPost(Data->OrdersDS);
    bDailyTasks = !Data->CanPost(Data->DailyTasksDS);
    if ( Data->ProgramPart->Active )
      bPrPartDel = !Data->ProgramPart->FieldByName("MainPart")->AsBoolean;
  }

  PrNewM->Enabled           = bOrders && bDailyTasks;
  PrNewB->Enabled           = bOrders && bDailyTasks;
  PrOpenM->Enabled          = bProgram && bOrders && bDailyTasks;
  PrOpenB->Enabled          = bProgram && bOrders && bDailyTasks;
  PrDelM->Enabled           = bProgram && bOrders;
  PrPartNewM->Enabled       = bProgram && bOrders;
  PrPartSelectM->Enabled    = bProgram && bOrders;
  PrPartPropertyM->Enabled  = bProgram && bOrders;
  PrPartDelM->Enabled       = bProgram && bOrders && bPrPartDel;
  MachInProgM->Enabled      = bProgram;
  PrPreviewM->Enabled       = bProgram && bOrders;
  PrPreviewB->Enabled       = bProgram && bOrders;
  PrPrintM->Enabled         = bProgram && bOrders;
  PrPrintB->Enabled         = bProgram && bOrders;
  PrPrintByPrPartM->Enabled = bProgram && bOrders;
  PrSortM->Enabled          = bProgram && bOrders;
  PrInfoM->Enabled          = bProgram;
  PlaneNewM->Enabled        = bProgram && bOrders && bDailyTasks;
  PlaneNewB->Enabled        = bProgram && bOrders && bDailyTasks;
  PlaneInfoM->Enabled       = bProgram;
  PlanePreviewM->Enabled    = bProgram && bDailyTasks;
  PlanePreviewB->Enabled    = bProgram && bDailyTasks;
  PlanePrintM->Enabled      = bProgram && bDailyTasks;
  PlanePrintB->Enabled      = bProgram && bDailyTasks;
  DailyTaskSortM->Enabled   = bProgram && bDailyTasks;
  DateM->Enabled            = bOrders && bDailyTasks;
  PersonalM->Enabled        = bOrders && bDailyTasks;
  MachineM->Enabled         = bOrders && bDailyTasks;
  ProductM->Enabled         = bOrders && bDailyTasks;
  RawMaterialM->Enabled     = bOrders && bDailyTasks;
  CustomerM->Enabled        = bOrders && bDailyTasks;
}

void TRubberStudio::UpdateProductControls(const bool insert_record)
{
  // Полностью отключить элементы редактирования заказа в том случае, когда:
  // 1. нет ни одного заказа (activeOrder_ID == 0)
  // 2. выбран заказ, не относящийся к текущему дополнению (active_part_flag == false)
  // 3. текущее дополнение заблокировано (lock_flag == true)
  // В любом случае НЕ отключать эти элементы, если происходит вставка новай
  // записи в таблицу заказов (insert_record). Это связано с тем, что невозможно
  // получить в SQL запросе ShowProgramContents еще не записанную в
  // таблицу Orders запись
  bool active_part_flag = false;
  if (Data->ShowProgramContents->Active)
    active_part_flag = Data->ShowProgramContents->FieldByName("ProgramPartID")->AsInteger == activeProgramPart_ID;
  bool lock_flag = Data->ProgramPart->FieldByName("LockPart")->AsBoolean;
  bool control_flag = (activeOrder_ID && active_part_flag && !lock_flag) || (insert_record);
  // Перед отключение элементов редактирования заказа запомнить значения
  // количества, стоимости за 1шт и срочности для того, чтобы, отключив эти
  // элементы от БД, суметь вывести информацию по ним на экран
  AnsiString amount = insert_record ? "0" : "";
  AnsiString cost   = insert_record ? "0" : "";
  bool pressing     = false;
  bool empty = Data->ShowProgramContents->IsEmpty();
  if (!control_flag && !empty) {
    Amount->DataSource        = Data->OrdersDS;
    Cost->DataSource          = Data->OrdersDS;
    PressingOrder->DataSource = Data->OrdersDS;
    amount   = Amount->Text;
    cost     = Cost->Text;
    pressing = PressingOrder->Checked;
  }
  ProductID->Enabled      = control_flag;
  ProductIDB->Enabled     = control_flag;
  ProductIDL->Enabled     = control_flag;
  CustomerID->Enabled     = control_flag;
  CustomerIDB->Enabled    = control_flag;
  CustomerIDL->Enabled    = control_flag;
  Amount->Enabled         = control_flag;
  AmountL->Enabled        = control_flag;
  Cost->Enabled           = control_flag;
  CostL->Enabled          = control_flag;
  PressingOrder->Enabled  = control_flag;
  Amount->DataSource        = control_flag ? Data->OrdersDS : NULL;
  Cost->DataSource          = control_flag ? Data->OrdersDS : NULL;
  PressingOrder->DataSource = control_flag ? Data->OrdersDS : NULL;
  if ( insert_record ) {
    PrNewM->Enabled           = false;
    PrNewB->Enabled           = false;
    PrOpenM->Enabled          = false;
    PrOpenB->Enabled          = false;
    PrDelM->Enabled           = false;
    PrPartNewM->Enabled       = false;
    PrPartSelectM->Enabled    = false;
    PrPartPropertyM->Enabled  = false;
    PrPartDelM->Enabled       = false;
    MachInProgM->Enabled      = false;
    PrPreviewM->Enabled       = false;
    PrPreviewB->Enabled       = false;
    PrPrintM->Enabled         = false;
    PrPrintB->Enabled         = false;
    PrPrintByPrPartM->Enabled = false;
    PrSortM->Enabled          = false;
    PrInfoM->Enabled          = false;
    PlaneNewM->Enabled        = false;
    PlaneNewB->Enabled        = false;
    PlaneInfoM->Enabled       = false;
    PlanePreviewM->Enabled    = false;
    PlanePreviewB->Enabled    = false;
    PlanePrintM->Enabled      = false;
    PlanePrintB->Enabled      = false;
    DailyTaskSortM->Enabled   = false;
    DateM->Enabled            = false;
    PersonalM->Enabled        = false;
    MachineM->Enabled         = false;
    ProductM->Enabled         = false;
    RawMaterialM->Enabled     = false;
    CustomerM->Enabled        = false;
  } else {
    RefreshStudioControls();
  }

  // Отключить кнопки добавления/удаления заказа тогда, когда выбранное
  // дополнение заблокировано
  if (lock_flag) {
    SaveB->Enabled   = false;
    InsB->Enabled    = false;
    DelB->Enabled    = false;
    CancelB->Enabled = false;
  } else {
    SaveB->Enabled   = Data->CanPost(Data->OrdersDS);
    InsB->Enabled    = !Data->Programs->IsEmpty();
    DelB->Enabled    = !Data->Orders->IsEmpty();
    CancelB->Enabled = Data->CanPost(Data->OrdersDS);
  }
  // Вывести наименование продукта
  AnsiString str;
  if (empty) {
    str = "";
  } else {
    if ( !Data->LockProduct ) {
      int product_id = Data->ShowProgramContents->FieldByName("ProductID")->AsInteger;
      TLocateOptions locate;
      locate.Clear();
      if (Data->Products->Locate("ProductID", product_id, locate)) {
        str = Data->Products->FieldByName("ProductName")->AsString;
      } else {
        str = LoadStr(sUnknowValue);
      }
    }
  }
  ProductID->Text = str;
  // Вывести наименование заказчика
  if (empty) {
    str = "";
  } else {
    int customer_id = Data->ShowProgramContents->FieldByName("CustomerID")->AsInteger;
    TLocateOptions locate;
    locate.Clear();
    if (Data->Customers->Locate("CustomerID", customer_id, locate)) {
      str = Data->Customers->FieldByName("CustomerName")->AsString;
    } else {
      str = LoadStr(sUnknowValue);
    }
  }
  CustomerID->Text = str;
  // Вывести, в случае отключения элементов редактирования заказа, остальные значения
  if ((!control_flag && !empty) || (!activeOrder_ID)) {
    Amount->Text           = amount;
    Cost->Text             = cost;
    PressingOrder->Checked = pressing;
  }
  // Если нет активного заказа, например в случае ввода нового заказа,
  // запретить доступ к PrDBGrid, чтобы не было возможности перемещаться по записям
  bool flag = activeOrder_ID;
  if (PrDBGrid->Enabled != flag) {
    PrDBGrid->Enabled = flag;
    PrDBGrid->Repaint();
  }
}

void TRubberStudio::RefreshProgramContents()
{
  if (Data->Orders->Active) {
    Data->ShowProgramContents->Close();
    Data->ShowProgramContents->ParamByName("progID")->AsInteger = activeProgram_ID;
    Data->ShowProgramContents->Prepare();
    LockOrder();
    try {
      Data->ShowProgramContents->Open();
      UnlockOrder();
    } catch (Exception& e) {
      UnlockOrder();
      throw;
    }
    RefreshOrderID();
  }
}

void TRubberStudio::RefreshOrderID()
{
  if (Data->Orders->Active) {
    if (Data->ShowProgramContents->FieldByName("OrderID")->AsInteger != activeOrder_ID) {
      TLocateOptions locate;
      locate.Clear();
      if (!Data->ShowProgramContents->Locate("OrderID", activeOrder_ID, locate)) {
        LockOrder();
        try {
          Data->ShowProgramContents->First();
          if (Data->ShowProgramContents->IsEmpty()) {
            activeOrder_ID = 0;
          } else {
            activeOrder_ID = Data->ShowProgramContents->FieldByName("OrderID")->AsInteger;
          }
          UnlockOrder();
        } catch (Exception& e) {
          UnlockOrder();
          throw;
        }
      }
    }
    UpdateProductControls();
  }
}

void TRubberStudio::DrawMenuCaption(TCanvas* canvas, AnsiString& text, bool enabled, TRect& rect, bool selected, UINT flags)
{
  canvas->Brush->Style = bsClear;
  if (!enabled) {
    if (!selected) {
      OffsetRect(&rect, 1, 1);
      canvas->Font->Color = clBtnHighlight;
      DrawText(canvas->Handle, text.c_str(), text.Length(), &rect, flags);
      OffsetRect(&rect, -1, -1);
    }
    if (selected && ColorToRGB(clHighlight) == ColorToRGB(clBtnShadow)) {
      canvas->Font->Color = clBtnHighlight;
    } else {
      canvas->Font->Color = clBtnShadow;
    }
  }
  DrawText(canvas->Handle, text.c_str(), text.Length(), &rect, flags);
}

void __fastcall TRubberStudio::OnDrawMenuItem(TObject *Sender, TCanvas *ACanvas, const TRect &ARect, bool Selected)
{
  TMenuItem* menu = (TMenuItem*)Sender;

  TRect rect = ARect;
  if (!Selected) ACanvas->FillRect(rect);
  if (menu->ImageIndex != -1) {
    TRect r;
    r.Left   = rect.Left + 1;
    r.Top    = rect.Top + 1;
    r.Right  = r.Left + Images->Width;
    r.Bottom = r.Top  + Images->Height;
    if (menu->Enabled) {
      Images->Draw(ACanvas, r.Left, r.Top, menu->ImageIndex, true);
    } else {
      DisabledImages->Draw(ACanvas, r.Left, r.Top, menu->ImageIndex, true);
    }
    r.Left--;
    r.Top--;
    r.Right  += 2;
    r.Bottom += 2;
    if (Selected) DrawEdge(ACanvas->Handle, &r, BDR_RAISEDINNER, BF_RECT);
    if (menu->Checked && !Selected) DrawEdge(ACanvas->Handle, &r, BDR_RAISEDOUTER, BF_RECT);
    rect.Left = r.Right+1;
    if (Selected) {
      ACanvas->Brush->Color = clHighlight;
      ACanvas->FillRect(rect);
    }
  } else {
    if (Selected) {
      ACanvas->Brush->Color = clHighlight;
      ACanvas->FillRect(rect);
    }
    if (menu->Checked) {
      TRect r;
#define rectLength 5
      r.Left   = rect.Left + 1 + (16 - rectLength) / 2;
      r.Top    = rect.Top  + 2 + (16 - rectLength) / 2;
      r.Right  = r.Left + rectLength;
      r.Bottom = r.Top  + rectLength;
      DrawEdge(ACanvas->Handle, &r, BDR_RAISEDOUTER, BF_RECT);
    }
    rect.Left += 19;
  }
  rect.Left += 1;
  UINT DrawStyle = DT_EXPANDTABS | DT_SINGLELINE | DT_LEFT;
  TRect r = rect;
  DrawMenuCaption(ACanvas, menu->Caption, menu->Enabled, rect, Selected, DrawStyle | DT_CALCRECT | DT_NOCLIP);
  OffsetRect(&rect, 0, ((r.Bottom - r.Top) - (rect.Bottom - rect.Top)) / 2);
  DrawMenuCaption(ACanvas, menu->Caption, menu->Enabled, rect, Selected, DrawStyle);
  if (menu->ShortCut) {
    rect.Left  = rect.Right;
    rect.Right = r.Right - 12;
    DrawMenuCaption(ACanvas, ShortCutToText(menu->ShortCut), menu->Enabled, rect, Selected, DT_RIGHT);
  }
}

void TRubberStudio::LockProgram()
{
  lockProgram_count++;
  LockProgramPart();
}

void TRubberStudio::UnlockProgram()
{
  if (lockProgram_count > 0) lockProgram_count--;
  UnlockProgramPart();
}

bool TRubberStudio::isUnlockProgram_ID()
{
  return lockProgram_count == 0;
}

void TRubberStudio::LockProgramPart()
{
  lockProgramPart_count++;
  LockOrder();
}

void TRubberStudio::UnlockProgramPart()
{
  if (lockProgramPart_count > 0) lockProgramPart_count--;
  UnlockOrder();
}

bool TRubberStudio::isUnlockProgramPart_ID()
{
  return lockProgramPart_count == 0;
}

void TRubberStudio::LockOrder()
{
  lockOrder_count++;
}

void TRubberStudio::UnlockOrder()
{
  if (lockOrder_count > 0) lockOrder_count--;
}

bool TRubberStudio::isUnlockOrder_ID()
{
  return lockOrder_count == 0;
}

void TRubberStudio::LockDailyTask()
{
  lockDailyTask_count++;
}

void TRubberStudio::UnlockDailyTask()
{
  if (lockDailyTask_count > 0) lockDailyTask_count--;
}

bool TRubberStudio::isUnlockDailyTask_ID()
{
  return lockDailyTask_count == 0;
}

void __fastcall TRubberStudio::ExitMClick(TObject *Sender)
{
  Close();
}

void __fastcall TRubberStudio::ContentsMClick(TObject *Sender)
{
  Application->HelpCommand(HELP_CONTENTS, 0);
}

void __fastcall TRubberStudio::SearchOnHelpMClick(TObject *Sender)
{
  AnsiString str = "";
  Application->HelpCommand(HELP_PARTIALKEY, (int)str.c_str());
}

void __fastcall TRubberStudio::AboutMClick(TObject *Sender)
{
  TAboutForm* about = new TAboutForm();
  try {
    about->ShowModal();
    delete about;
  } catch (Exception& e) {
    delete about;
    throw;
  }
}

// ------------------------------------------------------------------
// ---------- База данных
// ------------------------------------------------------------------
void __fastcall TRubberStudio::DateMClick(TObject *Sender)
{
  new TDateForm(NULL);
  try {
    DateForm->ShowModal();
    delete DateForm;
  } catch (Exception& e) {
    delete DateForm;
    throw;
  }
}

void __fastcall TRubberStudio::PersonalMClick(TObject *Sender)
{
  new TPersonalForm(NULL);
  try {
    PersonalForm->ShowModal();
    delete PersonalForm;
  } catch (Exception& e) {
    delete PersonalForm;
    throw;
  }
}

void __fastcall TRubberStudio::MachineMClick(TObject *Sender)
{
  new TMachineForm(NULL);
  try {
    MachineForm->ShowModal();
    delete MachineForm;
  } catch (Exception& e) {
    delete MachineForm;
    throw;
  }
}

void __fastcall TRubberStudio::ProductMClick(TObject *Sender)
{
  new TProductForm(NULL);
  try {
    ProductForm->ShowModal();
    delete ProductForm;
  } catch (Exception& e) {
    delete ProductForm;
    throw;
  }
}

void __fastcall TRubberStudio::RawMaterialMClick(TObject *Sender)
{
  new TRawMaterialForm(NULL);
  try {
    RawMaterialForm->ShowModal();
    delete RawMaterialForm;
  } catch (Exception& e) {
    delete RawMaterialForm;
    throw;
  }
}

void __fastcall TRubberStudio::CustomerMClick(TObject *Sender)
{
  new TCustomerForm(NULL);
  try {
    CustomerForm->ShowModal();
    delete CustomerForm;
  } catch (Exception& e) {
    delete CustomerForm;
    throw;
  }
}

// ------------------------------------------------------------------
// ---------- Производственная программа
// ------------------------------------------------------------------
void __fastcall TRubberStudio::PrNewMClick(TObject *Sender)
{
  LockProgram();
  try {
    bool firstProgram = Data->Programs->IsEmpty();
    Data->Programs->Append();
    Data->Programs->FieldByName("CompanyName")->AsString = LoadStr(sCompanyName);

    new TProgramInfoForm(NULL);
    try {
      ProgramInfoForm->firstProgram = firstProgram;
      ProgramInfoForm->Caption = LoadStr(sProgramCreate);
      switch ( ProgramInfoForm->ShowModal() ) {
        case mrOk    :
          Data->ProgramPart->Append();
          Data->ProgramPart->FieldByName("CorrectionDate")->AsDateTime = TDateTime::CurrentDate();
          Data->ProgramPart->Post();
          UnlockProgram();
          Data->ProgramsAfterScroll(NULL);
          Data->PlanInProg->Append();
          Data->PlanInProg->FieldByName("ProgramID")->AsInteger = activeProgram_ID;
          Data->PlanInProg->Post();
          UpdateCaption();
          break;
        case mrCancel: UnlockProgram(); break;
      }
      delete ProgramInfoForm;
    } catch (Exception& e) {
      delete ProgramInfoForm;
      throw;
    }
  } catch (Exception& e) {
    UnlockProgram();
    throw;
  }
}

void __fastcall TRubberStudio::PrOpenMClick(TObject *Sender)
{
  LockProgram();
  TSelectProgramForm* form = new TSelectProgramForm(NULL);
  form->bDesktopCenter = false;
  try {
    TToolButton* button = dynamic_cast<TToolButton*>(Sender);
    if (button) {
      TPoint point;
      point.x = 0;
      point.y = button->Height;
      point = button->ClientToScreen(point);
      form->Left = point.x;
      form->Top  = point.y;
    } else {
      form->Position = poDesktopCenter;
    }
    void* bookmark = Data->Programs->GetBookmark();
    switch (form->ShowModal()) {
      case mrOk    : UnlockProgram();
                     Data->ProgramsAfterScroll(NULL);
                     Data->ShowProgramContentsAfterScroll(NULL);
                     UpdateCaption();
                     break;
      case mrCancel: Data->Programs->GotoBookmark(bookmark);
                     UnlockProgram();
                     break;
    }
    delete form;
  } catch (Exception& e) {
    UnlockProgram();
    delete form;
    throw;
  }
}

void __fastcall TRubberStudio::PrDelMClick(TObject *Sender)
{
  if (ShowQueryMessage(sProgramDelete, MB_YESNO) == IDYES) {
    Data->Programs->Delete();
    UpdateProductControls();
  }
}

void __fastcall TRubberStudio::MachInProgMClick(TObject *Sender)
{
  new TMachineInProgForm(NULL);
  try {
    MachineInProgForm->ShowModal();
    delete MachineInProgForm;
  } catch (Exception& e) {
    delete MachineInProgForm;
    throw;
  }
}

void __fastcall TRubberStudio::PrPreviewMClick(TObject *Sender)
{
  LockProgram();
  TCursor oldcur = Screen->Cursor;
  TPlProgramReport* report = NULL;
  try {
    CalculateAllPrice();
    Screen->Cursor = crHourGlass;
    report = new TPlProgramReport(MiscFormat(LoadStr(sProgramTitle).c_str(), Data->GetDateValue().c_str()));
    report->Preview();
    delete report;
    Screen->Cursor = oldcur;
    UnlockProgram();
    RefreshOrderID();
  } catch (Exception& e) {
    delete report;
    Screen->Cursor = oldcur;
    UnlockProgram();
    RefreshOrderID();
    throw;
  }
}

void __fastcall TRubberStudio::PrPrintMClick(TObject *Sender)
{
  LockProgram();
  TCursor oldcur = Screen->Cursor;
  TPlProgramReport* report = NULL;
  try {
    CalculateAllPrice();
    Screen->Cursor = crHourGlass;
    report = new TPlProgramReport(MiscFormat(LoadStr(sProgramTitle).c_str(), Data->GetDateValue().c_str()));
    report->Print(true);
    delete report;
    Screen->Cursor = oldcur;
    UnlockProgram();
    RefreshOrderID();
  } catch (Exception& e) {
    delete report;
    Screen->Cursor = oldcur;
    UnlockProgram();
    RefreshOrderID();
    throw;
  }
}

void __fastcall TRubberStudio::PrPrintBClick(TObject *Sender)
{
  LockProgram();
  TCursor oldcur = Screen->Cursor;
  TPlProgramReport* report = NULL;
  try {
    CalculateAllPrice();
    Screen->Cursor = crHourGlass;
    report = new TPlProgramReport(MiscFormat(LoadStr(sProgramTitle).c_str(), Data->GetDateValue().c_str()));
    report->Print(false);
    delete report;
    Screen->Cursor = oldcur;
    UnlockProgram();
    RefreshOrderID();
  } catch (Exception& e) {
    delete report;
    Screen->Cursor = oldcur;
    UnlockProgram();
    RefreshOrderID();
    throw;
  }
}

void __fastcall TRubberStudio::PrPrintByPrPartMClick(TObject *Sender)
{
  PrPrintByPrPartM->Checked = !PrPrintByPrPartM->Checked;
  bPrintByPrPart = PrPrintByPrPartM->Checked;
}

bool TRubberStudio::isPrintByPrPart()
{
  return bPrintByPrPart && PrSortByPartM->Checked;
}

void __fastcall TRubberStudio::PrSortByNameMClick(TObject *Sender)
{
  TMenuItem* menu = dynamic_cast<TMenuItem*>(Sender);
  if (menu) {
    ProgramContents_sortBy = (TProgramContentsSortBy)menu->Tag;
    menu->Checked = true;
  }
  AnsiString str = PrSortByPartM->Checked ? "ProgramPart.MainPart DESC, ProgramPart.CorrectionDate, " : "";
  switch (ProgramContents_sortBy) {
    case sb_Name    : str += "Products.ProductName, Products.TechMap, Customers.CustomerName;"; break;
    case sb_TechMap : str += "Products.TechMap, Products.ProductName, Customers.CustomerName;"; break;
    case sb_Customer: str += "Customers.CustomerName, Products.ProductName, Products.TechMap;"; break;
  }
  Data->ShowProgramContents->SQL->Strings[12] = "ORDER BY " + str;
  LockProgramPart();
  try {
    RefreshProgramContents();
    UnlockProgramPart();
  } catch (Exception& e) {
    UnlockProgramPart();
    throw;
  }
}

void __fastcall TRubberStudio::PrSortByPartMClick(TObject *Sender)
{
  PrSortByPartM->Checked = !PrSortByPartM->Checked;
  PrPrintByPrPartM->Enabled = PrSortByPartM->Checked;
  PrSortByNameMClick(NULL);
}

void __fastcall TRubberStudio::PrInfoMClick(TObject *Sender)
{
  new TProgramInfoForm(NULL);
  try {
    ProgramInfoForm->ShowModal();
    delete ProgramInfoForm;
  } catch (Exception& e) {
    delete ProgramInfoForm;
    throw;
  }
}

// ------------------------------------------------------------------
// ---------- Дополнение к производственной программе
// ------------------------------------------------------------------
void __fastcall TRubberStudio::PrPartNewMClick(TObject *Sender)
{
  Data->ProgramPart->Append();
  Data->ProgramPart->FieldByName("MainPart")->AsBoolean        = false;
  Data->ProgramPart->FieldByName("CorrectionDate")->AsDateTime = TDateTime::CurrentDate();
  Data->ProgramPart->Post();
  TProgramPartForm* form = new TProgramPartForm(NULL);
  try {
    if (form->ShowModal() == mrCancel) Data->ProgramPart->Delete();
    UpdateCaption();
    delete form;
  } catch (Exception& e) {
    delete form;
    throw;
  }
}

void __fastcall TRubberStudio::PrPartSelectActiveM(TObject *Sender)
{
  int part_id = ((TMenuItem*)Sender)->Tag;
  TLocateOptions locate;
  locate.Clear();
  Data->ProgramPart->Locate("ProgramPartID", part_id, locate);
  UpdateCaption();
}

void __fastcall TRubberStudio::PrPartPropertyMClick(TObject *Sender)
{
  LockProgramPart();
  TProgramPartForm* form = new TProgramPartForm(NULL);
  try {
    form->ShowModal();
    UnlockProgramPart();
    UpdateCaption();
    UpdateProductControls();
    delete form;
  } catch (Exception& e) {
    UnlockProgramPart();
    UpdateCaption();
    UpdateProductControls();
    delete form;
    throw;
  }
}

void __fastcall TRubberStudio::PrPartDelMClick(TObject *Sender)
{
  AnsiString str = MiscFormat(LoadStr(sProgramPartDelete).c_str(), Data->ProgramPart->FieldByName("CorrectionDate")->AsDateTime.FormatString("dd.mm.yyyy").c_str());
  if (ShowQueryMessage(str, MB_YESNO) == IDYES) {
    Data->ProgramPart->Delete();
    UpdateCaption();
    UpdateProductControls();
  }
}

// ------------------------------------------------------------------
// ---------- Заказы
// ------------------------------------------------------------------
void __fastcall TRubberStudio::SaveBClick(TObject *Sender)
{
  LockOrder();
  try {
    if (!Data->Orders->FieldByName("ProgramPartID")->AsInteger)
      Data->Orders->FieldByName("ProgramPartID")->AsInteger = activeProgramPart_ID;
    Data->Orders->Post();
    activeOrder_ID = Data->Orders->FieldByName("OrderID")->AsInteger;
    UnlockOrder();
    RefreshProgramContents();
  } catch (Exception& e) {
    UnlockOrder();
    throw;
  }
}

void __fastcall TRubberStudio::InsBClick(TObject *Sender)
{
  if (Data->CanPost(Data->OrdersDS)) {
    if (!Data->Orders->FieldByName("ProgramPartID")->AsInteger)
      Data->Orders->FieldByName("ProgramPartID")->AsInteger = activeProgramPart_ID;
    Data->Orders->Post();
  }
  activeOrder_ID = 0;
  Data->Orders->Append();
  Data->Orders->FieldByName("ProgramPartID")->AsInteger = activeProgramPart_ID;
  UpdateProductControls(true);
  ProductID->Text  = LoadStr(sUnknowValue);
  CustomerID->Text = LoadStr(sUnknowValue);
  ProductID->SetFocus();
}

void __fastcall TRubberStudio::DelBClick(TObject *Sender)
{
  if (ShowQueryMessage(sRecordDelete, MB_YESNO) == IDYES) {
    Data->Orders->Delete();
    Data->OrdersDSStateChange(NULL);
  }
}

void __fastcall TRubberStudio::CancelBClick(TObject *Sender)
{
  Data->Orders->Cancel();
}

void __fastcall TRubberStudio::PrDBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  TCanvas* canvas = PrDBGrid->Canvas;
  bool focus = PrDBGrid->Focused();
  if (activeOrder_ID &&
      Data->ShowProgramContents->FieldByName("ProgramPartID")->AsInteger != activeProgramPart_ID &&
      ((focus && !State.Contains(gdSelected)) || (!focus)))
    canvas->Brush->Color = clBtnFace;
  AnsiString str = LoadStr(sUnknowValue);
  switch (Column->ID) {
    // Прессформа
    case 2: {
      if (Data->LocateProduct()) {
        str = MiscFormat("%d/%d  ", Data->Products->FieldByName("PFNumber")->AsInteger, Data->Products->FieldByName("PFPlaces")->AsInteger);
        if (Data->Products->FieldByName("PFCylindric")->AsBoolean) {
          TRect rect = Rect;
          canvas->FillRect(rect);
          rect.left += 2;
          rect.top  += 2;
          canvas->TextOut(rect.left, rect.top, str);
          rect.left += canvas->TextWidth(str);
          str = '\306';
          AnsiString fontname = canvas->Font->Name;
          canvas->Font->Name = "Symbol";
          canvas->TextOut(rect.left, rect.top, str);
          rect.left += canvas->TextWidth(str) + 1;
          canvas->Font->Name = fontname;
          str = MiscFormat("%dx%d", Data->Products->FieldByName("PFDiameter")->AsInteger, Data->Products->FieldByName("PFHeight")->AsInteger);
          canvas->TextOut(rect.left, rect.top, str);
          return;
        } else {
          str = MiscFormat("%s%dx%dx%d", str.c_str(), Data->Products->FieldByName("PFLength")->AsInteger, Data->Products->FieldByName("PFWidth")->AsInteger, Data->Products->FieldByName("PFHeight")->AsInteger);
        }
      }
      break;
    }
    // Срочность
    case 5: {
      int res_id = Column->Field->AsBoolean ? sYesValueUpperCase : sNoValueLowerCase;
      str = LoadStr(res_id);
      break;
    }
    // Марка резины
    case 6: {
      if (Data->LocateProduct())
        str = Data->Products->FieldByName("RubName")->AsString;
      break;
    }
    // Вес детали
    case 7: {
      if (Data->LocateProduct())
        str = Data->Products->FieldByName("Weight")->AsFloat;
      break;
    }
    // Вес заготовки
    case 8: {
      if (Data->LocateProduct())
        str = Data->Products->FieldByName("ZagWeight")->AsFloat;
      break;
    }
    // Количествово резины
    case 9: {
      if (Data->LocateProduct())
        str = Data->Products->FieldByName("ZagWeight")->AsFloat / 1000.0 * (double)Column->Field->AsInteger;
      break;
    }
    // Время вулканизации
    case 10: {
      if (Data->LocateProduct())
        str = Data->Products->FieldByName("VulcanTime")->AsFloat;
      break;
    }
    // Время загрузки
    case 11: {
      if (Data->LocateProduct())
        str = Data->Products->FieldByName("LoadTime")->AsFloat;
      break;
    }
    // Норма времени на вулканизацию
    case 12: {
      if (Data->LocateProduct())
        str = RoundDoubleToAnsiString(480.0 / Data->Products->FieldByName("VulcanNorma")->AsFloat);
      break;
    }
    // Расценка на вулканизацию
    case 13: {
      if (Data->LocateProduct()) {
        double norma = Data->Products->FieldByName("VulcanNorma")->AsFloat;
        double koef;
        if (Data->ShowProgramContents->FieldByName("Vulcan5Class")->AsBoolean) {
          koef = Data->ShowProgramContents->FieldByName("VulcanCostTariff5")->AsFloat;
        } else {
          koef = Column->Field->AsFloat;
        }
        str = RoundDoubleToAnsiString(koef * 8.0 / norma, 3);
      }
      break;
    }
    // Норма времени на обрезку
    case 14: {
      if (Data->LocateProduct())
        str = RoundDoubleToAnsiString(480.0 / Data->Products->FieldByName("CutNorma")->AsFloat);
      break;
    }
    // Расценка на обрезку
    case 15: {
      if (Data->LocateProduct()) {
        double norma = Data->Products->FieldByName("CutNorma")->AsFloat;
        double koef  = Column->Field->AsFloat;
        str = RoundDoubleToAnsiString(koef * 8.0 / norma, 3);
      }
      break;
    }
    // Время вулканизации по всему объему выпуска
    case 16: {
      if (Data->LocateProduct())
        str = RoundDoubleToAnsiString(RoundDouble(480.0 / Data->Products->FieldByName("VulcanNorma")->AsFloat) / 60.0 * (double)Column->Field->AsInteger);
      break;
    }
    // З/плата за вулкагизацию по всему объему выпуска
    case 17: {
      if (Data->LocateProduct()) {
        double norma = Data->Products->FieldByName("VulcanNorma")->AsFloat;
        double koef;
        if (Data->ShowProgramContents->FieldByName("Vulcan5Class")->AsBoolean) {
          koef = Column->Field->AsFloat;
        } else {
          koef = Data->ShowProgramContents->FieldByName("VulcanCostTariff4")->AsFloat;
        }
        str = RoundDoubleToAnsiString(RoundDouble(koef * 8.0 / norma, 3) * (double)Data->ShowProgramContents->FieldByName("Amount")->AsInteger);
      }
      break;
    }
    // Время обрезки по всему объему выпуска
    case 18: {
      if (Data->LocateProduct())
        str = RoundDoubleToAnsiString(RoundDouble(480.0 / Data->Products->FieldByName("CutNorma")->AsFloat) / 60.0 * (double)Column->Field->AsInteger);
      break;
    }
    // З/плата за обрезку по всему объему выпуска
    case 19: {
      if (Data->LocateProduct()) {
        double norma = Data->Products->FieldByName("CutNorma")->AsFloat;
        double koef  = Column->Field->AsFloat;
        str = RoundDoubleToAnsiString(RoundDouble(koef * 8.0 / norma, 3) * (double)Data->ShowProgramContents->FieldByName("Amount")->AsInteger);
      }
      break;
    }
    // Стоимость
    case 21: {
      if (Data->LocateProduct())
        str = RoundDoubleToAnsiString(Data->ShowProgramContents->FieldByName("Cost")->AsFloat * (double)Column->Field->AsInteger);
      break;
    }
    // Остальные поля таблицы
    default: str = Column->Field->AsString; break;
  }
  canvas->FillRect(Rect);
  canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
}

void __fastcall TRubberStudio::ProductIDBClick(TObject *Sender)
{
  new TSelectProductForm(NULL);
  SelectProductForm->bDesktopCenter = false;
  try {
    TPoint p;
    p.x = 0;
    p.y = ProductIDB->Height + 4;
    p = ProductIDB->ClientToScreen(p);
    SelectProductForm->Left = p.x;
    SelectProductForm->Top  = p.y;
    if (SelectProductForm->ShowModal() == mrOk) {
      Data->Orders->Edit();
      Data->Orders->FieldByName("ProductID")->AsInteger = Data->Products->FieldByName("ProductID")->AsInteger;
      TField* cost = Data->Orders->FieldByName("Cost");
      if (cost->AsFloat == 0.0)
        cost->AsFloat = Data->Products->FieldByName("CostForOrder")->AsFloat;
      ProductID->Text = Data->Products->FieldByName("ProductName")->AsString;
      ProductID->SetFocus();
    }
    delete SelectProductForm;
  } catch (Exception& e) {
    delete SelectProductForm;
    throw;
  }
}

void __fastcall TRubberStudio::CustomerIDBClick(TObject *Sender)
{
  TSelectCustomerForm* form = new TSelectCustomerForm(NULL);
  form->bDesktopCenter = false;
  try {
    TPoint p;
    p.x = 0;
    p.y = CustomerIDB->Height + 4;
    p = CustomerIDB->ClientToScreen(p);
    form->Left = p.x;
    form->Top  = p.y;
    if (form->ShowModal() == mrOk) {
      Data->Orders->Edit();
      Data->Orders->FieldByName("CustomerID")->AsInteger = Data->Customers->FieldByName("CustomerID")->AsInteger;
      CustomerID->Text = Data->Customers->FieldByName("CustomerName")->AsString;
      CustomerID->SetFocus();
    }
    delete form;
  } catch (Exception& e) {
    delete form;
    throw;
  }
}

void TRubberStudio::CalculateAllPrice()
{
  Data->ShowProgramContents->DisableControls();
  vulcanProgramTime    = 0.0;
  vulcanProgramPrice   = 0.0;
  cutProgramTime       = 0.0;
  cutProgramPrice      = 0.0;
  customerProgramPrice = 0.0;
  activeOrder_ID = Data->ShowProgramContents->FieldByName("OrderID")->AsInteger;
  try {
    if (Data->CanPost(Data->OrdersDS))
      Data->Orders->Post();
    Data->ShowProgramContents->First();
    while (!Data->ShowProgramContents->Eof) {
      if (Data->LocateProduct(Data->ShowProgramContents->FieldByName("ProductID")->AsInteger)) {
        double amount      = (double)Data->ShowProgramContents->FieldByName("Amount")->AsInteger;
        double vulcanNorma = Data->Products->FieldByName("VulcanNorma")->AsFloat;
        double cutNorma    = Data->Products->FieldByName("CutNorma")->AsFloat;
        vulcanProgramTime += RoundDouble(RoundDouble(480.0 / vulcanNorma) / 60.0 * amount);
        cutProgramTime    += RoundDouble(RoundDouble(480.0 / cutNorma) / 60.0 * amount);
        double koef;
        int date_id = Data->Programs->FieldByName("DateID")->AsInteger;
        bool flag = true;
        if (Data->DateTable->FieldByName("DateID")->AsInteger != date_id) {
          TLocateOptions locate;
          locate.Clear();
          flag = Data->DateTable->Locate("DateID", date_id, locate);
        }
        if (flag) {
          if (Data->Products->FieldByName("Vulcan5Class")->AsBoolean) {
            koef = Data->DateTable->FieldByName("VulcanCostTariff5")->AsFloat;
          } else {
            koef = Data->DateTable->FieldByName("VulcanCostTariff4")->AsFloat;
          }
          vulcanProgramPrice += RoundDouble(RoundDouble(koef * 8.0 / vulcanNorma, 3) * amount);
          koef = Data->DateTable->FieldByName("CutCostTariff")->AsFloat;
          cutProgramPrice += RoundDouble(RoundDouble(koef * 8.0 / cutNorma, 3) * amount);
        }
        customerProgramPrice += RoundDouble(Data->ShowProgramContents->FieldByName("Cost")->AsFloat * amount);
      }
      Data->ShowProgramContents->Next();
    }
  } catch (Exception& e) {
    Data->ShowProgramContents->EnableControls();
    throw;
  }

  TLocateOptions locate;
  locate.Clear();
  Data->ShowProgramContents->Locate("OrderID", activeOrder_ID, locate);

  Data->ShowProgramContents->EnableControls();
}

// ------------------------------------------------------------------
// ---------- Сменно-суточные задания
// ------------------------------------------------------------------
void __fastcall TRubberStudio::PlaneNewMClick(TObject *Sender)
{
  if (!DirExists(modelDir.c_str())) CreateDir(modelDir);
  SetCurrentDir(modelDir);

  new TSplashForm(NULL);
  Enabled = false;
  SplashForm->Show();
  try {
    BuildModels();

    if ( !trace_flag ) {
      STARTUPINFO si;
      PROCESS_INFORMATION pi;
      ZeroMemory(&si, sizeof(STARTUPINFO));
      si.cb          = sizeof(STARTUPINFO);
      si.wShowWindow = SW_SHOWDEFAULT;
      si.dwFlags     = STARTF_USESHOWWINDOW;
      unsigned long result = 0;

      AnsiString command_line = MiscFormat("\"%srdosim.exe\" \"%srub_gen.smr\"", GetNormalDirFromName(Application->ExeName).c_str(), modelDir.c_str());
      if ( plane_precision ) {
        if (!CreateProcess(NULL, command_line.c_str(), NULL, NULL, true, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
          throw RubberError(sErrorCreateProcess);
        ShowWindow(Application->Handle, SW_SHOWMINIMIZED);
        WaitForSingleObject(pi.hProcess, INFINITE);
        ShowWindow(Application->Handle, SW_NORMAL);
        GetExitCodeProcess(pi.hProcess, &result);
        if (result == 4) throw RubberError(sErrorRDORun);
      }

      SplashForm->SetFocus();

      ConnectModels();

      command_line = MiscFormat("\"%srdosim.exe\" \"%srubber.smr\"", GetNormalDirFromName(Application->ExeName).c_str(), modelDir.c_str());
      if (!CreateProcess(NULL, command_line.c_str(), NULL, NULL, true, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
        throw RubberError(sErrorCreateProcess);
      ShowWindow(Application->Handle, SW_SHOWMINIMIZED);
      WaitForSingleObject(pi.hProcess, INFINITE);
      ShowWindow(Application->Handle, SW_NORMAL);
      GetExitCodeProcess(pi.hProcess, &result);
      if (result == 4) throw RubberError(sErrorRDORun);
    }

    SplashForm->SetFocus();

    BuildDailyTasks();

    SplashForm->ProgressBar->Position = 100;
    Application->ProcessMessages();
    delete SplashForm;
    Enabled = true;
  } catch (Exception& e) {
    delete SplashForm;
    Enabled = true;
    bShowStopIcon = true;
    throw RubberError(MiscFormat("%s:\r\n%s", LoadStr(sErrorBuildDailyTasks).c_str(), e.Message.c_str()));
  }
}

void TRubberStudio::BuildModels()
{
  // Определить дату, на которую необходимо произвести расчет сменно-суточного задания
  Data->GetFirstCalcSmena->Close();
  Data->GetFirstCalcSmena->ParamByName("PlanInProgID")->AsInteger = Data->PlanInProg->FieldByName("PlanInProgID")->AsInteger;
  Data->GetFirstCalcSmena->Prepare();
  Data->GetFirstCalcSmena->Open();
  Data->GetFirstCalcSmena->First();
  CalcDailyTasks_begin_smena = 0;
  try {
    if (!Data->GetFirstCalcSmena->Eof)
      CalcDailyTasks_begin_smena = Data->GetFirstCalcSmena->FieldByName("Smena")->AsInteger;
    Data->GetFirstCalcSmena->Close();
  } catch (Exception &e) {
    Data->GetFirstCalcSmena->Close();
    throw;
  }
  if (CalcDailyTasks_begin_smena) {
    if ((CalcDailyTasks_begin_smena - 1) % 3) CalcDailyTasks_begin_smena--;
    if ((CalcDailyTasks_begin_smena - 1) % 3) CalcDailyTasks_begin_smena--;
    CalcDailyTasks_begin_smena += 3;
  } else {
    CalcDailyTasks_begin_smena = 1;
  }
  if (SplashForm) {
    TDateTime date;
    date = Data->DateTable->FieldByName("DateValue")->AsDateTime;
    SmenaToDate(CalcDailyTasks_begin_smena, date);
    AnsiString str = date.FormatString("dd mmm yyyy года");
    SplashForm->Caption = MiscFormat(SplashForm->Caption.c_str(), str.c_str());
    Application->ProcessMessages();
  }

  Amount_sum     = 0;
  VulcanTime_max = 0.0;
  LoadTime_max   = 0.0;

#define fileCount_gen 6
#define fileCount 4
  if ( !trace_flag ) {
    int res_gen[fileCount_gen] = {PAT_GEN_MODEL, OPR_GEN_MODEL, FRM_GEN_MODEL, FUN_GEN_MODEL, SMR_GEN_MODEL, PMD_GEN_MODEL};
    char* fileName_gen[fileCount_gen] = {"rub_gen.pat", "rub_gen.opr", "rub_gen.frm", "rub_gen.fun", "rub_gen.smr", "rub_gen.pmd"};
    for (int i = 0; i < fileCount_gen; i++) {
      TResourceStream* file = new TResourceStream(0, res_gen[i], RT_RCDATA);
      file->SaveToFile(fileName_gen[i]);
      delete file;
    }
    int res[fileCount] = {PAT_MODEL, OPR_MODEL, FRM_MODEL, SMR_MODEL};
    char* fileName[fileCount] = {"rubber.pat", "rubber.opr", "rubber.frm", "rubber.smr"};
    for (int i = 0; i < fileCount; i++) {
      TResourceStream* file = new TResourceStream(0, res[i], RT_RCDATA);
      file->SaveToFile(fileName[i]);
      delete file;
    }
  }
#undef fileCount_gen
#undef fileCount

  TStringList* model_strings   = new TStringList();
  TStringList* machine_rss     = new TStringList();
  TStringList* machine_pmd     = new TStringList();
  TStringList* product_gen_rss = new TStringList();
  TStringList* product_rss     = new TStringList();
  TStringList* press_form_rss  = new TStringList();

  if (trace_PersonalID) {
    for (int i = 0; i < trace_PersonalID->Count; i++)
      delete trace_PersonalID->Items[i];
    delete trace_PersonalID;
  }
  trace_PersonalID = new TList();

  if (trace_MachineID) {
    for (int i = 0; i < trace_MachineID->Count; i++)
      delete trace_MachineID->Items[i];
    delete trace_MachineID;
  }
  trace_MachineID = new TList();

  if (trace_ProductID) {
    for (int i = 0; i < trace_ProductID->Count; i++)
      delete trace_ProductID->Items[i];
    delete trace_ProductID;
  }
  trace_ProductID = new TList();

  TList* group_list = new TList();

  typedef struct {
    int       product_db_id;
    double    vulcan_time;
    int       vulcan_temperature;
    int       height;
    short int pressing;
    int       group_id;
  } TGroup;

  try {
    // Подготовить данные в файлы *.rtp, *.rss и *.pmd по прессам
    Data->Model_RES_Machine->Close();
    Data->Model_RES_Machine->ParamByName("progID")->AsInteger = activeProgram_ID;
    Data->Model_RES_Machine->Prepare();
    Data->Model_RES_Machine->Open();
    if (Data->Model_RES_Machine->IsEmpty()) {
      Data->Model_RES_Machine->Close();
      throw RubberError(sErrorNoMachineInProg);
    }
    int machine_model_id = 0;
    machine_pmd->Add("$Results");

    int floor_height_max_h = 0;
    int plate_width_max_h  = 0;
    int floor_height_max_w = 0;
    int plate_width_max_w  = 0;
    int pf_height_max_h = 0;
    int pf_width_max_h  = 0;
    AnsiString pf_height_max_prod_name_h = "";
    int pf_height_max_w = 0;
    int pf_width_max_w  = 0;
    AnsiString pf_height_max_prod_name_w = "";


    int floors_count = 0;
    Data->Model_RES_Machine->First();
    while (!Data->Model_RES_Machine->Eof) {
      machine_model_id++;
      TTraceMachineID* trace_machine_id = new TTraceMachineID;
      trace_machine_id->model_id = machine_model_id;
      trace_machine_id->db_id    = Data->Model_RES_Machine->FieldByName("MachineID")->AsInteger;
      trace_MachineID->Add(trace_machine_id);
      AnsiString press_name = Data->Model_RES_Machine->FieldByName("MachineName")->AsString;

      double pressure = Data->Model_RES_Machine->FieldByName("Pressure")->AsFloat;
      if ( pressure == 0.0 ) throw RubberError( MiscFormat(LoadStr(sErrorBadMachine).c_str(), press_name.c_str(), LoadStr(sErrorBadMachinePressure).c_str()) );

      double effort = Data->Model_RES_Machine->FieldByName("Effort")->AsFloat;
      if ( effort == 0.0 ) throw RubberError( MiscFormat(LoadStr(sErrorBadMachine).c_str(), press_name.c_str(), LoadStr(sErrorBadMachineEffort).c_str()) );

      int floors = Data->Model_RES_Machine->FieldByName("Floors")->AsInteger;
      if ( floors == 0 ) throw RubberError( MiscFormat(LoadStr(sErrorBadMachine).c_str(), press_name.c_str(), LoadStr(sErrorBadMachineFloors).c_str()) );

      int floor_height = Data->Model_RES_Machine->FieldByName("FloorHeight")->AsInteger;
      if ( floor_height == 0 ) throw RubberError( MiscFormat(LoadStr(sErrorBadMachine).c_str(), press_name.c_str(), LoadStr(sErrorBadMachineFloorHeight).c_str()) );

      int load_sides = Data->Model_RES_Machine->FieldByName("LoadSides")->AsInteger;
      if ( load_sides == 0 ) throw RubberError( MiscFormat(LoadStr(sErrorBadMachine).c_str(), press_name.c_str(), LoadStr(sErrorBadMachineLoadSides).c_str()) );

      // ---- PlateLength не используется в моделе !!!
      // if ( Data->Model_RES_Machine->FieldByName("PlateLength")->AsInteger == 0 )
      //   throw RubberError( MiscFormat(LoadStr(sErrorBadMachine).c_str(), press_name.c_str(), LoadStr(sErrorBadMachinePlateLength).c_str()) );

      int plate_width  = Data->Model_RES_Machine->FieldByName("PlateWidth")->AsInteger;
      if ( plate_width == 0 ) throw RubberError( MiscFormat(LoadStr(sErrorBadMachine).c_str(), press_name.c_str(), LoadStr(sErrorBadMachinePlateWidth).c_str()) );

      double koef = pressure / effort / 2;

      press_name.Insert(MiscFormat("Пр_%d_", machine_model_id), 1);
      for (int i = 1; i <= press_name.Length(); i++) {
        char c = press_name[i];
        if (!((c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'z') ||
              (c >= 'A' && c <= 'Z') ||
              (c >= 'а' && c <= 'я') ||
              (c >= 'А' && c <= 'Я')))
          press_name[i] = '_';
      }
      if (machine_model_id == 1) {
        model_strings->Add(MiscFormat("  Наименование : (%s,", press_name.c_str()));
      } else {
        model_strings->Add(MiscFormat("                  %s,", press_name.c_str()));
      }

      // Определить два пресса (один с максимальной высотой этажа, другой -
      // с максимальной шириной плиты) для будущей проверки с параметрами прессформ
      if ( floor_height >= floor_height_max_h ) {
        floor_height_max_h = floor_height;
        plate_width_max_h  = plate_width;
      }
      if ( plate_width >= plate_width_max_w ) {
        floor_height_max_w = floor_height;
        plate_width_max_w  = plate_width;
      }
      for (int i = 1; i <= floors; i++) {
        floors_count++;
        AnsiString p_name = MiscFormat("Пресс_%d_%d", machine_model_id, i);
        machine_rss->Add(MiscFormat("  %s : Прессы no_trace %d %d %d %d %d %d * * %s * * * * * %1.2f %1.2f * * * *",
          p_name.c_str(),
          machine_model_id, (int)effort, i, floor_height, plate_width,
          load_sides,
          press_name.c_str(),
          koef * 2, koef)
        );
        AnsiString p_pmd_name = MiscFormat("Пресс_%d", machine_model_id);
        if (i == 1) {
          machine_pmd->Add(MiscFormat("  %s_Занят : watch_state %s.Состояние = Работа or %s.Состояние = Загрузка or", p_pmd_name.c_str(), p_name.c_str(), p_name.c_str()));
        } else {
          AnsiString s = AnsiString::StringOfChar(' ', p_pmd_name.Length() + 22);
          machine_pmd->Add(MiscFormat("%s %s.Состояние = Работа or %s.Состояние = Загрузка or", s.c_str(), p_name.c_str(), p_name.c_str()));
        }
      }
      AnsiString s = machine_pmd->Strings[machine_pmd->Count-1];
      s.Delete(s.Length()-2, 3);
      machine_pmd->Strings[machine_pmd->Count-1] = s;

      Data->Model_RES_Machine->Next();
    }
    Data->Model_RES_Machine->Close();

    TFileStream* file = NULL;
    TFileStream* file_gen = NULL;
    
    if ( !trace_flag ) {
      // Записать *.rtp
      TFileStream* file     = new TFileStream("rubber.rtp", fmCreate);
      TFileStream* file_gen = new TFileStream("rub_gen.rtp", fmCreate);
      try {
        SaveResourceToStream(RTP1_MODEL, file);
        SaveResourceToStream(RTP1_GEN_MODEL, file_gen);
        AnsiString s = MiscFormat("  Номер        : integer[1..%d]\r\n", machine_model_id);
        file->Write(s.c_str(), s.Length());
        file_gen->Write(s.c_str(), s.Length());
        SaveResourceToStream(RTP2_MODEL, file);
        SaveResourceToStream(RTP2_GEN_MODEL, file_gen);
        s = model_strings->Strings[model_strings->Count-1];
        s.Delete(s.Length(), 1);
        s += ")";
        model_strings->Strings[model_strings->Count-1] = s;
        model_strings->SaveToStream(file);
        model_strings->SaveToStream(file_gen);
        model_strings->Clear();
        SaveResourceToStream(RTP3_MODEL, file);
        SaveResourceToStream(RTP3_GEN_MODEL, file_gen);
        delete file;
        delete file_gen;
      } catch (Exception& e) {
        delete file;
        delete file_gen;
        throw;
      }
    }
    // Подготовить данные в файл *.rss по изделиям и прессформам
    Data->Model_RES_Product->Close();
    Data->Model_RES_Product->ParamByName("progID")->AsInteger = activeProgram_ID;
    Data->Model_RES_Product->Prepare();
    Data->Model_RES_Product->Open();

    // Произвести группировку изделий по времени вулканизции,
    // температуре вулканизации, высоте прессформы и срочности заказа
    // Определить максимадьное значение времени вылканизации (суммарный объем
    // выпуска и максимадьное значение времен загрузки подсчитываются ниже)
    Data->Model_RES_Product->First();
    while (!Data->Model_RES_Product->Eof) {
      if (Data->Model_RES_Product->FieldByName("AmountSum")->AsInteger) {
        TGroup* group = new TGroup;
        group->product_db_id      = Data->Model_RES_Product->FieldByName("ProductID")->AsInteger;
        group->vulcan_time        = Data->Model_RES_Product->FieldByName("VulcanTime")->AsFloat;
        group->vulcan_temperature = Data->Model_RES_Product->FieldByName("VulcanTemperature")->AsInteger;
        group->height             = Data->Model_RES_Product->FieldByName("PFHeight")->AsInteger;
        group->pressing           = Data->Model_RES_Product->FieldByName("PressingOrder")->AsBoolean ? 1 : 0;
        group->group_id           = 0;
        group_list->Add(group);
        if (VulcanTime_max < group->vulcan_time) VulcanTime_max = group->vulcan_time;

        AnsiString product_name = Data->Model_RES_Product->FieldByName("ProductName")->AsString;

        if ( Data->Model_RES_Product->FieldByName("LoadTime")->AsFloat == 0.0 )
          throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductLoadTime).c_str()) );

        if ( group->vulcan_time == 0 )
          throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductVulcanTime).c_str()) );

        if ( group->vulcan_temperature == 0 )
          throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductVulcanTemperature).c_str()) );

        if ( Data->Model_RES_Product->FieldByName("PFPressureOn100")->AsInteger == 0 )
          throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductPFPressureOn100).c_str()) );

        if ( Data->Model_RES_Product->FieldByName("PFNumber")->AsInteger == 0 )
          throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductPFNumber).c_str()) );

        if ( Data->Model_RES_Product->FieldByName("PFPlaces")->AsInteger == 0 )
          throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductPFPlaces).c_str()) );

        if ( group->height == 0 )
          throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductPFHeight).c_str()) );

        if ( Data->Model_RES_Product->FieldByName("PFCylindric")->AsBoolean ) {
          if ( Data->Model_RES_Product->FieldByName("PFDiameter")->AsInteger == 0 )
            throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductPFDiameter).c_str()) );

        } else {
          if ( Data->Model_RES_Product->FieldByName("PFLength")->AsInteger == 0 )
            throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductPFLength).c_str()) );

          if ( Data->Model_RES_Product->FieldByName("PFWidth")->AsInteger == 0 )
            throw RubberError( MiscFormat(LoadStr(sErrorBadProduct).c_str(), product_name.c_str(), LoadStr(sErrorBadProductPFWidth).c_str()) );
        }
      }
      Data->Model_RES_Product->Next();
    }
    int group_id = 1;
    for (int i = 0; i < group_list->Count; i++) {
      TGroup* group_1 = (TGroup*)group_list->Items[i];
      if (!group_1->group_id) {
        for (int j = 0; j < group_list->Count; j++) {
          TGroup* group_2 = (TGroup*)group_list->Items[j];
          if (group_1->vulcan_time        == group_2->vulcan_time &&
              group_1->vulcan_temperature == group_2->vulcan_temperature &&
              group_1->height             == group_2->height &&
              group_1->pressing           == group_2->pressing) {
            group_1->group_id = group_2->group_id = group_id;
          }
        }
        group_id++;
      }
    }
    // Непосредственная генерация данных по изделиям и прессформам
    int product_model_id    = 1;
    int press_form_model_id = 1;
    Data->Model_RES_Product->First();

    Data->GetAmtDone->Close();
    Data->GetAmtDone->ParamByName("PlanInProgID")->AsInteger = Data->PlanInProg->FieldByName("PlanInProgID")->AsInteger;
    Data->GetAmtDone->ParamByName("smena_id")->AsInteger     = CalcDailyTasks_begin_smena;
    Data->GetAmtDone->Prepare();
    Data->GetAmtDone->Open();
    TLocateOptions locate;
    locate.Clear();

    bool orders = false;

    while (!Data->Model_RES_Product->Eof) {

      if (Data->Model_RES_Product->FieldByName("AmountSum")->AsInteger) {

        TGroup* group = NULL;
        int product_db_id = Data->Model_RES_Product->FieldByName("ProductID")->AsInteger;

        double amount    = Data->Model_RES_Product->FieldByName("AmountSum")->AsFloat;
        int pf_number    = Data->Model_RES_Product->FieldByName("PFNumber")->AsInteger;
        int pf_places    = Data->Model_RES_Product->FieldByName("PFPlaces")->AsInteger;
        int pf_pressure  = Data->Model_RES_Product->FieldByName("PFPressureOn100")->AsInteger;
        double load_time = Data->Model_RES_Product->FieldByName("LoadTime")->AsFloat;

        // Учесть уже обработанный объем
        if (Data->GetAmtDone->Locate("ProductID", product_db_id, locate)) {
          amount -= Data->GetAmtDone->FieldByName("AmtDone")->AsFloat;
          if (amount < 0) amount = 0;
        }

        if (amount) {
          orders = true;
          // Подчитать суммарный объем выпуска и максимадьное значение времен загрузки
          // (максимадьное значение времени вылканизации подсчивыается выше)
          Amount_sum += amount;
          if (LoadTime_max < load_time) LoadTime_max = load_time;

          TTraceProductID* trace_product_id = new TTraceProductID;
          trace_product_id->model_id  = product_model_id;
          trace_product_id->db_id     = product_db_id;
          trace_product_id->last      = (int)amount;
          trace_product_id->koef_load_time_plases = load_time / pf_places;

          trace_ProductID->Add(trace_product_id);

          for (int i = 0; i < group_list->Count; i++) {
            group = (TGroup*)group_list->Items[i];
            if (group->product_db_id == product_db_id) {
              trace_product_id->tech_group_index = group->group_id;
              break;
            }
          }

          product_gen_rss->Add(MiscFormat("  Изделие_%d : Изделия no_trace %d %d %1.1f %1.1f %d %d * %d %d %d %1.1f",
            product_model_id,
            product_model_id, group->group_id, amount, amount, pf_number, pf_places, pf_pressure, group->vulcan_temperature, group->pressing, amount)
          );
          product_rss->Add(MiscFormat("  Изделие_%d : Изделия trace %d %d %1.1f %1.1f %d %d * %d %d %d * * * %1.1f *",
            product_model_id,
            product_model_id, group->group_id, amount, amount, pf_number, pf_places, pf_pressure, group->vulcan_temperature, group->pressing, amount)
          );
          int length;
          int width;
          if (Data->Model_RES_Product->FieldByName("PFCylindric")->AsBoolean) {
            length = Data->Model_RES_Product->FieldByName("PFDiameter")->AsInteger;
            width  = length;
          } else {
            length = Data->Model_RES_Product->FieldByName("PFLength")->AsInteger;
            width  = Data->Model_RES_Product->FieldByName("PFWidth")->AsInteger;
          }
          // Определить максимальную высоту и ширину прессформы для будущей проверки с
          // параметрами выбранного оборудования
          int max_height = group->height;
          int max_width  = length > width ? length : width;
          if ( pf_height_max_h < max_height ) {
            pf_height_max_h = max_height;
            pf_width_max_h  = max_width;
            pf_height_max_prod_name_h = Data->Model_RES_Product->FieldByName("ProductName")->AsString;
          }
          if ( pf_width_max_w < max_width ) {
            pf_height_max_w = max_height;
            pf_width_max_w  = max_width;
            pf_height_max_prod_name_w = Data->Model_RES_Product->FieldByName("ProductName")->AsString;
          }
          for (int i = 0; i < pf_number; i++) {
            press_form_rss->Add(MiscFormat("  Пресс_форма_%d : Пресс_формы no_trace %d * %d %d %d %d %d %d %1.1f %1.1f * * * %d %d",
              press_form_model_id,
              press_form_model_id, product_model_id, group->group_id, length, width, group->height, pf_places, group->vulcan_time,
              load_time, pf_pressure, group->vulcan_temperature)
            );
            press_form_model_id++;
          }
          product_model_id++;
        }
      }
      Data->Model_RES_Product->Next();
    }
    Data->Model_RES_Product->Close();
    Data->GetAmtDone->Close();
    if (!orders) throw RubberError(sErrorNoOrders);
    // Проверить возможность обработки изделий на выбранном оборудовании по
    // характеристикам высоты этажа и ширины плиты прессов
    if ( pf_height_max_h > floor_height_max_h || pf_width_max_h > plate_width_max_h )
      throw RubberError( MiscFormat( LoadStr(sErrorBadMachineInProg).c_str(), pf_height_max_prod_name_h.c_str()) );
    if ( pf_height_max_w > floor_height_max_w || pf_width_max_w > plate_width_max_w )
      throw RubberError( MiscFormat( LoadStr(sErrorBadMachineInProg).c_str(), pf_height_max_prod_name_w.c_str()) );

    if ( !trace_flag ) {
      // Записать *.rss
      file     = new TFileStream("rubber.rss", fmCreate);
      file_gen = new TFileStream("rub_gen.rss", fmCreate);
      try {
        model_strings->Add("$Resources\r\n");
        model_strings->Add("  Система : тип_Система no_trace 0 20 0.7 0.06 Инициализация 0 * * * * * * * * * 1 0 0 0 0 0 0 0.0 * *");
        model_strings->Add( MiscFormat("  Систем  : Система1    no_trace * * * * * * * * * * * * %1.2f %d %1.7f\r\n", ((double)(100 - plane_precision)) / 100.0, floors_count, sqrt(fill_koef) ));
        model_strings->SaveToStream(file_gen);
        model_strings->Clear();
        model_strings->Add("$Resources");
        model_strings->Add( MiscFormat("  Систем : Система no_trace * * * * * * * * * * %1.7f\r\n", sqrt(fill_koef) ));
        model_strings->SaveToStream(file);
        model_strings->Clear();
        machine_rss->Add("");
        machine_rss->SaveToStream(file);
        machine_rss->SaveToStream(file_gen);
        delete machine_rss; machine_rss = NULL;
        product_gen_rss->Add("");
        product_gen_rss->SaveToStream(file_gen);
        product_rss->Add("");
        product_rss->SaveToStream(file);
        delete product_gen_rss; product_gen_rss = NULL;
        delete product_rss; product_rss = NULL;
        press_form_rss->Add("");
        press_form_rss->Add("$End");
        press_form_rss->SaveToStream(file);
        press_form_rss->SaveToStream(file_gen);
        delete press_form_rss; press_form_rss = NULL;
        delete file;
        delete file_gen;
      } catch (Exception& e) {
        delete file;
        delete file_gen;
        throw;
      }
      // Записать *.pmd
      file = new TFileStream("rubber.pmd", fmCreate);
      try {
        machine_pmd->Add("");
        machine_pmd->SaveToStream(file);
        delete machine_pmd; machine_pmd = NULL;
        SaveResourceToStream(PMD_MODEL, file);
        delete file;
      } catch (Exception& e) {
        delete file;
        throw;
      }
    }

    delete model_strings; model_strings = NULL;
    for (int i = 0; i < group_list->Count; i++)
      delete group_list->Items[i];
    delete group_list;
    if (SplashForm) {
      SplashForm->ProgressBar->Position = 2;
      Application->ProcessMessages();
    }
  } catch (Exception& e) {
    Data->Model_RES_Machine->Close();
    Data->Model_RES_Product->Close();
    Data->GetAmtDone->Close();
    delete model_strings;
    delete machine_rss;
    delete machine_pmd;
    delete product_gen_rss;
    delete product_rss;
    delete press_form_rss;
    for (int i = 0; i < group_list->Count; i++)
      delete group_list->Items[i];
    delete group_list;
    if (trace_PersonalID) {
      for (int i = 0; i < trace_PersonalID->Count; i++)
        delete trace_PersonalID->Items[i];
      delete trace_PersonalID; trace_PersonalID = NULL;
    }
    if (trace_MachineID) {
      for (int i = 0; i < trace_MachineID->Count; i++)
        delete trace_MachineID->Items[i];
      delete trace_MachineID; trace_MachineID = NULL;
    }
    if (trace_ProductID) {
      for (int i = 0; i < trace_ProductID->Count; i++)
        delete trace_ProductID->Items[i];
      delete trace_ProductID; trace_ProductID = NULL;
    }
    throw RubberError(MiscFormat("%s:\r\n%s", LoadStr(sErrorBuildModel).c_str(), e.Message.c_str()));
  }
}

void TRubberStudio::ConnectModels()
{
  if (SplashForm) {
    SplashForm->ProgressBar->Position = 34;
    Application->ProcessMessages();
  }

  double k1 = 0.0;
  double k2 = 0.0;
  double k3 = 0.0;
  double k4 = 0.0;

  if ( plane_precision == 0 ) {

    k1 = plane_k1;
    k2 = plane_k2;
    k3 = plane_k3;
    k4 = plane_k4;

  } else {
    ImportPlaneKoef( k1, k2, k3, k4 );
  }

  if ( Amount_sum == 0.0 || VulcanTime_max == 0.0 || LoadTime_max == 0.0 ) throw RubberError(sErrorBuildModelData);
  k2 /= Amount_sum;
  k3 /= VulcanTime_max;
  k4 /= LoadTime_max;

  TStringList* model_strings  = new TStringList();
  try {
    if ( !trace_flag ) {
      // Записать *.fun
      TFileStream* file = new TFileStream("rubber.fun", fmCreate);
      try {
        model_strings->Add("$Constant");
        model_strings->Add(MiscFormat(" Параметр_1 : real = %2.7f", k1));
        model_strings->Add(MiscFormat(" Параметр_2 : real = %2.7f", k2));
        model_strings->Add(MiscFormat(" Параметр_3 : real = %2.7f", k3));
        model_strings->Add(MiscFormat(" Параметр_4 : real = %2.7f", k4));
        model_strings->SaveToStream(file);
        model_strings->Clear();
        SaveResourceToStream(FUN_MODEL, file);
        delete file;
      } catch (Exception& e) {
        delete file;
        throw;
      }
    }
    if (SplashForm) {
      SplashForm->ProgressBar->Position = 35;
      Application->ProcessMessages();
    }
    delete model_strings; model_strings = NULL;
  } catch (Exception& e) {
    delete model_strings;
    throw RubberError(sErrorBuildModel);
  }
}

void TRubberStudio::BuildDailyTasks()
{
  if (SplashForm) {
    SplashForm->ProgressBar->Position = 45;
    Application->ProcessMessages();
  }

  TStringList* string_list = NULL;
  TList* product_list      = NULL;
  TList* personal_list     = NULL;

  TFileStream* file = NULL;
  TStringList* log  = NULL;

  try {
    if (!trace_PersonalID || !trace_MachineID || !trace_ProductID) throw RubberError(sErrorBuildModelData);
    SetCurrentDir(modelDir);

    typedef struct {
      int db_id;
      int amount;
      int tech_group_index;
      int manometr;
      int smena;
      int machine_db_id;
      int pf;
      int pf_count;
      int personal_db_id;
      int group;
      double model_time;
      double koef_load_time_plases;
    } TTraceProduct;

    // Подсчитать объем выпуска (для последующего вывода)
    int amount_in_program = 0;
    if ( log_flag || trace_flag ) {
      for (int i = 0; i < trace_ProductID->Count; i++)
        amount_in_program += ((TTraceProductID*)trace_ProductID->Items[i])->last;
    }

    // Парсинг файла трассировки
    string_list  = new TStringList();
    product_list = new TList();

#define product_model_time          2
#define product_resource_type_index 3
#define product_model_id_index      5
#define product_manometr_index     12
#define product_smena_index        15
#define machine_model_id_index     16
#define pf_model_id_index          17
#define product_amount_index       18
#define can_trace                  19

    try {
      string_list->LoadFromFile("rubber.trc");
    } catch (Exception& e) {
      throw RubberError(sErrorOpenTraceFile);
    }
    if (SplashForm) {
      SplashForm->ProgressBar->Position = 50;
      Application->ProcessMessages();
    }
    int count = string_list->Count;
    for (int index = 0; index < count; index++) {
      AnsiString str = string_list->Strings[index];
      str = str.Trim();
      if (!str.Length()) continue;
      // Изменение ресурса изделия
      if (str.Pos("RK ") && PickOutInt(str, can_trace) && PickOutInt(str, product_resource_type_index) == 3 && PickOutInt(str, product_amount_index) != -1) {
        TTraceProduct* product = new TTraceProduct;
        product->group          = 0;
        product->pf_count       = 0;
        product->personal_db_id = -1;
        // Момент окончания обработки
        product->model_time = PickOutDouble(str, product_model_time);
        // Номер изделия
        int product_model_id = PickOutInt(str, product_model_id_index);
        // Перевод номера изделия из модельного в тот, который ему соответствует в базе данных
        TTraceProductID* trace_product_id;
        for (int i = 0; i < trace_ProductID->Count; i++) {
          trace_product_id = (TTraceProductID*)trace_ProductID->Items[i];
          if (trace_product_id->model_id == product_model_id) {
            product->db_id                 = trace_product_id->db_id;
            product->tech_group_index      = trace_product_id->tech_group_index;
            product->koef_load_time_plases = trace_product_id->koef_load_time_plases;
            break;
          }
        }
        // Кол-во изготовленных изделий определенного наименования за последний временной интервал
        product->amount = trace_product_id->last - PickOutInt(str, product_amount_index);
        trace_product_id->last -= product->amount;
        // Манометрическое давление
        product->manometr = PickOutInt(str, product_manometr_index);
        // Номер смены
        product->smena = PickOutInt(str, product_smena_index);
        // Номер пресса
        int machine_model_id = PickOutInt(str, machine_model_id_index);
        // Перевод номера пресса из модельного в тот, который ему соответствует в базе данных
        for (int i = 0; i < trace_MachineID->Count; i++) {
          TTraceMachineID* trace_machine_id = (TTraceMachineID*)trace_MachineID->Items[i];
          if (trace_machine_id->model_id == machine_model_id) {
            product->machine_db_id = trace_machine_id->db_id;
            break;
          }
        }
        // Номер прессформы
        product->pf = PickOutInt(str, pf_model_id_index);

        product_list->Add(product);
      }
      if (SplashForm) {
        SplashForm->ProgressBar->Position = ((double)index / count) * 30 + 50;
        Application->ProcessMessages();
      }
    }
    delete string_list; string_list = NULL;
    count = product_list->Count;

    if ( log_flag || trace_flag ) {
      // Подготовить лог для вывода информации
      file = log_flag ? new TFileStream("rubber.log", fmCreate) : new TFileStream("trace.log", fmCreate);
      log = new TStringList();
      // Вывести оборудование
      log->Add("");
      log->Add("Оборудование");
      for (int i = 0; i < trace_MachineID->Count; i++) {
        TTraceMachineID* trace_machine_id = (TTraceMachineID*)trace_MachineID->Items[i];
        log->Add(MiscFormat("model_id = %d, db_id = %d", trace_machine_id->model_id, trace_machine_id->db_id));
      }
      // Вывести выпускаемую номенклатуру
      log->Add("");
      log->Add("Изделия");
      for (int i = 0; i < trace_ProductID->Count; i++) {
        TTraceProductID* trace_product_id = (TTraceProductID*)trace_ProductID->Items[i];
        log->Add(MiscFormat("model_id = %d, db_id = %d", trace_product_id->model_id, trace_product_id->db_id));
      }
      // Вывести еще несгруппированные изделия
      log->Add("");
      log->Add("Файл трассировки");
      for (int i = 0; i < count; i++) {
        TTraceProduct* product = (TTraceProduct*)product_list->Items[i];
        log->Add(MiscFormat("db_id = %d, amount = %d, machine_id = %d, manometr = %d, smena = %d, model_time = %1.1f, pf = %d",
          product->db_id, product->amount, product->machine_db_id, product->manometr, product->smena, product->model_time, product->pf));
      }
    }
    // 1. Сгруппировать изделия, которые могут обрабатывается на одном станке
    //    в течении одной смены при одних технологических характеристиках (рабочий не учитывается)
    // 2. Просуммировать объем обработки одного изделия на одном станке
    //    в течении одной смены (рабочий не учитывается)
    int group = 1;
    bool change_group = false;
    for (int i = 0; i < count; i++) {
      TTraceProduct* product1 = (TTraceProduct*)product_list->Items[i];
      int sum = product1->amount;
      int pf_count = 1;
      if (sum) {
        if (!product1->group) {
          product1->group = group;
          change_group = true;
        }
        for (int j = i+1; j < count; j++) {
          TTraceProduct* product2 = (TTraceProduct*)product_list->Items[j];
          if (product2->amount &&
              product1->smena         == product2->smena &&
              product1->machine_db_id == product2->machine_db_id)
          {
            if (!product2->group &&
                product1->tech_group_index == product2->tech_group_index &&
                product1->manometr         == product2->manometr)
            {
              product2->group = product1->group;
            }
            if (product1->db_id == product2->db_id) {
              sum += product2->amount;
              product2->amount = 0;
              if (product1->model_time == product2->model_time) pf_count++;
            }
          }
        }
        product1->amount = sum;
        if (change_group) {
          group++;
          change_group = false;
        }
      }
      if ( product1->pf_count < pf_count ) product1->pf_count = pf_count;
      if (SplashForm) {
        SplashForm->ProgressBar->Position = ((double)i / count) * 5 + 80;
        Application->ProcessMessages();
      }
    }
    // Распределить рабочих по оборудованию, с учетом времени загрузки прессформ
    int index_begin = 0;
    int index_end;
    int smena_long  = 1;
    bool do_repeate = true;
    int max_personal_index = -1;
    do {
      int smena_short = smena_long;
      int work_day = 1 + (smena_short-1) / 3;
      smena_short -= (work_day - 1) * 3;
      double smena_time = long_smena_flag ? ( smena_short % 3 ? 480.0 : 420.0 ) :
                                            ( smena_short % 3 ? 440.0 : 380.0 );
      // Определить диапазон индексов для заданной смены
      index_end = product_list->Count;
      do_repeate = false;
      for (int i = index_begin; i < index_end; i++) {
        TTraceProduct* product = (TTraceProduct*)product_list->Items[i];
        if (product->smena != smena_long) {
          index_end = i;
          do_repeate = true;
          break;
        }
      }

      typedef struct {
        double work_time;
      } TPersonal_item;

      personal_list = new TList();

      // Проверить все оборудование в диапазоне индексов
      for (int i = 0; i < trace_MachineID->Count; i++) {
        TTraceMachineID* trace_machine_id = (TTraceMachineID*)trace_MachineID->Items[i];
        for (int j = index_begin; j < index_end; j++) {
          TTraceProduct* product = (TTraceProduct*)product_list->Items[j];
          if (product->machine_db_id == trace_machine_id->db_id) {
            if (product->amount && product->personal_db_id == -1) {
              // Выявить наименее загруженного прессовщика
              int personal_index = -1;
              double min_work_time = smena_time;
              for (int k = 0; k < personal_list->Count; k++) {
                TPersonal_item* personal = (TPersonal_item*)personal_list->Items[k];
                if (personal->work_time < min_work_time) {
                  min_work_time  = personal->work_time;
                  personal_index = k;
                }
              }
              // Запрещает выбирать уже задействованого прессовщика, даже если
              // у него есть свободное время, в том случае, если за ним появился
              // хотя бы еще один. Будет создан новый прессовщик
              // Другими словами - выбираем или последнего, или создаем нового
              if (new_personal_flag)
                if (personal_index < personal_list->Count-1) personal_index = -1;
              // Подсчтать время обработки ГРУППЫ изделий
              double need_work_time = product->amount * product->koef_load_time_plases;
              for (int k = index_begin; k < index_end; k++) {
                if (k != j) {
                  TTraceProduct* product_2 = (TTraceProduct*)product_list->Items[k];
                  if (product_2->group == product->group &&
                      product_2->amount &&
                      product_2->machine_db_id == trace_machine_id->db_id &&
                      product_2->personal_db_id == -1) {
                    need_work_time += product_2->amount * product_2->koef_load_time_plases;
                  }
                }
              }
              // Создать, если надо, нового прессовщика
              double free_work_time = 0.0;
              if (personal_index != -1)
                 free_work_time = smena_time - ((TPersonal_item*)personal_list->Items[personal_index])->work_time;
              if ((personal_index != -1 && free_work_time < need_work_time) ||
                  personal_index == -1)
              {
                TPersonal_item* personal = new TPersonal_item;
                personal->work_time = 0.0;
                personal_index      = personal_list->Count;
                personal_list->Add(personal);
              }
              // Назначить прессовщика на пресс для обработки данной ГРУППЫ изделий
              ((TPersonal_item*)personal_list->Items[personal_index])->work_time += need_work_time;
              product->personal_db_id = personal_index;
              for (int k = index_begin; k < index_end; k++) {
                if (k != j) {
                  TTraceProduct* product_2 = (TTraceProduct*)product_list->Items[k];
                  if (product_2->group == product->group &&
                      product_2->amount &&
                      product_2->machine_db_id == trace_machine_id->db_id &&
                      product_2->personal_db_id == -1) {
                    product_2->personal_db_id = personal_index;
                  }
                }
              }
              // Определить максимальное кол-во прессовщиков
              if (personal_index > max_personal_index)
                max_personal_index = personal_index;
            }
          }
        }
      }

      for (int i = 0; i < personal_list->Count; i++)
        delete personal_list->Items[i];
      delete personal_list; personal_list = NULL;

      smena_long++;
      index_begin = index_end;

      if (index_end == product_list->Count) do_repeate = false;

    } while (do_repeate);

    // Перевод номера прессовщика из модельного в тот, который ему соответствует
    // в базе данных виртуальных прессовщиков. Если виртуального прессовщика не
    // существует, его необходимо создать

    typedef struct {
      int index;
      int db_index;
    } TVirtualPersonal;

    personal_list = new TList();

    for (int i = 0; i <= max_personal_index; i++) {
      TVirtualPersonal* personal = new TVirtualPersonal;
      personal->index = i;
      AnsiString pressor_name   = MiscFormat("Прессовщик №%d", i+1);
      AnsiString pressor_office = "Прессовщик";
      Data->Virtual_Personal->Close();
      Data->Virtual_Personal->ParamByName("lastname")->AsString = pressor_name;
      Data->Virtual_Personal->ParamByName("office")->AsString   = pressor_office;
      Data->Virtual_Personal->Prepare();
      Data->Virtual_Personal->Open();
      Data->Virtual_Personal->First();
      if (Data->Virtual_Personal->IsEmpty()) {
        Data->LockPersonal = true;
        Data->Personal->Append();
        Data->Personal->FieldByName("LastName")->AsString   = pressor_name;
        Data->Personal->FieldByName("FirstName")->AsString  = "";
        Data->Personal->FieldByName("SecondName")->AsString = "";
        Data->Personal->FieldByName("Office")->AsString     = pressor_office;
        Data->Personal->FieldByName("Vulcan5Class")->AsBoolean = true;
        Data->Personal->Post();
        personal->db_index = Data->Personal->FieldByName("PersonalID")->AsInteger;
        Data->LockPersonal = false;
      } else {
        personal->db_index = Data->Virtual_Personal->FieldByName("PersonalID")->AsInteger;
      }
      personal_list->Add(personal);
      Data->Virtual_Personal->Close();
    }

    for (int i = 0; i < count; i++) {
      TTraceProduct* product = (TTraceProduct*)product_list->Items[i];
      if (product->amount) {
        for (int j = 0; j < personal_list->Count; j++) {
          TVirtualPersonal* personal = (TVirtualPersonal*)personal_list->Items[j];
          if (product->personal_db_id == personal->index) {
            product->personal_db_id = personal->db_index;
            break;
          }
        }
      }
    }

    if (SplashForm) {
      SplashForm->ProgressBar->Position = 90;
      Application->ProcessMessages();
    }

    if ( log_flag || trace_flag ) {
      // Подсчитать объем выпущенных изделий и вывести сгруппированные изделия
      log->Add("");
      log->Add("Результат");
      int amount_sun = 0;
      for (int i = 0; i < count; i++) {
        TTraceProduct* product = (TTraceProduct*)product_list->Items[i];
        if (product->amount) {
          amount_sun += product->amount;
          log->Add(MiscFormat("id = %d, amount = %d, machine_id = %d, manometr = %d, smena = %d, pf_count = %d, personal = %d, group = %d",
            product->db_id, product->amount, product->machine_db_id, product->manometr, product->smena, product->pf_count, product->personal_db_id, product->group));
        }
      }
      // Вывести остаток и подсчитать суммарный остаток
      log->Add("");
      log->Add("Остаток по изделиям");
      int last_in_program = 0;
      for (int i = 0; i < trace_ProductID->Count; i++) {
        TTraceProductID* trace_product_id = (TTraceProductID*)trace_ProductID->Items[i];
        last_in_program += trace_product_id->last;
        log->Add(MiscFormat("model_id = %d, db_id = %d, остаток = %d", trace_product_id->model_id, trace_product_id->db_id, trace_product_id->last));
      }
      // Вывести итоги
      log->Add("");
      log->Add("Итого");
      log->Add(MiscFormat("Всего в про-во, шт : %d", amount_in_program));
      log->Add(MiscFormat("Обработано, шт     : %d", amount_sun));
      log->Add(MiscFormat("Остаток, шт        : %d", last_in_program));
      log->Add("");
      log->SaveToStream(file);
      delete file; file = NULL;
      delete log;  log = NULL;
    }
    // Обновить базу данных по сменно-суточным заданиям
    if ( !trace_flag ) {
      LockDailyTask();
      try {
        Data->DailyTasks->First();
        while (!Data->DailyTasks->Eof) {
          if (Data->DailyTasks->FieldByName("Smena")->AsInteger >= CalcDailyTasks_begin_smena) {
            Data->DailyTasks->Delete();
          } else {
            Data->DailyTasks->Next();
          }
        }

        int plan_in_prog = Data->PlanInProg->FieldByName("PlanInProgID")->AsInteger;
        for (int i = 0; i < count; i++) {
          TTraceProduct* product = (TTraceProduct*)product_list->Items[i];
          if (product->amount) {
            Data->DailyTasks->Append();
            Data->DailyTasks->FieldByName("PlanInProgID")->AsInteger   = plan_in_prog;
            Data->DailyTasks->FieldByName("Smena")->AsInteger          = product->smena + CalcDailyTasks_begin_smena - 1;
            Data->DailyTasks->FieldByName("PersonalID")->AsInteger     = product->personal_db_id;
            Data->DailyTasks->FieldByName("ProductID")->AsInteger      = product->db_id;
            Data->DailyTasks->FieldByName("MachineID")->AsInteger      = product->machine_db_id;
            Data->DailyTasks->FieldByName("AmtToDo")->AsInteger        = product->amount;
            Data->DailyTasks->FieldByName("ProductGroup")->AsInteger   = product->group;
            Data->DailyTasks->FieldByName("VulcanPressure")->AsInteger = product->manometr;
            Data->DailyTasks->FieldByName("PFCount")->AsInteger        = product->pf_count;
            Data->DailyTasks->Post();
          }
          if (SplashForm) {
            SplashForm->ProgressBar->Position = ((double)i / count) * 10 + 90;
            Application->ProcessMessages();
          }
        }
        Data->ShowDailyTasksDate->Close();
        UnlockDailyTask();
        RefreshDailyTasks();
        TLocateOptions locate;
        locate.Clear();
        Data->ShowDailyTasksDate->Locate("WorkDay", 1 + (CalcDailyTasks_begin_smena - 1) / 3, locate);
      } catch (Exception& e) {
        UnlockDailyTask();
        throw;
      }
    }

    for (int i = 0; i < product_list->Count; i++)
      delete product_list->Items[i];
    delete product_list; product_list = NULL;
    for (int i = 0; i < personal_list->Count; i++)
      delete personal_list->Items[i];
    delete personal_list; personal_list = NULL;
    if (trace_PersonalID) {
      for (int i = 0; i < trace_PersonalID->Count; i++)
        delete trace_PersonalID->Items[i];
      delete trace_PersonalID; trace_PersonalID = NULL;
    }
    if (trace_MachineID) {
      for (int i = 0; i < trace_MachineID->Count; i++)
        delete trace_MachineID->Items[i];
      delete trace_MachineID; trace_MachineID = NULL;
    }
    if (trace_ProductID) {
      for (int i = 0; i < trace_ProductID->Count; i++)
        delete trace_ProductID->Items[i];
      delete trace_ProductID; trace_ProductID = NULL;
    }
  } catch (Exception& e) {
    Data->Virtual_Personal->Close();
    Data->LockPersonal = false;
    delete string_list;
    if (product_list) {
      for (int i = 0; i < product_list->Count; i++)
        delete product_list->Items[i];
      delete product_list;
    }
    if (personal_list) {
      for (int i = 0; i < personal_list->Count; i++)
        delete personal_list->Items[i];
      delete personal_list;
    }
    if (trace_PersonalID) {
      for (int i = 0; i < trace_PersonalID->Count; i++)
        delete trace_PersonalID->Items[i];
      delete trace_PersonalID; trace_PersonalID = NULL;
    }
    if (trace_MachineID) {
      for (int i = 0; i < trace_MachineID->Count; i++)
        delete trace_MachineID->Items[i];
      delete trace_MachineID; trace_MachineID = NULL;
    }
    if (trace_ProductID) {
      for (int i = 0; i < trace_ProductID->Count; i++)
        delete trace_ProductID->Items[i];
      delete trace_ProductID; trace_ProductID = NULL;
    }
    delete file;
    delete log;
    throw;
  }
}

void __fastcall TRubberStudio::PlanePreviewMClick(TObject *Sender)
{
  TCursor oldcur = Screen->Cursor;
  TPlDailyTasksReport* report = NULL;
  try {
    Screen->Cursor = crHourGlass;
    report = new TPlDailyTasksReport(Data->ShowDailyTasksDate->FieldByName("WorkDay")->AsInteger, Data->PlanInProg->FieldByName("PlanName")->AsString);
    report->Preview();
    delete report;
    Screen->Cursor = oldcur;
  } catch (Exception& e) {
    Screen->Cursor = oldcur;
    delete report;
    throw;
  }
}

void __fastcall TRubberStudio::PlanePrintMClick(TObject *Sender)
{
  TCursor oldcur = Screen->Cursor;
  TPlDailyTasksReport* report = NULL;
  try {
    Screen->Cursor = crHourGlass;
    report = new TPlDailyTasksReport(Data->ShowDailyTasksDate->FieldByName("WorkDay")->AsInteger, Data->PlanInProg->FieldByName("PlanName")->AsString);
    report->Print(true);
    delete report;
    Screen->Cursor = oldcur;
  } catch (Exception& e) {
    Screen->Cursor = oldcur;
    delete report;
    throw;
  }
}

void __fastcall TRubberStudio::PlanePrintBClick(TObject *Sender)
{
  TCursor oldcur = Screen->Cursor;
  TPlDailyTasksReport* report = NULL;
  try {
    Screen->Cursor = crHourGlass;
    report = new TPlDailyTasksReport(Data->ShowDailyTasksDate->FieldByName("WorkDay")->AsInteger, Data->PlanInProg->FieldByName("PlanName")->AsString);
    report->Print(false);
    delete report;
    Screen->Cursor = oldcur;
  } catch (Exception& e) {
    Screen->Cursor = oldcur;
    delete report;
    throw;
  }
}

void __fastcall TRubberStudio::DTSortByFioMClick(TObject *Sender)
{
  TMenuItem* menu = dynamic_cast<TMenuItem*>(Sender);
  if (menu) {
    DailyTasks_sortBy = (TDailyTasksSortBy)menu->Tag;
    menu->Checked = true;
  }
  AnsiString str = "ORDER BY DailyTasks.Smena";
  switch (DailyTasks_sortBy) {
    case sb_Fio        : str += ", Personal.LastName, Personal.FirstName, Personal.SecondName, Personal.PersonalID, Machines.MachineName, Machines.MachineID, Products.ProductName, Products.ProductID"; break;
    case sb_Press      : str += ", Machines.MachineName, Machines.MachineID, Personal.LastName, Personal.FirstName, Personal.SecondName, Personal.PersonalID, Products.ProductName, Products.ProductID"; break;
    case sb_ProductName: str += ", Products.ProductName, Products.ProductID, Personal.LastName, Personal.FirstName, Personal.SecondName, Personal.PersonalID, Machines.MachineName, Machines.MachineID"; break;
  }
  str += ", DailyTasks.ProductGroup;";
  Data->ShowDailyTasks->SQL->Strings[11] = str;
  LockDailyTask();
  try {
    RefreshDailyTasks();
    UnlockDailyTask();
  } catch (Exception& e) {
    UnlockDailyTask();
    throw;
  }
}

void __fastcall TRubberStudio::PlaneInfoMClick(TObject *Sender)
{
  TPlaneInfo* form = new TPlaneInfo(NULL);
  try {
    form->PrecisionTB->Position = plane_precision;
    form->Koef1E->Text = plane_k1;
    form->Koef2E->Text = plane_k2;
    form->Koef3E->Text = plane_k3;
    form->Koef4E->Text = plane_k4;
    if ( form->ShowModal() == mrOk ) {
      plane_precision = form->PrecisionTB->Position;
      plane_k1 = form->Koef1E->Text.ToDouble();
      plane_k2 = form->Koef2E->Text.ToDouble();
      plane_k3 = form->Koef3E->Text.ToDouble();
      plane_k4 = form->Koef4E->Text.ToDouble();
    }
    delete form;
  } catch (Exception& e) {
    delete form;
    throw;
  }
}

void __fastcall TRubberStudio::DataDailyTasksDBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  // Дата
  TCanvas* canvas = DataDailyTasksDBGrid->Canvas;
  int date_id = Data->Programs->FieldByName("DateID")->AsInteger;
  bool flag = true;
  if (Data->DateTable->FieldByName("DateID")->AsInteger != date_id) {
    TLocateOptions locate;
    locate.Clear();
    flag = Data->DateTable->Locate("DateID", date_id, locate);
  }
  AnsiString str;
  TDateTime date;
  if (flag && !Data->ShowDailyTasksDate->IsEmpty()) {
    date = Data->DateTable->FieldByName("DateValue")->AsDateTime;
    SmenaToDate(1 + (Column->Field->AsInteger - 1) * 3, date);
    str = date.FormatString("dd.mm.yy");
  } else {
    str = "";
  }
  canvas->FillRect(Rect);
  canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
}

void __fastcall TRubberStudio::DailyTasksDBGridDrawColumnCell(TObject *Sender,
  const TRect &Rect, int DataCol, TColumn *Column, TGridDrawState State)
{
  TCanvas* canvas = DailyTasksDBGrid->Canvas;
  AnsiString str = LoadStr(sUnknowValue);
  switch (Column->ID) {
    // Смена
    case 0: {
      if (!Data->ShowDailyTasksDate->IsEmpty()) {
        int smena = Column->Field->AsInteger;
        int work_day = 1 + (smena-1) / 3;
        smena -= (work_day - 1) * 3;
        str = smena;
      } else {
        str = "";
      }
      break;
    }
    // Прессовщик
    case 1: {
      str = "";
      AnsiString s;
      // Имя
      s = Data->ShowDailyTasks->FieldByName("FirstName")->AsString;
      s = s.Trim();
      if (s.Length()) str = MiscFormat("%c. ", s[1]);
      // Отчество
      s = Data->ShowDailyTasks->FieldByName("SecondName")->AsString;
      s = s.Trim();
      if (s.Length()) str += MiscFormat("%c. ", s[1]);
      // Фамилия
      str += Column->Field->AsString;
      break;
    }
    // Остальные поля таблицы
    default: DailyTasksDBGrid->DefaultDrawColumnCell(Rect, DataCol, Column, State); return;
  }
  canvas->FillRect(Rect);
  canvas->TextOut(Rect.Left+2, Rect.Top+2, str);
}

void __fastcall TRubberStudio::DailyTasksDBGridMouseMove(TObject *Sender,
  TShiftState Shift, int X, int Y)
{
  DailyTasks_point.x = X;
  DailyTasks_point.y = Y;
}

void __fastcall TRubberStudio::PersonalIDBClick(TObject *Sender)
{
  if (!PersonalID->Enabled) return;
  TSelectPersonalForm* form = new TSelectPersonalForm(NULL);
  form->bDesktopCenter = false;
  try {
    TPoint p;
    if (Sender == DailyTasksDBGrid) {
      p = DailyTasksDBGrid->ClientToScreen(DailyTasks_point);
    } else {
      p.x = 0;
      p.y = PersonalIDB->Height + 4;
      p = PersonalIDB->ClientToScreen(p);
    }
    form->Left = p.x;
    form->Top  = p.y;
    if (form->ShowModal() == mrOk) {
      Data->DailyTaskGroupCount->Close();
      Data->DailyTaskGroupCount->ParamByName("PlanInProgID")->AsInteger = Data->PlanInProg->FieldByName("PlanInProgID")->AsInteger;
      Data->DailyTaskGroupCount->ParamByName("smena_id")->AsInteger     = Data->DailyTasks->FieldByName("Smena")->AsInteger;
      Data->DailyTaskGroupCount->ParamByName("group_id")->AsInteger     = Data->DailyTasks->FieldByName("ProductGroup")->AsInteger;
      Data->DailyTaskGroupCount->Prepare();
      Data->DailyTaskGroupCount->Open();
      Data->DailyTaskGroupCount->First();
      int personal_id = Data->Personal->FieldByName("PersonalID")->AsInteger;
      LockDailyTask();
      try {
        while (!Data->DailyTaskGroupCount->Eof) {
          int plan_id = Data->DailyTaskGroupCount->FieldByName("PlanID")->AsInteger;
          TLocateOptions locate;
          locate.Clear();
          if (Data->DailyTasks->Locate("PlanID", plan_id, locate)) {
            Data->DailyTasks->Edit();
            Data->DailyTasks->FieldByName("PersonalID")->AsInteger = personal_id;
            Data->DailyTasks->Post();
          }
          Data->DailyTaskGroupCount->Next();
        }
        Data->DailyTaskGroupCount->Close();
        UnlockDailyTask();
      } catch (Exception &e) {
        Data->DailyTaskGroupCount->Close();
        UnlockDailyTask();
        throw;
      }
      RefreshDailyTasks();
      PersonalID->Text = form->GetPersonalName();
      PersonalID->SetFocus();
    }
    delete form;
  } catch (Exception& e) {
    delete form;
    throw;
  }
}

void __fastcall TRubberStudio::SaveDayBClick(TObject *Sender)
{
  Data->DailyTasks->Post();
}

void __fastcall TRubberStudio::CancelDayBClick(TObject *Sender)
{
  Data->DailyTasks->Cancel();
}

void TRubberStudio::UpdateDailyTaskControls()
{
  bool empty = Data->ShowDailyTasks->IsEmpty();
  AnsiString str = "";
  if (empty) {
    AmtDone->DataSource = NULL;
    AmtDone->Text       = "";
  } else {
    AmtDone->DataSource = Data->DailyTasksDS;
    int personal_id = Data->ShowDailyTasks->FieldByName("PersonalID")->AsInteger;
    TLocateOptions locate;
    locate.Clear();
    if (Data->Personal->Locate("PersonalID", personal_id, locate)) {
      str = Data->GetPersonalName();
    } else {
      str = LoadStr(sUnknowValue);
    }
  }
  PersonalID->Text = str;

  bool flag = !empty;
  DataDailyTasksDBGrid->Enabled = flag;
  DailyTasksDBGrid->Enabled     = flag;
  PersonalID->Enabled           = flag;
  PersonalIDB->Enabled          = flag;
  PersonalIDL->Enabled          = flag;
  AmtDone->Enabled              = flag;
  AmtDoneL->Enabled             = flag;
}

void TRubberStudio::RefreshDailyTasks()
{
  if (Data->DailyTasks->Active) {
    LockDailyTask();
    try {
      int plan_in_prog = Data->PlanInProg->FieldByName("PlanInProgID")->AsInteger;
      bool active = Data->ShowDailyTasksDate->Active;
      if (!active || (active && Data->ShowDailyTasksDate->ParamByName("PlanInProgID")->AsInteger != plan_in_prog)) {
        Data->ShowDailyTasksDate->Close();
        Data->ShowDailyTasksDate->ParamByName("PlanInProgID")->AsInteger = plan_in_prog;
        Data->ShowDailyTasksDate->Prepare();
        Data->ShowDailyTasksDate->Open();
      }
      int smena = 1 + (Data->ShowDailyTasksDate->FieldByName("WorkDay")->AsInteger - 1) * 3;
      Data->ShowDailyTasks->Close();
      Data->ShowDailyTasks->ParamByName("PlanInProgID")->AsInteger = Data->PlanInProg->FieldByName("PlanInProgID")->AsInteger;
      Data->ShowDailyTasks->ParamByName("Smena_1")->AsInteger      = smena;
      Data->ShowDailyTasks->ParamByName("Smena_3")->AsInteger      = smena + 2;
      Data->ShowDailyTasks->Prepare();
      Data->ShowDailyTasks->Open();

      UnlockDailyTask();
    } catch (Exception& e) {
      UnlockDailyTask();
      throw;
    }
    RefreshDailyTaskID();
  }
}

void TRubberStudio::RefreshDailyTaskID()
{
  if (Data->DailyTasks->Active) {
    if (Data->ShowDailyTasks->FieldByName("PlanID")->AsInteger != activeDailyTask) {
      TLocateOptions locate;
      locate.Clear();
      if (!Data->ShowDailyTasks->Locate("PlanID", activeDailyTask, locate)) {
        LockDailyTask();
        try {
          Data->ShowDailyTasks->First();
          if (Data->ShowDailyTasks->IsEmpty()) {
            activeDailyTask = 0;
          } else {
            activeDailyTask = Data->ShowDailyTasks->FieldByName("PlanID")->AsInteger;
          }
          UnlockDailyTask();
        } catch (Exception &e) {
          UnlockDailyTask();
          throw;
        }
      }
    }
    UpdateDailyTaskControls();
  }
}

int TRubberStudio::GetPlanePrecision()
{
  return plane_precision;
}

double TRubberStudio::GetPlaneKoef( const int koef_id )
{
  switch ( koef_id ) {
    case 1: return plane_k1;
    case 2: return plane_k2;
    case 3: return plane_k3;
    case 4: return plane_k4;
    default: return 0;
  }
}

void TRubberStudio::ImportPlaneKoef( double& k1, double& k2, double& k3, double& k4 )
{
  TStringList* string_list = NULL;
  try {

    SetCurrentDir( modelDir );
    string_list  = new TStringList();
    try {
      string_list->LoadFromFile( "rub_gen.pmv" );
    } catch (Exception& e) {
      throw RubberError( sErrorOpenTraceFile );
    }

#define k1_str_index 17
#define k2_str_index 18
#define k3_str_index 19
#define k4_str_index 20

    if ( string_list->Count <= k4_str_index ) throw RubberError( sErrorOpenTraceFile );

    k1 = PickOutDouble( string_list->Strings[k1_str_index], 2 );
    k2 = PickOutDouble( string_list->Strings[k2_str_index], 2 );
    k3 = PickOutDouble( string_list->Strings[k3_str_index], 2 );
    k4 = PickOutDouble( string_list->Strings[k4_str_index], 2 );

    delete string_list; string_list = NULL;
  } catch ( Exception& e ) {
    delete string_list;
    throw;
  }
}


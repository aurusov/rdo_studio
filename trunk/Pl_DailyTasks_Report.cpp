//---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop

#include "Pl_DailyTasks_Report.h"
#include "DataModule.h"

#include "Form_Rubber.h"
#include "Misc.h"
#include "ConstRes.h"
#include "DailyTasks_report_consts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TPlDailyTasksReport::TPlDailyTasksReport():
           TPlBaseReport(),
           LineOffsetY(0.0),
           FPageNumber(0),
           ReportTableWidth(0.0),
           LastX(0.0),
           RecordHeight(0.0),
           DrawingData(false),
           DrawingSmenaTitle(false),
           Plan(""),
           TableTop(0.0)
{
  LoadReportParams("DailyTasks", poPortrait);
  CorrectOffsets();
}

__fastcall TPlDailyTasksReport::TPlDailyTasksReport(const int work_day, AnsiString PlanName):
           TPlBaseReport(),
           LineOffsetY(0.0),
           FPageNumber(0),
           ReportTableWidth(0.0),
           LastX(0.0),
           RecordHeight(0.0),
           DrawingData(false),
           DrawingSmenaTitle(false)
{
  int date_id = Data->Programs->FieldByName("DateID")->AsInteger;
  bool flag = true;
  if (Data->DateTable->FieldByName("DateID")->AsInteger != date_id) {
    TLocateOptions locate;
    locate.Clear();
    flag = Data->DateTable->Locate("DateID", date_id, locate);
  }
  AnsiString str;
  TDateTime date;
  if (flag) {
    date = Data->DateTable->FieldByName("DateValue")->AsDateTime;
    SmenaToDate(1 + (work_day - 1) * 3, date);
    str = date.FormatString("dd.mm.yyyy г.");
  } else {
    str = LoadStr(sUnknowValue);
  }
  Title = MiscFormat(LoadStr(sTaskTitle).c_str(), str.c_str());
  Plan = PlanName;
  LoadReportParams("DailyTasks", poPortrait);
  CorrectOffsets();
  ReportTableWidth = FioWidth + PressNameWidth + DetNameWidth + TasksWulkTimeWidth + WulkTempWidth +
               PFNumberWidth + PressureWidth + QuantityWidth + QuantityInFactWidth;
  switch (RubberStudio->DailyTasks_sortBy) {
    case sb_Fio         : ReportTableWidth -= FioWidth; break;
    case sb_ProductName : ReportTableWidth -= PressNameWidth; break;
    case sb_Press       : ReportTableWidth -= DetNameWidth; break;
  }
  LineOffsetY = 1.5;
}

__fastcall TPlDailyTasksReport::~TPlDailyTasksReport()
{
  WriteReportParams("DailyTasks");
}

// Проверка на окончание отрисовки страницы или отчета
bool TPlDailyTasksReport::CheckEndDraw(double Value)
{
  double checkvalue = PageHeightMm - OffsetBottom - OffsetTop - TasksMiscOffset;
  if (PrintFooter)
    checkvalue -= FooterHeight;
  if (Value >= checkvalue) {
    if (Page == FPageNumber && DrawMode != dm_Init) {
      return true;
    }
    if (DrawMode == dm_Print && DrawingData && !DrawingSmenaTitle)
      CutTable();
    BeginNewPage();
    if (DrawingData && !DrawingSmenaTitle) {
      double lastX = LastX;
      DrawTable();
      LastX = lastX;
    }
  }
  return false;
}

bool TPlDailyTasksReport::DrawReportTop()
{
  TFontStyles fontstyle;
  fontstyle.Clear();
  fontstyle << fsBold;
  SetFont("Arial", fontstyle, 14);
  TTextDimentions dim;
  AnsiString str = LoadStr(sAssert);
  TextInRectMm(0, 0, 0, 0, str, DT_CALCRECT, &dim);
  double aswidth = dim.TextWidth;
  double asheight = dim.TextHeight;
  AnsiString fmt = "%s\r\n%s _____________";

  str = MiscFormat(fmt.c_str(), LoadStr(sCompanyProductionChief).c_str(), Data->Programs->FieldByName("CompanyProductionChief")->AsString.c_str());
  fontstyle.Clear();
  SetFont("Arial", fontstyle, 12);
  TextInRectMm(0, 0, 0, 0, str, DT_LEFT | DT_CALCRECT, &dim);
  double width = std::max(aswidth, dim.TextWidth);

  double left = PageWidthMm - width - OffsetLeft - OffsetRight - 3;

  fontstyle << fsBold;
  SetFont("Arial", fontstyle, 14);
  str = LoadStr(sAssert);
  if (CheckEndDraw(LastY + asheight + LineOffsetY))
    return true;

  TextInRectMm(left, LastY, PageWidthMm - OffsetLeft - OffsetRight, LastY + asheight, str, DT_LEFT, NULL);

  LastY += (asheight + LineOffsetY);

  str = MiscFormat(fmt.c_str(), LoadStr(sCompanyProductionChief).c_str(), Data->Programs->FieldByName("CompanyProductionChief")->AsString.c_str());
  fontstyle.Clear();
  SetFont("Arial", fontstyle, 12);
  if (CheckEndDraw(LastY + dim.TextHeight + LineOffsetY))
    return true;
  TextInRectMm(left, LastY, PageWidthMm - OffsetLeft - OffsetRight, LastY + dim.TextHeight, str, DT_LEFT, NULL);

  LastY += (dim.TextHeight + LineOffsetY);

  return false;
}

bool TPlDailyTasksReport::DrawReportTitle()
{
  TFontStyles fontstyle;
  fontstyle.Clear();
  fontstyle << fsBold;
  SetFont("Arial", fontstyle, 20);
  AnsiString str = Title;
  TTextDimentions dim;
  TextInRectMm(3, LastY, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, str, DT_CENTER | DT_VCENTER, &dim);
  LastY += (dim.TextHeight + LineOffsetY);

  fontstyle.Clear();
  SetFont("Arial", fontstyle, 12);
  str = "(" + MiscFormat(LoadStr(sProgramTitle).c_str(), Data->GetDateValue().c_str()) + ")";
  TextInRectMm(3, LastY, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, str, DT_CENTER | DT_VCENTER, &dim);
  LastY += dim.TextHeight;

  return false;
}

bool TPlDailyTasksReport::DrawTable()
{
  DrawTo->Brush->Style = bsClear;
  LastX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
  TFontStyles style;
  style.Clear();
  style << fsBold;
  SetFont("Arial", style, 10);
  double DrawHeight = PageHeightMm - OffsetBottom - OffsetTop - TasksMiscOffset;
  if (PrintFooter)
    DrawHeight -= FooterHeight;

// Заголовок таблицы
  if (RubberStudio->DailyTasks_sortBy != sb_Fio) {
    TextInRectMm(LastX + 1, LastY + 1, LastX + FioWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sTaskFio), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LastX += FioWidth;
  }

  if (RubberStudio->DailyTasks_sortBy != sb_Press) {
    TextInRectMm(LastX + 1, LastY + 1, LastX + PressNameWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sTaskPress), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LastX += PressNameWidth;
  }

  if (RubberStudio->DailyTasks_sortBy != sb_ProductName) {
    TextInRectMm(LastX + 1, LastY + 1, LastX + DetNameWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sTaskDetName), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LastX += DetNameWidth;
  }

  TextInRectMm(LastX + 1, LastY + 1, LastX + TasksWulkTimeWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sWulkanTime), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
  LastX += TasksWulkTimeWidth;

  TextInRectMm(LastX + 1, LastY + 1, LastX + WulkTempWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sTaskWulkTemp), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
  LastX += WulkTempWidth;

  EllipseAtMm(LastX - WulkTempWidth + 2.25, LastY + 3.5, LastX - WulkTempWidth + 3.75, LastY + 5, clWhite, bsClear, clBlack, psSolid, 0, true);
  EllipseAtMm(LastX - WulkTempWidth + 4.8, LastY + 8.5, LastX - WulkTempWidth + 6.3, LastY + 10, clWhite, bsClear, clBlack, psSolid, 0, true);

  TextInRectMm(LastX + 1, LastY + 1, LastX + PFNumberWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sTaskPFNumber), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
  LastX += PFNumberWidth;

  TextInRectMm(LastX + 1, LastY + 1, LastX + PressureWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sTaskPressure), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
  LastX += PressureWidth;

  TextInRectMm(LastX + 1, LastY + 1, LastX + QuantityWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sTaskQuantity), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
  LastX += QuantityWidth;

  TextInRectMm(LastX + 1, LastY + 1, LastX + QuantityInFactWidth -1, LastY + TasksTableHeaderHeight, LoadStr(sTaskQuantityInFact), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
  LastX += QuantityInFactWidth;


// Границы таблицы и линии. Пока до низа страницы.
  TableTop = LastY;
  double TableOffsetX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
  RectAtMm(TableOffsetX, LastY, TableOffsetX + ReportTableWidth, LastY + TasksTableHeaderHeight, clWhite, bsClear, clBlack, psSolid, 0.5, true);

  if (RubberStudio->DailyTasks_sortBy != sb_Fio) {
    TableOffsetX += FioWidth;
    LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  }

  if (RubberStudio->DailyTasks_sortBy != sb_Press) {
    TableOffsetX += PressNameWidth;
    LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  }

  if (RubberStudio->DailyTasks_sortBy != sb_ProductName) {
    TableOffsetX += DetNameWidth;
    LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  }

  TableOffsetX += TasksWulkTimeWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += WulkTempWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += PFNumberWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += PressureWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += QuantityWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);

  LastY += TasksTableHeaderHeight;
  return false;
}

bool TPlDailyTasksReport::DrawBottomFio()
{
  TFontStyles style;
  style.Clear();
  SetFont("Arial", style, 10);
  LastY += TasksMainOffset;
  if (DrawFio(LoadStr(sCompanyDepartmentChief), Data->Programs->FieldByName("CompanyDepartmentChief")->AsString))
    return true;
  LastY += LineOffsetY;
  if (DrawFio(LoadStr(sForeman), ""))
    return true;
  return false;
}

bool TPlDailyTasksReport::DrawSmenaTable(int Smena)
{
  DrawingSmenaTitle = true;
  if (Smena != 1) {
    if (DrawingData) {
        CutTable();
        if (DrawBottomFio())
          return true;
    }

    LastY += TasksMainOffset;

    if (Page == FPageNumber && DrawMode != dm_Init) {
      return true;
    }
    BeginNewPage();
  };

  TTextDimentions dim;
  TFontStyles fontstyle;
  fontstyle.Clear();
  fontstyle << fsBold;
  SetFont("Arial", fontstyle, 16);
  AnsiString smena = MiscFormat(LoadStr(sTaskSmenaTitle).c_str(), Smena);
  TextInRectMm(0, 0, 0, 0, smena, DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
  if (CheckEndDraw(LastY + dim.TextHeight + LineOffsetY))
    return true;
  TextInRectMm(3, LastY, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, smena, DT_CENTER | DT_VCENTER, NULL);

  LastY += (dim.TextHeight + LineOffsetY);

  fontstyle.Clear();
  SetFont("Arial", fontstyle, 12);

  AnsiString str = "";
  switch (RubberStudio->DailyTasks_sortBy) {
    case sb_Fio         : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByPersonal).c_str()); break;
    case sb_Press       : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByPress).c_str()); break;
    case sb_ProductName : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByName).c_str()); break;
  }

  TextInRectMm(0, 0, 0, 0, str, DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
  if (CheckEndDraw(LastY + dim.TextHeight + LineOffsetY))
    return true;
  TextInRectMm(3, LastY, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, str, DT_CENTER | DT_VCENTER, NULL);
  LastY += dim.TextHeight + TasksMainOffset + LineOffsetY;

  if (DrawingData) {
    DrawTable();
  }
  DrawingSmenaTitle = false;
  LastX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
  return false;
}

// Отрисовка названия группы (группирование по наим., тех. карте или заказчику)
bool TPlDailyTasksReport::DrawTableGroup(AnsiString GroupTitle, AnsiString GroupFieldName) {
  TTextDimentions dim;
  TextInRectMm(0, 0, 0, 0, GroupTitle, DT_CALCRECT, &dim);
  double checkrecordheight = (DrawMode == dm_Init) ? dim.TextHeight : RecordHeight;
  if (CheckEndDraw(LastY + TableGroupHeight + checkrecordheight + 1 + 0.5))
    return true;
  TFontStyles style;
  style.Clear();
  style << fsBold << fsItalic;
  double TableOffsetX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
  RectAtMm(TableOffsetX, LastY, TableOffsetX + ReportTableWidth, LastY + TableGroupHeight, TColor(TasksGroupFillColor), bsSolid, clBlack, psSolid, 0.25, true);
  SetFont("Arial", style, 10);
  switch (RubberStudio->DailyTasks_sortBy) {
    case sb_Fio         : GroupTitle = GetFio(); break;
    case sb_Press       : GroupTitle = Data->ShowDailyTasks->FieldByName("MachineName")->AsString; break;
    case sb_ProductName : GroupTitle = Data->ShowDailyTasks->FieldByName("ProductName")->AsString; break;
  }
  TextInRectMm(TableOffsetX, LastY - 0.25, TableOffsetX + ReportTableWidth, LastY + TableGroupHeight, GroupTitle, DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
  LastY += TableGroupHeight;
  return false;
}

// Отрисовка ячейки
void TPlDailyTasksReport::DrawTableCell(AnsiString &CellText, double CellWidth, UINT Format, double &MaxHeight)
{
  if (DrawMode == dm_Init) {
    TTextDimentions dim;
    TextInRectMm(LastX + 1, LastY + 1, LastX + CellWidth -1, 0, CellText, Format | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    if (dim.TextHeight > MaxHeight)
      MaxHeight = dim.TextHeight;
  } else {
    TextInRectMm(LastX + 1, LastY + 1, LastX + CellWidth -1, LastY + MaxHeight, CellText, Format, NULL);
  }
  LastX += CellWidth;
  CellText = "";
}

// Обрезка таблицы в конце вывода данных на данной странице
void TPlDailyTasksReport::CutTable() {
  if (DrawMode != dm_Init) {
    double TableHeight = PageHeightMm - OffsetBottom - OffsetTop - TasksMiscOffset;
    if (PrintFooter)
      TableHeight -= FooterHeight;
    RectAtMm(-OffsetLeft, LastY + 1, PageWidthMm - OffsetLeft, TableHeight + 1, clWhite, bsSolid, clWhite, psClear, 0, true);
    double TableOffsetX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
    if (!TableTop)
      TableTop = LastY - 1;
    RectAtMm(TableOffsetX, TableTop, TableOffsetX + ReportTableWidth, LastY + 1, clWhite, bsClear, clBlack, psSolid, 0.5, true);
  }
  LastY += (1 + 0.5);
}

bool TPlDailyTasksReport::DrawFio(AnsiString FioTitle, AnsiString Fio)
{
  AnsiString fmt = "%s\r\n%s _____________";
  AnsiString fstr = Fio;
  if (!Fio.Length())
    fstr = "________________   ";
  AnsiString str = MiscFormat(fmt.c_str(), FioTitle.c_str(), fstr.c_str());
  TTextDimentions dim;
  TextInRectMm(0, 0, 0, 0, str, DT_CALCRECT, &dim);
  if (CheckEndDraw(LastY + dim.TextHeight))
    return true;
  double TableOffsetX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
  TextInRectMm(TableOffsetX + ReportTableWidth/2 - 25, LastY, TableOffsetX + ReportTableWidth/2 - 25 + dim.TextWidth + 1, LastY + dim.TextHeight + 1, str, DT_LEFT, NULL);
  LastY += (dim.TextHeight + LineOffsetY);
  return false;
}

// Окончание отрисовки
void TPlDailyTasksReport::EndDrawing(TCursor OldCursor)
{
  if (DrawingData && !DrawingSmenaTitle) {
    CutTable();
    DrawingData = false;
  }
  DrawFooter();
  Data->ShowDailyTasks->First();
  Data->ShowDailyTasks->EnableControls();
  Screen->Cursor = OldCursor;
}

AnsiString TPlDailyTasksReport::GetFio()
{
  AnsiString str = "";
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
  return str += Data->ShowDailyTasks->FieldByName("LastName")->AsString;
}

// Собственно отрисовка
void TPlDailyTasksReport::DrawReport(int PageNumber)
{
  FPageNumber = PageNumber;
  TCursor oldcur = Screen->Cursor;
  LastY = 0;
  Data->ShowDailyTasks->DisableControls();
  try {
    Screen->Cursor = crHourGlass;
    Data->ShowDailyTasks->First();
    if (Previewing)
      RectAtMm(-OffsetLeft - TasksMainOffset, -OffsetTop - TasksMainOffset, PageWidthMm - OffsetLeft + TasksMainOffset, PageHeightMm - OffsetTop + TasksMainOffset, clWhite, bsSolid, clWhite, psSolid, 1);
    if (DrawReportTop()) {
      EndDrawing(oldcur);
      return;
    }
    LastY += TasksMainOffset;
    if (DrawReportTitle()) {
      EndDrawing(oldcur);
      return;
    }
    LastY += TasksMainOffset;
    double tableoffset = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
    AnsiString group_field_name;
    switch (RubberStudio->DailyTasks_sortBy) {
      case sb_Fio         : group_field_name = "PersonalID"; break;
      case sb_ProductName : group_field_name = "ProductID"; break;
      case sb_Press       : group_field_name = "MachineID"; break;
    }
    AnsiString previus_group_value = "";
    int previus_smena_value = 0;
    int previus_personal = 0;
    int previus_product_group = -1;
    AnsiString previus_product_name = "";


    int count = Data->ShowDailyTasks->RecordCount;
    if (DrawMode == dm_Init)
      RecordHeight = 0;

    AnsiString str = "";
    DrawingData = true;
    DrawingSmenaTitle = false;
    LastX = tableoffset;

// Отрисовка данных
    for (int i = 0; i < count; i++) {
      LastX = tableoffset;

      int smena_value = Data->ShowDailyTasks->FieldByName("Smena")->AsInteger;
      int work_days = 1 + (smena_value-1) / 3;
      smena_value -= (work_days - 1) * 3;

      if (smena_value != previus_smena_value) {
        if (DrawSmenaTable(smena_value)) {
          EndDrawing(oldcur);
          return;
        }
        previus_smena_value = smena_value;
        previus_group_value = "";
        previus_personal = 0;
        previus_product_group = -1;
        previus_product_name = "";
      }

      bool draw_product_line_group = true;
      AnsiString group_value = Data->ShowDailyTasks->FieldByName(group_field_name)->AsString;
      if (group_value != previus_group_value) {
        if (DrawTableGroup(group_value, group_field_name)) {
          EndDrawing(oldcur);
          return;
        }
        previus_group_value = group_value;
        if (RubberStudio->DailyTasks_sortBy == sb_ProductName || RubberStudio->DailyTasks_sortBy == sb_Press) {
          previus_personal = 0;
          previus_product_name = "";
        }
        if (RubberStudio->DailyTasks_sortBy == sb_ProductName)
          previus_product_group = -1;
        draw_product_line_group = false;
      }

      TTextDimentions dim;
      TextInRectMm(0, 0, 0, 0, group_value, DT_CALCRECT, &dim);
      double checkheight = (DrawMode == dm_Init) ? dim.TextHeight : RecordHeight;
      if (CheckEndDraw(LastY + checkheight + 1 + 0.5)){
        EndDrawing(oldcur);
        return;
      }

      TFontStyles style;
      style.Clear();
      SetFont("Arial Narrow", style, 10);

      int group = Data->ShowDailyTasks->FieldByName("ProductGroup")->AsInteger;

      if (RubberStudio->DailyTasks_sortBy != sb_ProductName) {
        if (group != previus_product_group) {
          if (draw_product_line_group) {
            LineAtMm(tableoffset, LastY + 1, tableoffset + ReportTableWidth, LastY + 1, 0.25, psSolid);
            LastY += 1 + 0.25;
            previus_personal = 0;
          }
          if (RubberStudio->DailyTasks_sortBy != sb_Press) {
            str = Data->ShowDailyTasks->FieldByName("MachineName")->AsString;
            DrawTableCell(str, PressNameWidth, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);
          }
          previus_product_group = group;
        } else {
          if (RubberStudio->DailyTasks_sortBy != sb_Press)
            LastX += PressNameWidth;
        }
      }

      if (RubberStudio->DailyTasks_sortBy != sb_Fio) {
        int personal = Data->ShowDailyTasks->FieldByName("PersonalID")->AsInteger;
        if (personal != previus_personal) {
          str = GetFio();
          DrawTableCell(str, FioWidth, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);
          previus_personal = personal;
        } else {
          LastX += FioWidth;
        }
      }

      if (RubberStudio->DailyTasks_sortBy == sb_ProductName) {
        str = Data->ShowDailyTasks->FieldByName("MachineName")->AsString;
        DrawTableCell(str, PressNameWidth, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);
      }

      if (RubberStudio->DailyTasks_sortBy != sb_ProductName) {
        str = Data->ShowDailyTasks->FieldByName("ProductName")->AsString;
        DrawTableCell(str, DetNameWidth, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);
      }

      str = Data->ShowDailyTasks->FieldByName("VulcanTime")->AsFloat;
      DrawTableCell(str, TasksWulkTimeWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

      str = Data->ShowDailyTasks->FieldByName("VulcanTemperature")->AsFloat;
      DrawTableCell(str, WulkTempWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

      str = Data->ShowDailyTasks->FieldByName("PFCount")->AsInteger;
      DrawTableCell(str, PFNumberWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

      str = Data->ShowDailyTasks->FieldByName("VulcanPressure")->AsFloat;
      DrawTableCell(str, PressureWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

      str = Data->ShowDailyTasks->FieldByName("AmtToDo")->AsString;
      DrawTableCell(str, QuantityWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

      str = Data->ShowDailyTasks->FieldByName("AmtDone")->AsString;
      DrawTableCell(str, QuantityInFactWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

      LastY += RecordHeight;
      Data->ShowDailyTasks->Next();
    }
    DrawingData = false;
    CutTable();
    if (DrawBottomFio())
      return;
    LastY += TasksMainOffset;

    EndDrawing(oldcur);
  } catch (Exception& e) {
    Screen->Cursor = oldcur;
    Data->ShowDailyTasks->EnableControls();
  }
}


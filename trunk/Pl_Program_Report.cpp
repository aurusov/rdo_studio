//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "Pl_Program_Report.h"
#include "DataModule.h"

#include "Form_Rubber.h"
#include "Misc.h"
#include "ConstRes.h"
#include "Program_report_consts.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TPlProgramReport::TPlProgramReport():
           TPlBaseReport(),
           LineOffsetY(0.0),
           FPageNumber(0),
           BottomItemTop(0.0),
           ReportTableWidth(0.0),
           LastX(0.0),
           ReportTableHeaderHeight(0.0),
           RecordHeight(0.0),
           DrawingData(false),
           DrawingAdditionTitle(false),
           CurrentvulcanProgramTime(0.0),
           CurrentvulcanProgramPrice(0.0),
           CurrentcutProgramTime(0.0),
           CurrentcutProgramPrice(0.0),
           CurrentcustomerProgramPrice(0.0),
           TotalvulcanProgramTime(0.0),
           TotalvulcanProgramPrice(0.0),
           TotalcutProgramTime(0.0),
           TotalcutProgramPrice(0.0),
           TotalcustomerProgramPrice(0.0),
           AdditionToPrint(""),
           PrintMainPart(false),
           PrintingTargetAddition(false),
           MainPart(false)
{
  LoadReportParams("Program", poLandscape);
  CorrectOffsets();
}

__fastcall TPlProgramReport::TPlProgramReport(AnsiString ReportTitle):
           TPlBaseReport(),
           LineOffsetY(0.0),
           FPageNumber(0),
           BottomItemTop(0.0),
           ReportTableWidth(0.0),
           LastX(0.0),
           ReportTableHeaderHeight(0.0),
           RecordHeight(0.0),
           DrawingData(false),
           DrawingAdditionTitle(false),
           CurrentvulcanProgramTime(0.0),
           CurrentvulcanProgramPrice(0.0),
           CurrentcutProgramTime(0.0),
           CurrentcutProgramPrice(0.0),
           CurrentcustomerProgramPrice(0.0),
           TotalvulcanProgramTime(0.0),
           TotalvulcanProgramPrice(0.0),
           TotalcutProgramTime(0.0),
           TotalcutProgramPrice(0.0),
           TotalcustomerProgramPrice(0.0),
           AdditionToPrint(""),
           PrintMainPart(false),
           PrintingTargetAddition(false),
           MainPart(false)
{
  PrintMainPart = Data->ProgramPart->FieldByName("MainPart")->AsBoolean;
  if (!PrintMainPart && RubberStudio->isPrintByPrPart())
    AdditionToPrint = MiscFormat(LoadStr(sAdditionTitle).c_str(), Data->ProgramPart->FieldByName("CorrectionDate")->AsDateTime.FormatString(LoadStr(sAdditionDateFormat)));

  Title = ReportTitle;
  LoadReportParams("Program", poLandscape);
  CorrectOffsets();
  ReportTableWidth = TechCardWidth + NameWidth + PressFormWidth + ToProduceWidth + RubberSortWidth +
               DetWeightWidth + ZagWeightWidth + RubberQuantityWidth + WulkTimeWidth + LoadTimeWidth + WulkTimeNormaWidth +
               WulkCostWidth + CutTimeNormaWidth + CutCostWidth + WulkTimeProgWidth + WulkSalaryProgWidth +
               CutTimeProgWidth + CutSalaryProgWidth;
  switch (RubberStudio->ProgramContents_sortBy) {
    case sb_Customer : ReportTableWidth += (EdPriceWidth + PriceWidth); break;
    case sb_TechMap  : ReportTableWidth -= TechCardWidth; break;
    case sb_Name     : ReportTableWidth -= NameWidth; break;
  }
  LineOffsetY = 1.5;
}

__fastcall TPlProgramReport::~TPlProgramReport()
{
  WriteReportParams("Program");
  if (ReportData && ReportData != Data->ShowProgramContents)
    ReportData->Close();
}

int TPlProgramReport::FindSQLSubstring(AnsiString Substring)
{
  int i = 0;
  while (((TQuery*)ReportData)->SQL->Strings[i].AnsiPos(Substring) == 0)
    i++;
  return i;
}

// Открытие Query с полной группировкой
void TPlProgramReport::OpenReportDataSource()
{
  try {
    switch (RubberStudio->ProgramContents_sortBy) {
      case sb_Customer : ReportData = Data->ShowProgramContents; break;
      default          : {
        ReportData = Data->ShowProgContFullGroup;

        TStrings* sql = ((TQuery*)ReportData)->SQL;

        AnsiString groupstr = RubberStudio->ProgramContents_sortBy == sb_Name ? "      Orders.ProductID, Products.TechMap," : "      Products.TechMap, Orders.ProductID,";
        AnsiString sortstr = RubberStudio->ProgramContents_sortBy == sb_Name  ? "Products.ProductName, Products.TechMap" : "Products.TechMap, Products.ProductName";
        AnsiString partsstr = "      ProgramPart.MainPart, ProgramPart.ProgramPartID, ProgramPart.CorrectionDate,";
        int groupindex;
        if (!RubberStudio->PrSortByPartM->Checked) {
          if (sql->IndexOf(partsstr) != -1) {
            sql->Delete(FindSQLSubstring("FROM") - 2);
            sql->Delete(FindSQLSubstring("GROUP BY") + 1);
          }
          groupindex = FindSQLSubstring("GROUP BY") + 1;
          sortstr = "ORDER BY " + sortstr;
        } else {
          if (sql->IndexOf(partsstr) == -1) {
            sql->Insert(FindSQLSubstring("FROM") - 1, partsstr);
            sql->Insert(FindSQLSubstring("GROUP BY") + 1, partsstr);
          }
          groupindex = FindSQLSubstring("GROUP BY") + 2;
          sortstr = "ORDER BY ProgramPart.MainPart DESC, ProgramPart.CorrectionDate, " + sortstr;
        }
        int sortindex =  sql->Count - 1;
        sql->Strings[groupindex] = groupstr;
        sql->Strings[sortindex] = sortstr;

        ((TQuery*)ReportData)->ParamByName("progID")->AsInteger = Data->ShowProgramContents->ParamByName("progID")->AsInteger;
        ((TQuery*)ReportData)->Prepare();
        ReportData->Open();

        break;
      }
    }
  } catch (Exception& e) {
    if (ReportData && ReportData != Data->ShowProgramContents)
      ReportData->Close();
    throw;
  }
}

// Проверка на окончание отрисовки страницы или отчета
bool TPlProgramReport::CheckEndDraw(double Value)
{
  double checkvalue = PageHeightMm - OffsetBottom - OffsetTop - MiscOffset;
  if (PrintFooter)
    checkvalue -= FooterHeight;
  if (Value >= checkvalue) {
    if (Page == FPageNumber && DrawMode != dm_Init) {
      return true;
    }
    if (DrawMode == dm_Print && DrawingData && !DrawingAdditionTitle)
      CutTable();
    BeginNewPage();
    if (DrawingData && !DrawingAdditionTitle) {
      double lastX = LastX;
      DrawTable();
      LastX = lastX;
    }
  }
  return false;
}

bool TPlProgramReport::DrawReportTop()
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
  AnsiString fmt = "%s\r\n%s\r\n%s _____________";


  AnsiString fio = Data->Programs->FieldByName("CompanyDirector")->AsString;
  if (!fio.Length())
    fio = "________________   ";

  str = MiscFormat(fmt.c_str(), LoadStr(sCompanyDirector).c_str(), Data->Programs->FieldByName("CompanyName")->AsString.c_str(), fio.c_str());
  fontstyle.Clear();
  SetFont("Arial", fontstyle, 12);
  TextInRectMm(0, 0, 0, 0, str, DT_LEFT | DT_CALCRECT, &dim);
  double width = max(aswidth, dim.TextWidth);

  double left = PageWidthMm - width - OffsetLeft - OffsetRight - 3;

  fontstyle << fsBold;
  SetFont("Arial", fontstyle, 14);
  str = LoadStr(sAssert);
  if (CheckEndDraw(LastY + asheight + LineOffsetY))
    return true;

  TextInRectMm(left, LastY, PageWidthMm - OffsetLeft - OffsetRight, LastY + asheight, str, DT_LEFT, NULL);

  LastY += (asheight + LineOffsetY);

  str = MiscFormat(fmt.c_str(), LoadStr(sCompanyDirector).c_str(), Data->Programs->FieldByName("CompanyName")->AsString.c_str(), fio.c_str());
  fontstyle.Clear();
  SetFont("Arial", fontstyle, 12);
  if (CheckEndDraw(LastY + dim.TextHeight + LineOffsetY))
    return true;

  TextInRectMm(left, LastY, PageWidthMm - OffsetLeft - OffsetRight, LastY + dim.TextHeight, str, DT_LEFT, NULL);

  LastY += (dim.TextHeight + LineOffsetY);

  return false;
}

bool TPlProgramReport::DrawReportTitle()
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

  switch (RubberStudio->ProgramContents_sortBy) {
    case sb_Name     : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByName).c_str()); break;
    case sb_TechMap  : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByTechMap).c_str()); break;
    case sb_Customer : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByCustomer).c_str()); break;
  }
  TextInRectMm(3, LastY, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, str, DT_CENTER | DT_VCENTER, &dim);
  LastY += dim.TextHeight;

  return false;
}

bool TPlProgramReport::DrawTable()
{
  DrawTo->Brush->Style = bsClear;
  LastX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
  TFontStyles style;
  style.Clear();
  style << fsBold;
  SetFont("Arial", style, 7);
  TTextDimentions dim;
  dim.TextWidth = 0;
  dim.TextHeight = 0;
  double DrawHeight = PageHeightMm - OffsetBottom - OffsetTop - MiscOffset;
  if (PrintFooter)
    DrawHeight -= FooterHeight;

  if (DrawMode == dm_Init)
    ReportTableHeaderHeight = 0;

// Заголовок таблицы
  if (RubberStudio->ProgramContents_sortBy != sb_TechMap) {
    DrawTableCell(LoadStr(sTechMapNo), TechCardWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);
  }

  if (RubberStudio->ProgramContents_sortBy != sb_Name) {
    DrawTableCell(LoadStr(sDetName), NameWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);
  }

  DrawTableCell(LoadStr(sPressForm), PressFormWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);

  DrawTableCell(LoadStr(sToProduce), ToProduceWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);

  DrawTableCell(LoadStr(sRubberSort), RubberSortWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);

  DrawTableCell(LoadStr(sDetWeight), DetWeightWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);

  DrawTableCell(LoadStr(sZagWeight), ZagWeightWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);

  DrawTableCell(LoadStr(sRubberQuantity), RubberQuantityWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);

  DrawTableCell(LoadStr(sWulkanTime), WulkTimeWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);

  DrawTableCell(LoadStr(sLoadTime), LoadTimeWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);

  if (DrawMode == dm_Init) {
    TextInRectMm(LastX + 1, LastY + 1, LastX + WulkTimeNormaWidth + WulkCostWidth -1, 0, LoadStr(sWulkan), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    TTextDimentions dim1;
    TextInRectMm(LastX + 1, LastY + dim.TextHeight + 1, LastX + WulkTimeNormaWidth -1, 0, LoadStr(sTimeNorma), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim1);
    dim1.TextHeight += 2;
    LastX += WulkTimeNormaWidth;
    TTextDimentions dim2;
    TextInRectMm(LastX + 1, LastY + dim.TextHeight + 1, LastX + WulkCostWidth -1, 0, LoadStr(sCost), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim2);
    dim2.TextHeight += 2;
    dim.TextHeight += max(dim1.TextHeight, dim2.TextHeight);
    if (dim.TextHeight > ReportTableHeaderHeight)
      ReportTableHeaderHeight = dim.TextHeight;
  } else {
    TextInRectMm(LastX + 1, LastY + 1, LastX + WulkTimeNormaWidth + WulkCostWidth -1, 0, LoadStr(sWulkan), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    TTextDimentions dim1;
    TextInRectMm(LastX + 1, LastY + 1, LastX + WulkTimeNormaWidth + WulkCostWidth -1, LastY + 1 + dim.TextHeight, LoadStr(sWulkan), DT_WORDBREAK | DT_CENTER | DT_VCENTER, &dim1);
    LineAtMm(LastX, LastY + dim1.TextHeight + 1, LastX + WulkTimeNormaWidth + WulkCostWidth, LastY + dim1.TextHeight + 1, 0.25, psSolid);
    TextInRectMm(LastX + 1, LastY + dim1.TextHeight + 1, LastX + WulkTimeNormaWidth -1, LastY + ReportTableHeaderHeight, LoadStr(sTimeNorma), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LastX += WulkTimeNormaWidth;
    TextInRectMm(LastX + 1, LastY + dim1.TextHeight + 1, LastX + WulkCostWidth -1, LastY + ReportTableHeaderHeight, LoadStr(sCost), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LineAtMm(LastX, LastY + dim1.TextHeight + 1, LastX, DrawHeight, 0.25, psSolid);
  }
  LastX += WulkCostWidth;

  if (DrawMode == dm_Init) {
    TextInRectMm(LastX + 1, LastY + 1, LastX + CutTimeNormaWidth + CutCostWidth -1, 0, LoadStr(sCut), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    TTextDimentions dim1;
    TextInRectMm(LastX + 1, LastY + dim.TextHeight + 1, LastX + CutTimeNormaWidth -1, 0, LoadStr(sTimeNorma), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim1);
    dim1.TextHeight += 2;
    LastX += CutTimeNormaWidth;
    TTextDimentions dim2;
    TextInRectMm(LastX + 1, LastY + dim.TextHeight + 1, LastX + CutCostWidth -1, 0, LoadStr(sCost), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim2);
    dim2.TextHeight += 2;
    dim.TextHeight += max(dim1.TextHeight, dim2.TextHeight);
    if (dim.TextHeight > ReportTableHeaderHeight)
      ReportTableHeaderHeight = dim.TextHeight;
  } else {
    TextInRectMm(LastX + 1, LastY + 1, LastX + CutTimeNormaWidth + CutCostWidth -1, 0, LoadStr(sCut), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    TTextDimentions dim1;
    TextInRectMm(LastX + 1, LastY + 1, LastX + CutTimeNormaWidth + CutCostWidth -1, LastY + 1 + dim.TextHeight, LoadStr(sCut), DT_WORDBREAK | DT_CENTER | DT_VCENTER, &dim1);
    LineAtMm(LastX, LastY + dim1.TextHeight + 1, LastX + CutTimeNormaWidth + CutCostWidth, LastY + dim1.TextHeight + 1, 0.25, psSolid);
    TextInRectMm(LastX, LastY + dim1.TextHeight + 1, LastX + CutTimeNormaWidth -1, LastY + ReportTableHeaderHeight, LoadStr(sTimeNorma), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LastX += CutTimeNormaWidth;
    TextInRectMm(LastX + 1, LastY + dim1.TextHeight + 1, LastX + CutCostWidth -1, LastY + ReportTableHeaderHeight, LoadStr(sCost), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LineAtMm(LastX, LastY + dim1.TextHeight + 1, LastX, DrawHeight, 0.25, psSolid);
  }
  LastX += CutCostWidth;

  if (DrawMode == dm_Init) {
    TextInRectMm(LastX + 1, LastY + 1, LastX + WulkTimeProgWidth + WulkSalaryProgWidth -1, 0, LoadStr(sWulkanProg), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    TTextDimentions dim1;
    TextInRectMm(LastX + 0.5, LastY + dim.TextHeight + 1, LastX + WulkTimeProgWidth -0.5, 0, LoadStr(sProgTime), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim1);
    dim1.TextHeight += 2;
    LastX += WulkTimeNormaWidth;
    TTextDimentions dim2;
    TextInRectMm(LastX + 1, LastY + dim.TextHeight + 1, LastX + WulkSalaryProgWidth -1, 0, LoadStr(sSalary), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim2);
    dim2.TextHeight += 2;
    dim.TextHeight += max(dim1.TextHeight, dim2.TextHeight);
    if (dim.TextHeight > ReportTableHeaderHeight)
      ReportTableHeaderHeight = dim.TextHeight;
  } else {
    TextInRectMm(LastX + 1, LastY + 1, LastX + WulkTimeProgWidth + WulkSalaryProgWidth -1, 0, LoadStr(sWulkanProg), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    TTextDimentions dim1;
    TextInRectMm(LastX + 1, LastY + 1, LastX + WulkTimeProgWidth + WulkSalaryProgWidth -1, LastY + 1 + dim.TextHeight, LoadStr(sWulkanProg), DT_WORDBREAK | DT_CENTER | DT_VCENTER, &dim1);
    LineAtMm(LastX, LastY + dim1.TextHeight + 1, LastX + WulkTimeProgWidth + WulkSalaryProgWidth, LastY + dim1.TextHeight + 1, 0.25, psSolid);
    TextInRectMm(LastX + 0.5, LastY + dim1.TextHeight + 1, LastX + WulkTimeProgWidth -0.5, LastY + ReportTableHeaderHeight, LoadStr(sProgTime), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LastX += WulkTimeProgWidth;
    TextInRectMm(LastX + 1, LastY + dim1.TextHeight + 1, LastX + WulkSalaryProgWidth -1, LastY + ReportTableHeaderHeight, LoadStr(sSalary), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LineAtMm(LastX, LastY + dim1.TextHeight + 1, LastX, DrawHeight, 0.25, psSolid);
  }
  LastX += WulkSalaryProgWidth;

  if (DrawMode == dm_Init) {
    TextInRectMm(LastX + 1, LastY + 1, LastX + CutTimeProgWidth + CutSalaryProgWidth -1, 0, LoadStr(sCutProg), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    TTextDimentions dim1;
    TextInRectMm(LastX + 0.5, LastY + dim.TextHeight + 1, LastX + CutTimeProgWidth -0.5, 0, LoadStr(sProgTime), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim1);
    dim1.TextHeight += 2;
    LastX += CutTimeProgWidth;
    TTextDimentions dim2;
    TextInRectMm(LastX + 1, LastY + dim.TextHeight + 1, LastX + CutSalaryProgWidth -1, 0, LoadStr(sSalary), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim2);
    dim2.TextHeight += 2;
    dim.TextHeight += max(dim1.TextHeight, dim2.TextHeight);
    if (dim.TextHeight > ReportTableHeaderHeight)
      ReportTableHeaderHeight = dim.TextHeight;
  } else {
    TextInRectMm(LastX + 1, LastY + 1, LastX + CutTimeProgWidth + CutSalaryProgWidth -1, 0, LoadStr(sCutProg), DT_WORDBREAK | DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
    dim.TextHeight += 2;
    TTextDimentions dim1;
    TextInRectMm(LastX + 1, LastY + 1, LastX + CutTimeProgWidth + CutSalaryProgWidth -1, LastY + 1 + dim.TextHeight, LoadStr(sCutProg), DT_WORDBREAK | DT_CENTER | DT_VCENTER, &dim1);
    LineAtMm(LastX, LastY + dim1.TextHeight + 1, LastX + CutTimeProgWidth + CutSalaryProgWidth, LastY + dim1.TextHeight + 1, 0.25, psSolid);
    TextInRectMm(LastX + 0.5, LastY + dim1.TextHeight + 1, LastX + CutTimeProgWidth -0.5, LastY + ReportTableHeaderHeight, LoadStr(sProgTime), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LastX += CutTimeProgWidth;
    TextInRectMm(LastX + 1, LastY + dim1.TextHeight + 1, LastX + CutSalaryProgWidth -1, LastY + ReportTableHeaderHeight, LoadStr(sSalary), DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
    LineAtMm(LastX, LastY + dim1.TextHeight + 1, LastX, DrawHeight, 0.25, psSolid);
  }
  LastX += CutSalaryProgWidth;

  if (RubberStudio->ProgramContents_sortBy == sb_Customer) {
    DrawTableCell(LoadStr(sEdPrice), EdPriceWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);
    DrawTableCell(LoadStr(sPrice), PriceWidth, DT_WORDBREAK | DT_CENTER | DT_VCENTER, ReportTableHeaderHeight);
  }

// Границы таблицы и линии. Пока до низа страницы.
  TableTop = LastY;
  double TableOffsetX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
  RectAtMm(TableOffsetX, LastY, TableOffsetX + ReportTableWidth, LastY + ReportTableHeaderHeight, clWhite, bsClear, clBlack, psSolid, 0.5, true);
  if (RubberStudio->ProgramContents_sortBy != sb_TechMap) {
    TableOffsetX += TechCardWidth;
    LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  }
  if (RubberStudio->ProgramContents_sortBy != sb_Name) {
    TableOffsetX += NameWidth;
    LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  }
  TableOffsetX += PressFormWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += ToProduceWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += RubberSortWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += DetWeightWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += ZagWeightWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += RubberQuantityWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += WulkTimeWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += LoadTimeWidth;
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += (WulkTimeNormaWidth + WulkCostWidth);
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += (CutTimeNormaWidth + CutCostWidth);
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += (WulkTimeProgWidth + WulkSalaryProgWidth);
  LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  TableOffsetX += (CutTimeProgWidth + CutSalaryProgWidth);
  if (RubberStudio->ProgramContents_sortBy != sb_Name && RubberStudio->ProgramContents_sortBy != sb_TechMap)
    LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  if (RubberStudio->ProgramContents_sortBy == sb_Customer) {
    TableOffsetX += EdPriceWidth;
    LineAtMm(TableOffsetX, LastY, TableOffsetX, DrawHeight, 0.25, psSolid);
  }
  LastY += ReportTableHeaderHeight;
  return false;
}


// Вычисление стоимости группы (при переходе к другому дополнению
// суммирование начинается снова), если стоит разбивка по дополнениям
AnsiString TPlProgramReport::GetGroupPrice(AnsiString GroupTitle, AnsiString GroupFieldName, AnsiString Addition)
{
  int i = 0;
  double res = 0;
  AnsiString group_value = ReportData->FieldByName(GroupFieldName)->AsString;
  AnsiString addition_str = Addition;
  while ((group_value == GroupTitle && addition_str == Addition) && !ReportData->Eof)
  {
    res += RoundDouble(ReportData->FieldByName("Cost")->AsFloat * (double)ReportData->FieldByName("Amount")->AsInteger);
    ReportData->Next();
    i++;
    group_value = ReportData->FieldByName(GroupFieldName)->AsString;
    if (RubberStudio->PrSortByPartM->Checked && !ReportData->FieldByName("MainPart")->AsBoolean)
      addition_str = MiscFormat(LoadStr(sAdditionTitle).c_str(), ReportData->FieldByName("CorrectionDate")->AsDateTime.FormatString(LoadStr(sAdditionDateFormat)));
  }
  if (ReportData->Eof)
    i--;
  for (int j = 0; j < i; j++)
    ReportData->Prior();
  return RoundDoubleToAnsiString(res);
}

// Вычисление количества в группе (при переходе к другому дополнению
// суммирование начинается снова), если стоит разбивка по дополнениям
AnsiString TPlProgramReport::GetGroupAmount(AnsiString GroupTitle, AnsiString GroupFieldName, AnsiString Addition)
{
  int i = 0;
  int res = 0;
  AnsiString group_value = ReportData->FieldByName(GroupFieldName)->AsString;
  AnsiString addition_str = Addition;
  while ((group_value == GroupTitle && addition_str == Addition) && !ReportData->Eof)
  {
    res += ReportData->FieldByName("Amount")->AsInteger;
    ReportData->Next();
    i++;
    group_value = ReportData->FieldByName(GroupFieldName)->AsString;
    if (RubberStudio->PrSortByPartM->Checked && !ReportData->FieldByName("MainPart")->AsBoolean)
      addition_str = MiscFormat(LoadStr(sAdditionTitle).c_str(), ReportData->FieldByName("CorrectionDate")->AsDateTime.FormatString(LoadStr(sAdditionDateFormat)));
  }
  if (ReportData->Eof)
    i--;
  for (int j = 0; j < i; j++)
    ReportData->Prior();
  return res;
}

// Отрисовка названия группы (группирование по наим., тех. карте или заказчику)
bool TPlProgramReport::DrawTableGroup(AnsiString GroupTitle, AnsiString GroupFieldName, AnsiString Addititon) {
  TTextDimentions dim;
  TextInRectMm(0, 0, 0, 0, GroupTitle, DT_CALCRECT, &dim);
  double checkrecordheight = (DrawMode == dm_Init) ? dim.TextHeight : RecordHeight;
  if (CheckEndDraw(LastY + TableGroupHeight + checkrecordheight + 1 + 0.5))
    return true;
  if (RubberStudio->ProgramContents_sortBy != sb_Customer)
    GroupTitle += MiscFormat(LoadStr(sGroupAmount).c_str(), GetGroupAmount(GroupTitle, GroupFieldName, Addititon));
  else
    GroupTitle += MiscFormat(LoadStr(sGroupPrice).c_str(), GetGroupPrice(GroupTitle, GroupFieldName, Addititon));
  TFontStyles style;
  style.Clear();
  style << fsBold << fsItalic;
  double TableOffsetX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
  RectAtMm(TableOffsetX, LastY, TableOffsetX + ReportTableWidth, LastY + TableGroupHeight, TColor(CustFillColor), bsSolid, clBlack, psSolid, 0.25, true);
  SetFont("Arial", style, 10);
  TextInRectMm(TableOffsetX, LastY - 0.25, TableOffsetX + ReportTableWidth, LastY + TableGroupHeight, GroupTitle, DT_WORDBREAK | DT_CENTER | DT_VCENTER, NULL);
  LastY += TableGroupHeight;
  return false;
}

// Отрисовка ячейки
void TPlProgramReport::DrawTableCell(AnsiString &CellText, double CellWidth, UINT Format, double &MaxHeight)
{
  if (!CellText.Length())
    CellText = LoadStr(sUnknowValue);
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
void TPlProgramReport::CutTable() {
  if (DrawMode != dm_Init) {
    double TableHeight = PageHeightMm - OffsetBottom - OffsetTop - MiscOffset;
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

// Отрисовка заголовка дополнения
bool TPlProgramReport::DrawReportAdditionTitle(AnsiString AdditionTitle)
{
  DrawingAdditionTitle = true;
  if (PrintingTargetAddition) {
    if (DrawingData)
        CutTable();
    LastY += MainOffset;

    if (DrawReportBottom()) {
      return true;
    }

    if (Page == FPageNumber && DrawMode != dm_Init) {
      return true;
    }
    BeginNewPage();
  }

  TFontStyles fontstyle;
  fontstyle.Clear();
  fontstyle << fsBold;
  SetFont("Arial", fontstyle, 16);

  TTextDimentions dim;
  TextInRectMm(3, LastY, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, AdditionTitle, DT_CENTER | DT_VCENTER, &dim);
  LastY += (dim.TextHeight + LineOffsetY);

  fontstyle.Clear();
  SetFont("Arial", fontstyle, 12);
  AnsiString str = "";
  switch (RubberStudio->ProgramContents_sortBy) {
    case sb_Name     : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByName).c_str()); break;
    case sb_TechMap  : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByTechMap).c_str()); break;
    case sb_Customer : str = MiscFormat(LoadStr(sGroupBy).c_str(), LoadStr(sGroupByCustomer).c_str()); break;
  }

  TextInRectMm(3, LastY, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, str, DT_CENTER | DT_VCENTER, &dim);
  LastY += (dim.TextHeight  + MainOffset + LineOffsetY);

  if (DrawingData) {
    DrawTable();
  }
  DrawingAdditionTitle = false;
  return false;
}

bool TPlProgramReport::DrawReportBottomItem(bool Left, AnsiString ItemTitle, AnsiString FieldName)
{
  AnsiString fmt = "%s\r\n%s _____________";

  AnsiString fstr = Data->Programs->FieldByName(FieldName)->AsString;
  if (!fstr.Length())
    fstr = "________________   ";

  AnsiString str = MiscFormat(fmt.c_str(), ItemTitle.c_str(), fstr.c_str());
  TTextDimentions dim;
  TextInRectMm(0, 0, 0, 0, str, DT_CALCRECT, &dim);

  if (CheckEndDraw(LastY + dim.TextHeight)){
    return true;
  }

  BottomItemTop = LastY;
  double TableOffsetX = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;

  double maxwidth = TechCardWidth + NameWidth + PressFormWidth + ToProduceWidth + RubberSortWidth +
               DetWeightWidth + ZagWeightWidth + RubberQuantityWidth + WulkTimeWidth + LoadTimeWidth + WulkTimeNormaWidth +
               WulkCostWidth + CutTimeNormaWidth + CutCostWidth + WulkTimeProgWidth + WulkSalaryProgWidth +
               CutTimeProgWidth + CutSalaryProgWidth + EdPriceWidth + PriceWidth;

  double offset = (ReportTableWidth/maxwidth)*40.0;

  if (Left)
    TextInRectMm(TableOffsetX + offset, LastY, TableOffsetX + offset + dim.TextWidth, LastY + dim.TextHeight + 1, str, DT_LEFT, NULL);
  else
    TextInRectMm(TableOffsetX + ReportTableWidth/2 + offset, LastY, TableOffsetX + ReportTableWidth/2 + offset + dim.TextWidth, LastY + dim.TextHeight + 1, str, DT_LEFT, NULL);
  LastY += dim.TextHeight;
  return false;
}

void TPlProgramReport::DrawReportBottomSum(double value, double CellWidth)
{
  AnsiString CellText = RoundDoubleToAnsiString(value);
  TTextDimentions dim;
  TextInRectMm((PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth) / 2 + 1, LastY + 1, LastX + CellWidth -1, 0, CellText, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS | DT_CALCRECT, &dim);
  dim.TextHeight += 2;
  TextInRectMm((PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth) / 2 + 1, LastY + 1, LastX + CellWidth -1, LastY + dim.TextHeight + 1, CellText, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, NULL);
  LastX += CellWidth;
}

bool TPlProgramReport::DrawReportBottom()
{
  TFontStyles style;
  style.Clear();
  SetFont("Arial Narrow", style, 7);

  LastY -= MainOffset;

  LastX = TechCardWidth + NameWidth + PressFormWidth + ToProduceWidth +
    RubberSortWidth + DetWeightWidth + ZagWeightWidth + RubberQuantityWidth +
    WulkTimeWidth + LoadTimeWidth + WulkTimeNormaWidth + WulkCostWidth +
    CutTimeNormaWidth + CutCostWidth + (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth) / 2;
  switch (RubberStudio->ProgramContents_sortBy) {
    case sb_Customer : break;
    case sb_TechMap  : LastX -= TechCardWidth; break;
    case sb_Name     : LastX -= NameWidth; break;
  }

  double lastX = LastX;
  TTextDimentions dim;
  dim.TextHeight = 0;
  dim.TextWidth = 0;
  if (DrawingAdditionTitle) {
    if (!MainPart) {
      TextInRectMm((PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth) / 2 + 1, LastY + 1, LastX + WulkTimeProgWidth -1, 0, RoundDoubleToAnsiString(CurrentvulcanProgramTime), DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS | DT_CALCRECT, &dim);
      dim.TextHeight += 2;
      if (CheckEndDraw(LastY + dim.TextHeight))
        return true;
      DrawReportBottomSum(CurrentvulcanProgramTime , WulkTimeProgWidth);
      DrawReportBottomSum(CurrentvulcanProgramPrice, WulkSalaryProgWidth);
      DrawReportBottomSum(CurrentcutProgramTime    , CutTimeProgWidth);
      DrawReportBottomSum(CurrentcutProgramPrice   , CutSalaryProgWidth);
      if (RubberStudio->ProgramContents_sortBy == sb_Customer) {
        LastX += EdPriceWidth;
        DrawReportBottomSum(CurrentcustomerProgramPrice, PriceWidth);
      }
      LastY += MainOffset;
      LastX = lastX;
    }
    if (DrawTotal())
      return true;
  } else {
    if (RubberStudio->PrSortByPartM->Checked && RubberStudio->PrPartSelectM->Count > 1) {
      if (!MainPart) {
        TextInRectMm((PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth) / 2 + 1, LastY + 1, LastX + WulkTimeProgWidth -1, 0, RoundDoubleToAnsiString(CurrentvulcanProgramTime), DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS | DT_CALCRECT, &dim);
        dim.TextHeight += 2;
        if (CheckEndDraw(LastY + dim.TextHeight))
          return true;
        DrawReportBottomSum(CurrentvulcanProgramTime , WulkTimeProgWidth);
        DrawReportBottomSum(CurrentvulcanProgramPrice, WulkSalaryProgWidth);
        DrawReportBottomSum(CurrentcutProgramTime    , CutTimeProgWidth);
        DrawReportBottomSum(CurrentcutProgramPrice   , CutSalaryProgWidth);
        if (RubberStudio->ProgramContents_sortBy == sb_Customer) {
          LastX += EdPriceWidth;
          DrawReportBottomSum(CurrentcustomerProgramPrice, PriceWidth);
        }
        LastY += MainOffset;
        LastX = lastX;
      }
    }

    if (DrawTotal())
      return true;

  }

  LastY += MainOffset * 1.5;

  style.Clear();
  SetFont("Arial", style, 10);

  if (DrawReportBottomItem(true, LoadStr(sCompanyCommercialChief), "CompanyCommercialChief"))
    return true;
  LastY = BottomItemTop;
  if (DrawReportBottomItem(false, LoadStr(sCompanyEconomist), "CompanyEconomist"))
    return true;
  return false;
}

bool TPlProgramReport::DrawTotal(void)
{
  TFontStyles style;
  style.Clear();
  style << fsBold;
  SetFont("Arial Narrow", style, 7);
  LastX -= WulkTimeProgWidth;
  AnsiString text = LoadStr(sProgSum);
  TTextDimentions dim;
  TextInRectMm((PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth) / 2 + 1, LastY + 1, LastX + WulkTimeProgWidth -1, 0, text, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS | DT_CALCRECT, &dim);
  dim.TextHeight += 2;
  if (CheckEndDraw(LastY + dim.TextHeight))
    return true;
  TextInRectMm((PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth) / 2 + 1, LastY + 1, LastX + WulkTimeProgWidth -1, LastY + dim.TextHeight + 1, text, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, NULL);
  LineAtMm(LastX + WulkTimeProgWidth - dim.TextWidth - 1, LastY + dim.TextHeight + 0.5, ReportTableWidth + (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth) / 2, LastY + dim.TextHeight +  0.5, 0.25, psSolid);
  LastX += WulkTimeProgWidth;
  DrawReportBottomSum(TotalvulcanProgramTime , WulkTimeProgWidth);
  DrawReportBottomSum(TotalvulcanProgramPrice, WulkSalaryProgWidth);
  DrawReportBottomSum(TotalcutProgramTime    , CutTimeProgWidth);
  DrawReportBottomSum(TotalcutProgramPrice   , CutSalaryProgWidth);
  if (RubberStudio->ProgramContents_sortBy == sb_Customer) {
    LastX += EdPriceWidth;
    DrawReportBottomSum(TotalcustomerProgramPrice, PriceWidth);
  }
  LastY += MainOffset;
  return false;
}

// Окончание отрисовки
void TPlProgramReport::EndDrawing(TCursor OldCursor)
{
  if (DrawingData && !DrawingAdditionTitle) {
    CutTable();
    DrawingData = false;
  }
  DrawFooter();
  ReportData->First();
  ReportData->EnableControls();
  Screen->Cursor = OldCursor;
}

// Собственно отрисовка
void TPlProgramReport::DrawReport(int PageNumber)
{
  PrintingTargetAddition = false;
  if (!AdditionToPrint.Length() && PrintMainPart)
    PrintingTargetAddition = true;
  if (!RubberStudio->isPrintByPrPart())
    PrintingTargetAddition = true;

  FPageNumber = PageNumber;
  TCursor oldcur = Screen->Cursor;
  LastY = 0;

  ReportData->DisableControls();
  try {
    Screen->Cursor = crHourGlass;
    ReportData->First();
    if (Previewing)
      RectAtMm(-OffsetLeft - MainOffset, -OffsetTop - MainOffset, PageWidthMm - OffsetLeft + MainOffset, PageHeightMm - OffsetTop + MainOffset, clWhite, bsSolid, clWhite, psSolid, 1);
    if (PrintingTargetAddition && DrawReportTop()) {
      EndDrawing(oldcur);
      return;
    }
    if (PrintingTargetAddition)
      LastY += MainOffset;
    if (PrintingTargetAddition && DrawReportTitle()) {
      EndDrawing(oldcur);
      return;
    }
    if (PrintingTargetAddition)
      LastY += MainOffset;
    if (PrintingTargetAddition && DrawTable()) {
      EndDrawing(oldcur);
      return;
    }
    double tableoffset = (PageWidthMm - OffsetLeft - OffsetRight - ReportTableWidth)/2;
    AnsiString group_field_name;
    switch (RubberStudio->ProgramContents_sortBy) {
      case sb_Name     : group_field_name = "ProductName"; break;
      case sb_TechMap  : group_field_name = "TechMap"; break;
      case sb_Customer : group_field_name = "CustomerName"; break;
    }
    AnsiString previus_group_value = "";
    AnsiString previus_addition_string = "";

    int count = ReportData->RecordCount;
    if (DrawMode == dm_Init)
      RecordHeight = 0;

    AnsiString str = "";
    DrawingData = true;
    DrawingAdditionTitle = false;
    LastX = tableoffset;

    CurrentvulcanProgramTime    = 0;
    CurrentvulcanProgramPrice   = 0;
    CurrentcutProgramTime       = 0;
    CurrentcutProgramPrice      = 0;
    CurrentcustomerProgramPrice = 0;

    TotalvulcanProgramTime    = 0;
    TotalvulcanProgramPrice   = 0;
    TotalcutProgramTime       = 0;
    TotalcutProgramPrice      = 0;
    TotalcustomerProgramPrice = 0;


    if (RubberStudio->PrSortByPartM->Checked)
      MainPart = ReportData->FieldByName("MainPart")->AsBoolean;
    else
      MainPart = true;

// Отрисовка данных
    for (int i = 0; i < count; i++) {
      LastX = tableoffset;
      if (RubberStudio->isPrintByPrPart() && PrintingTargetAddition && PrintMainPart && !ReportData->FieldByName("MainPart")->AsBoolean)
        break;
      if (Data->LocateProduct(ReportData->FieldByName("ProductID")->AsInteger)) {
        if (RubberStudio->PrSortByPartM->Checked && !ReportData->FieldByName("MainPart")->AsBoolean) {
          AnsiString addition_string = MiscFormat(LoadStr(sAdditionTitle).c_str(), ReportData->FieldByName("CorrectionDate")->AsDateTime.FormatString(LoadStr(sAdditionDateFormat)));
          if (PrintingTargetAddition && RubberStudio->isPrintByPrPart() && addition_string != AdditionToPrint)
            break;
          if (!RubberStudio->isPrintByPrPart() && addition_string != previus_addition_string) {
            TotalvulcanProgramTime    += CurrentvulcanProgramTime;
            TotalvulcanProgramPrice   += CurrentvulcanProgramPrice;
            TotalcutProgramTime       += CurrentcutProgramTime;
            TotalcutProgramPrice      += CurrentcutProgramPrice;
            TotalcustomerProgramPrice += CurrentcustomerProgramPrice;

            if (DrawReportAdditionTitle(addition_string)) {
              EndDrawing(oldcur);
              return;
            }
            previus_addition_string = addition_string;
            previus_group_value = "";
            LastX = tableoffset;
            CurrentvulcanProgramTime    = 0;
            CurrentvulcanProgramPrice   = 0;
            CurrentcutProgramTime       = 0;
            CurrentcutProgramPrice      = 0;
            CurrentcustomerProgramPrice = 0;
          }
          MainPart = false;
          if (RubberStudio->isPrintByPrPart() && addition_string == AdditionToPrint && !PrintingTargetAddition) {
            DrawHeader();
            previus_addition_string = addition_string;
            TotalvulcanProgramTime    += CurrentvulcanProgramTime;
            TotalvulcanProgramPrice   += CurrentvulcanProgramPrice;
            TotalcutProgramTime       += CurrentcutProgramTime;
            TotalcutProgramPrice      += CurrentcutProgramPrice;
            TotalcustomerProgramPrice += CurrentcustomerProgramPrice;

            if (DrawReportAdditionTitle(addition_string)) {
              EndDrawing(oldcur);
              return;
            }
            PrintingTargetAddition = true;

            CurrentvulcanProgramTime    = 0;
            CurrentvulcanProgramPrice   = 0;
            CurrentcutProgramTime       = 0;
            CurrentcutProgramPrice      = 0;
            CurrentcustomerProgramPrice = 0;
            previus_group_value = "";
            LastX = tableoffset;
          }
        }
        AnsiString group_value = ReportData->FieldByName(group_field_name)->AsString;
        if (group_value != previus_group_value) {
          if (PrintingTargetAddition && DrawTableGroup(group_value, group_field_name, previus_addition_string)) {
            EndDrawing(oldcur);
            return;
          }
          previus_group_value = group_value;
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
        SetFont("Arial Narrow", style, 7);
        if (PrintingTargetAddition && RubberStudio->ProgramContents_sortBy != sb_TechMap) {
          str = ReportData->FieldByName("TechMap")->AsString;
          DrawTableCell(str, TechCardWidth, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);
        }

        if (PrintingTargetAddition && RubberStudio->ProgramContents_sortBy != sb_Name) {
          str = ReportData->FieldByName("ProductName")->AsString;
          DrawTableCell(str, NameWidth, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);
        }

        str = MiscFormat("%d/%d ", Data->Products->FieldByName("PFNumber")->AsInteger, Data->Products->FieldByName("PFPlaces")->AsInteger);
        if (PrintingTargetAddition) {
          if (Data->Products->FieldByName("PFCylindric")->AsBoolean) {
            TextInRectMm(LastX + 1, LastY + 1, LastX + PressFormWidth -1, 0, str, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_CALCRECT, &dim);
            dim.TextHeight += 2;
            if (DrawMode == dm_Init) {
              if (dim.TextHeight > RecordHeight)
                RecordHeight = dim.TextHeight;
              } else
                TextInRectMm(LastX + 1, LastY + 1, LastX + PressFormWidth -1, LastY + RecordHeight, str, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, NULL);

            str = '\306';
            SetFont("Symbol", style, 7);
            double width = dim.TextWidth;
            TextInRectMm(LastX + 1 + width, LastY + 1, LastX + PressFormWidth -1, 0, str, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_CALCRECT, &dim);
            dim.TextHeight += 2;
            if (DrawMode == dm_Init) {
              if (dim.TextHeight > RecordHeight)
                RecordHeight = dim.TextHeight;
              } else
                TextInRectMm(LastX + 1  + width, LastY + 1, LastX + PressFormWidth -1, LastY + RecordHeight, str, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, NULL);
            width += dim.TextWidth;
            SetFont("Arial Narrow", style, 7);
            str = MiscFormat("%dx%d", Data->Products->FieldByName("PFDiameter")->AsInteger, Data->Products->FieldByName("PFHeight")->AsInteger);
            TextInRectMm(LastX + 1 + width, LastY + 1, LastX + PressFormWidth -1, 0, str, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_CALCRECT, &dim);
            dim.TextHeight += 2;
            if (DrawMode == dm_Init) {
              if (dim.TextHeight > RecordHeight)
                RecordHeight = dim.TextHeight;
              } else
                TextInRectMm(LastX + 1 + width, LastY + 1, LastX + PressFormWidth -1, LastY + RecordHeight, str, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, NULL);
          } else {
            str = MiscFormat("%s%dx%dx%d", str.c_str(), Data->Products->FieldByName("PFLength")->AsInteger, Data->Products->FieldByName("PFWidth")->AsInteger, Data->Products->FieldByName("PFHeight")->AsInteger);
            TextInRectMm(LastX + 1, LastY + 1, LastX + PressFormWidth -1, 0, str, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_CALCRECT, &dim);
            dim.TextHeight += 2;
            if (DrawMode == dm_Init) {
              if (dim.TextHeight > RecordHeight)
                RecordHeight = dim.TextHeight;
              } else
                TextInRectMm(LastX + 1, LastY + 1, LastX + PressFormWidth -1, LastY + RecordHeight, str, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, NULL);
          }
          LastX += PressFormWidth;
          str = "";
        }

        if (PrintingTargetAddition) {
          str = ReportData->FieldByName("Amount")->AsString;
          DrawTableCell(str, ToProduceWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

          str = Data->Products->FieldByName("RubName")->AsString;
          DrawTableCell(str, RubberSortWidth, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

          str = Data->Products->FieldByName("Weight")->AsFloat;
          DrawTableCell(str, DetWeightWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

          str = Data->Products->FieldByName("ZagWeight")->AsFloat;
          DrawTableCell(str, ZagWeightWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

          str = Data->Products->FieldByName("ZagWeight")->AsFloat / 1000.0 * (double)ReportData->FieldByName("Amount")->AsInteger;
          DrawTableCell(str, RubberQuantityWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

          str = Data->Products->FieldByName("VulcanTime")->AsFloat;
          DrawTableCell(str, WulkTimeWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

          str = Data->Products->FieldByName("LoadTime")->AsFloat;
          DrawTableCell(str, LoadTimeWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

          str = RoundDoubleToAnsiString(480.0 / Data->Products->FieldByName("VulcanNorma")->AsFloat);
          DrawTableCell(str, WulkTimeNormaWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);
        }

        double norma = Data->Products->FieldByName("VulcanNorma")->AsFloat;
        double koef;
        if (ReportData->FieldByName("Vulcan5Class")->AsBoolean) {
          koef = ReportData->FieldByName("VulcanCostTariff5")->AsFloat;
        } else {
          koef = ReportData->FieldByName("VulcanCostTariff4")->AsFloat;
        }
        str = RoundDoubleToAnsiString(koef * 8.0 / norma, 3);
        if (PrintingTargetAddition)
          DrawTableCell(str, WulkCostWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

        str = RoundDoubleToAnsiString(480.0 / Data->Products->FieldByName("CutNorma")->AsFloat);
        if (PrintingTargetAddition)
          DrawTableCell(str, CutTimeNormaWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

        norma = Data->Products->FieldByName("CutNorma")->AsFloat;
        koef  = ReportData->FieldByName("CutCostTariff")->AsFloat;
        str = RoundDoubleToAnsiString(koef * 8.0 / norma, 3);
        if (PrintingTargetAddition)
          DrawTableCell(str, CutCostWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

        double val = RoundDouble(RoundDouble(480.0 / Data->Products->FieldByName("VulcanNorma")->AsFloat) / 60.0 * (double)ReportData->FieldByName("Amount")->AsInteger);
        CurrentvulcanProgramTime += val;
        str = RoundDoubleToAnsiString(val);
        if (PrintingTargetAddition)
          DrawTableCell(str, WulkTimeProgWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

        norma = Data->Products->FieldByName("VulcanNorma")->AsFloat;
        if (ReportData->FieldByName("Vulcan5Class")->AsBoolean) {
          koef = ReportData->FieldByName("VulcanCostTariff5")->AsFloat;
        } else {
          koef = ReportData->FieldByName("VulcanCostTariff4")->AsFloat;
        }
        val = RoundDouble(RoundDouble(koef * 8.0 / norma, 3) * (double)ReportData->FieldByName("Amount")->AsInteger);
        CurrentvulcanProgramPrice += val;
        str = RoundDoubleToAnsiString(val);
        if (PrintingTargetAddition)
          DrawTableCell(str, WulkSalaryProgWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

        val = RoundDouble(RoundDouble(480.0 / Data->Products->FieldByName("CutNorma")->AsFloat) / 60.0 * (double)ReportData->FieldByName("Amount")->AsInteger);
        CurrentcutProgramTime += val;
        str = RoundDoubleToAnsiString(val);
        if (PrintingTargetAddition)
          DrawTableCell(str, CutTimeProgWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

        norma = Data->Products->FieldByName("CutNorma")->AsFloat;
        koef  = ReportData->FieldByName("CutCostTariff")->AsFloat;
        val = RoundDouble(RoundDouble(koef * 8.0 / norma, 3) * (double)ReportData->FieldByName("Amount")->AsInteger);
        CurrentcutProgramPrice += val;
        str = RoundDoubleToAnsiString(val);
        if (PrintingTargetAddition)
          DrawTableCell(str, CutSalaryProgWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

        if (RubberStudio->ProgramContents_sortBy == sb_Customer) {
          str = ReportData->FieldByName("Cost")->AsFloat;
          if (PrintingTargetAddition)
            DrawTableCell(str, EdPriceWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);

          val = RoundDouble(ReportData->FieldByName("Cost")->AsFloat * (double)ReportData->FieldByName("Amount")->AsInteger);
          CurrentcustomerProgramPrice += val;
          str = RoundDoubleToAnsiString(val);
          if (PrintingTargetAddition)
            DrawTableCell(str, PriceWidth, DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS, RecordHeight);
        }

// Комплектующие
        int rmcount = Data->RawMaterialInProduct->RecordCount;
        AnsiString rm_str = "";
        if (PrintingTargetAddition && rmcount) {
          for (int j = 0; j < rmcount; j++) {
            if (j)
              rm_str += "; ";
            int rm_id = Data->RawMaterialInProduct->FieldByName("RawMaterialID")->AsInteger;
            TLocateOptions locate;
            locate.Clear();
            AnsiString str;
            if (Data->RawMaterial->Locate("RawMaterialID", rm_id, locate)) {
              str = Data->RawMaterial->FieldByName("RawMaterialName")->AsString;
            } else {
              str = "";
            }
            rm_str += str;

            double rm_weight = Data->RawMaterialInProduct->FieldByName("RMWeight")->AsFloat;
            if (rm_weight == 0.0)
              str = "";
            else
              str = MiscFormat(LoadStr(sGramm).c_str(), rm_weight);
            rm_str += str;

            AnsiString comment = Data->RawMaterialInProduct->FieldByName("RMCommentInProduct")->AsString;
            if (comment.Length())
              str = MiscFormat(" - %s", comment.c_str());
            else
              str = "";
            rm_str += str;
            Data->RawMaterialInProduct->Next();
          }
          TextInRectMm(0, 0, 0, 0, rm_str, DT_CENTER | DT_VCENTER | DT_CALCRECT, &dim);
          if (CheckEndDraw(LastY + RecordHeight + dim.TextHeight + 0.5)){
            LastY += RecordHeight;
            EndDrawing(oldcur);
            LastY -= RecordHeight;
            return;
          }
          if (DrawMode != dm_Init) {
            TBrushStyle brstyle = DrawTo->Brush->Style;
            TColor brcolor = DrawTo->Brush->Color;

            DrawTo->Brush->Style = bsSolid;
            DrawTo->Brush->Color = clWhite;
            RectAtMm((ReportTableWidth - dim.TextWidth)/2 - 2, LastY + RecordHeight - 1, (ReportTableWidth + dim.TextWidth)/2 + 2, LastY + RecordHeight + dim.TextHeight - 1, clWhite, bsSolid, clWhite, psClear, 1);
            TextInRectMm(tableoffset, LastY + RecordHeight - 1, tableoffset + ReportTableWidth, LastY + RecordHeight + dim.TextHeight - 1, rm_str, DT_CENTER | DT_VCENTER, NULL);
            DrawTo->Brush->Color = brcolor;
            DrawTo->Brush->Style = brstyle;
          }
          LastY += dim.TextHeight;
        }
// Комплектующие
        if (PrintingTargetAddition)
          LastY += RecordHeight;
      }
      ReportData->Next();
    }
    DrawingData = false;
    CutTable();
    LastY += MainOffset;

    TotalvulcanProgramTime    += CurrentvulcanProgramTime;
    TotalvulcanProgramPrice   += CurrentvulcanProgramPrice;
    TotalcutProgramTime       += CurrentcutProgramTime;
    TotalcutProgramPrice      += CurrentcutProgramPrice;
    TotalcustomerProgramPrice += CurrentcustomerProgramPrice;

    if (DrawReportBottom()) {
      EndDrawing(oldcur);
      return;
    }
    EndDrawing(oldcur);
  } catch (Exception& e) {
    Screen->Cursor = oldcur;
    ReportData->EnableControls();
  }
}


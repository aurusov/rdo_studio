//---------------------------------------------------------------------------
#ifndef Pl_Program_ReportH
#define Pl_Program_ReportH
//---------------------------------------------------------------------------
#include "Pl_Base_Report.h"

//---------------------------------------------------------------------------
class TPlProgramReport :  public TPlBaseReport
{
private:
  double TableTop;
  int FindSQLSubstring(AnsiString Substring);
  void OpenReportDataSource();
  int FPageNumber;
  double LineOffsetY;
  bool CheckEndDraw(double Value);
  double ReportTableWidth;
  double LastX;
  bool DrawingData;
  double ReportTableHeaderHeight;
  bool DrawReportTop();
  bool DrawReportTitle();
  bool DrawTable();
  AnsiString GetGroupPrice(AnsiString GroupTitle, AnsiString GroupFieldName, AnsiString Addition);
  AnsiString GetGroupAmount(AnsiString GroupTitle, AnsiString GroupFieldName, AnsiString Addition);
  bool DrawTableGroup(AnsiString GroupTitle, AnsiString GroupFieldName, AnsiString Addititon);
  void CutTable();
  double RecordHeight;
  void DrawTableCell(AnsiString &CellText, double CellWidth, UINT Format, double &MaxHeight);
  bool DrawingAdditionTitle;
  bool DrawReportAdditionTitle(AnsiString AdditionTitle);
  double BottomItemTop;
  bool DrawReportBottomItem(bool Left, AnsiString ItemTitle, AnsiString FieldName);
  bool DrawTotal(void);

  double CurrentvulcanProgramTime;
  double CurrentvulcanProgramPrice;
  double CurrentcutProgramTime;
  double CurrentcutProgramPrice;
  double CurrentcustomerProgramPrice;

  double TotalvulcanProgramTime;
  double TotalvulcanProgramPrice;
  double TotalcutProgramTime;
  double TotalcutProgramPrice;
  double TotalcustomerProgramPrice;

  void DrawReportBottomSum(double value, double CellWidth);
  bool DrawReportBottom();
  void EndDrawing(TCursor OldCursor);

  AnsiString AdditionToPrint;
  bool PrintMainPart;
  bool PrintingTargetAddition;
  bool MainPart;
protected:
  void DrawReport(int PageNumber);
public:
  __fastcall TPlProgramReport();
  __fastcall TPlProgramReport(AnsiString Title);
  __fastcall ~TPlProgramReport();
};

#endif

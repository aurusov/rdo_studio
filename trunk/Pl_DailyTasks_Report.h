//---------------------------------------------------------------------------
#ifndef Pl_DailyTasks_ReportH
#define Pl_DailyTasks_ReportH
//---------------------------------------------------------------------------
#include "Pl_Base_Report.h"
//---------------------------------------------------------------------------
class TPlDailyTasksReport :  public TPlBaseReport
{
private:
  int FPageNumber;
  double TableTop;
  double LineOffsetY;
  bool CheckEndDraw(double Value);
  double ReportTableWidth;
  double LastX;
  bool DrawingData;
  bool DrawReportTop();
  bool DrawReportTitle();
  bool DrawTable();
  bool DrawSmenaTable(int Smena);
  bool DrawTableGroup(AnsiString GroupTitle, AnsiString GroupFieldName);
  void CutTable();
  double RecordHeight;
  void DrawTableCell(AnsiString &CellText, double CellWidth, UINT Format, double &MaxHeight);
  bool DrawingSmenaTitle;
  bool DrawFio(AnsiString FioTitle, AnsiString Fio);
  bool DrawBottomFio();
  void EndDrawing(TCursor OldCursor);
  AnsiString GetFio();
  AnsiString Plan;
protected:
  void DrawReport(int PageNumber);
public:
  __fastcall TPlDailyTasksReport();
  __fastcall TPlDailyTasksReport(const int work_day, AnsiString PlanName);
  __fastcall ~TPlDailyTasksReport();
};
#endif


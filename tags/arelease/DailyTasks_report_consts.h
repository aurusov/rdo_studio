//---------------------------------------------------------------------------
#ifndef DailyTasks_report_constsH
#define DailyTasks_report_constsH
#include "Report_Consts.h"
#include "Program_report_consts.h"
//---------------------------------------------------------------------------
#define TasksSignLineWidth 32 //Размер линии для подписи в мм
#define TasksGroupFillColor 14803425

// Таблица
#define FioWidth 30
#define PressNameWidth 30
#define DetNameWidth 30
#define TasksWulkTimeWidth 15
#define WulkTempWidth 15
#define PFNumberWidth 22
#define PressureWidth 21
#define QuantityWidth 17
#define QuantityInFactWidth 16
#define TasksTableHeaderHeight 15

#define TasksTableGroupHeight 6
// Таблица
#define TasksMiscOffset 3
#define TasksMainOffset 5

#define TasksReportID            ProgramReportID_End + 1
#define sTaskTitle               0 + TasksReportID
#define sGroupByPress            1 + TasksReportID
#define sGroupByPersonal         2 + TasksReportID
#define sTaskFio                 3 + TasksReportID
#define sTaskPress               4 + TasksReportID
#define sTaskDetName             5 + TasksReportID
#define sTaskWulkTemp            6 + TasksReportID
#define sTaskPFNumber            7 + TasksReportID
#define sTaskPressure            8 + TasksReportID
#define sTaskQuantity            9 + TasksReportID
#define sTaskQuantityInFact      10 + TasksReportID
#define sTaskSmenaTitle          13 + TasksReportID
#define sTaskGroupNo             14 + TasksReportID
#define TasksReportID_End        sTaskGroupNo

#endif



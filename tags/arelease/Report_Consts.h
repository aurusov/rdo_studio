//---------------------------------------------------------------------------
#ifndef Report_ConstsH
#define Report_ConstsH
#include "ConstRes.h"
//---------------------------------------------------------------------------
#define Inch 25.4 // Дюйм в миллиметрах
#define HeaderOffset 5 //Отступ отчета от хидера в мм.
#define FooterOffset 5 //Отступ отчета от футера в мм.

#define ReportID                 10000
#define sOptionsCaption          0 + ReportID
#define sPages                   1 + ReportID
#define sPrinted                 2 + ReportID
#define sPaperSize               3 + ReportID
#define sZoomFitBox              4 + ReportID
#define sZoomWidthBox            5 + ReportID
#define ReportID_End             sZoomWidthBox

#define ReportErrorID            ReportID_End + 1
#define sOffsetsError            0 + ReportErrorID
#define sNoPrintersError         1 + ReportErrorID
#define sScaleRangeError         2 + ReportErrorID
#define ReportErrorID_End        sScaleRangeError
#endif


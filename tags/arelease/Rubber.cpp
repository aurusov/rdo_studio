//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Rubber.res");
USEFORM("Form_Rubber.cpp", RubberStudio);
USEFORM("DataModule.cpp", Data);
USE("ConstRes.h", File);
USERES("RES_Model.RES");
USEUNIT("Misc.cpp");
USEUNIT("DrR_Exception.cpp");
USERES("RES_String.RES");
USEFORM("Dr_Form_About.cpp", AboutForm);
USEUNIT("Dr_MiscComponent.cpp");
USEFORM("Dr_Form_Program_Info.cpp", ProgramInfoForm);
USEFORM("Dr_Form_Product.cpp", ProductForm);
USEFORM("Dr_Form_Personal.cpp", PersonalForm);
USEFORM("Dr_Form_Hint.cpp", HintForm);
USEFORM("Dr_Form_Machine.cpp", MachineForm);
USEFORM("Dr_Form_Customer.cpp", CustomerForm);
USEFORM("Dr_Form_Program_Machine.cpp", MachineInProgForm);
USEFORM("Dr_Form_Select.cpp", SelectForm);
USEFORM("Dr_Form_Select_Customer.cpp", SelectCustomerForm);
USEFORM("Dr_Form_Select_Personal.cpp", SelectPersonalForm);
USEFORM("Dr_Form_Select_Product.cpp", SelectProductForm);
USEFORM("Dr_Form_Select_Program.cpp", SelectProgramForm);
USEFORM("Dr_Form_RawMaterial.cpp", RawMaterialForm);
USEFORM("Dr_Form_Date.cpp", DateForm);
USEFORM("Dr_Form_Select_Date.cpp", SelectDateForm);
USEFORM("Dr_Form_RawMaterialInProduct.cpp", RawMaterialInProductForm);
USEFORM("Dr_Form_Select_RawMaterial.cpp", SelectRawMaterialForm);
USEUNIT("Pl_Program_Report.cpp");
USEFORM("Pl_Form_ReportPreview.cpp", PreviewForm);
USEUNIT("Pl_Base_Report.cpp");
USERES("Report_Res_Bitmaps.RES");
USERES("Report_Res_String.RES");
USE("Report_Consts.h", File);
USEFORM("Pl_Form_Report_PreviewOptions.cpp", PreviewOptionsForm);
USE("Program_report_consts.h", File);
USERES("Program_Report_Res_String.RES");
USEFORM("Dr_Form_ProgramPart.cpp", ProgramPartForm);
USEUNIT("Pl_DailyTasks_Report.cpp");
USERES("Tasks_Report_Res_String.res");
USEFORM("Dr_Form_Splash.cpp", SplashForm);
USEFORM("Dr_Form_Plane_Info.cpp", PlaneInfo);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "";
                 Application->HelpFile = "C:\\Rubber\\Rubber.hlp";
                 Application->CreateForm(__classid(TRubberStudio), &RubberStudio);
                 Application->CreateForm(__classid(TData), &Data);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

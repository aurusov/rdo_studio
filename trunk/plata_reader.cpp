//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("plata_reader.res");
USEFORM("main_unit.cpp", Form1);
USE("res_const.h", File);
USERC("res_model.rc");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "RAO-platareader";
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("plane_fix.res");
USEFORM("Form_Fix.cpp", FormFix);
USEFORM("data.cpp", Planning); /* TDataModule: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFormFix), &FormFix);
                 Application->CreateForm(__classid(TPlanning), &Planning);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

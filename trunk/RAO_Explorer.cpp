//---------------------------------------------------------------------------

#include <vcl.h>
#include "Misc.h"
#pragma hdrstop
USERES("RAO_Explorer.res");
USEFORM("Main.cpp", Form1);
USEUNIT("HighlightText.cpp");
USEUNIT("Dr_PictureViewer.cpp");
USE("ConstRes.h", File);
USERES("Res_String.RES");
USERES("Res_Bitmaps.res");
USEUNIT("HighlightImage.cpp");
USEFORM("Form_Options.cpp", OptionsForm);
USEUNIT("Pl_CheckBox.cpp");
USEUNIT("Misc.cpp");
USEFORM("Message_Form.cpp", MessageForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "RAO Demo CD Explorer";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->Run();
        }
        catch (Exception &exception)
        {
//                 Application->ShowException(&exception);
                 ShowTypedMessage(exception.Message, MB_ICONSTOP);
        }
        return 0;
}
//---------------------------------------------------------------------------

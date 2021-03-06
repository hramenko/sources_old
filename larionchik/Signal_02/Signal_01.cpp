//===========================================================================

#include <vcl.h>
#pragma hdrstop
//===========================================================================
USEFORM("ProjectForms\frmMain\Unit_Main.cpp", frmMain);
USEFORM("ProjectForms\Unit_ACPSetting.cpp", frmACPSetting);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TfrmMain), &frmMain);
         Application->CreateForm(__classid(TfrmACPSetting), &frmACPSetting);
         Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//===========================================================================

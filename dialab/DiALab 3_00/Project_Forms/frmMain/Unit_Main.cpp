//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit_Main.h"
#include "Unit_Main_StandartVoids.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
   // ------------------
      Left   = 0;
      Top    = 0;
      Height = 730;

   // ------- ������������� ���� ���� � ���� ������� --------
      OpenDialog1->InitialDir =  ExtractFilePath( Application->ExeName );
      SaveDialog1->InitialDir =  ExtractFilePath( Application->ExeName );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
//
}

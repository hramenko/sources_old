//===========================================================================
#include <PCH.h>
#pragma hdrstop
//===========================================================================

#include "Unit_Main.h"
#include "Unit_Main_Registry.h"
#include "mpiCollection.h"
#include "mpiBaseADC.h"
#include "Unit_ACPSetting.h"
#include "mpiChart.h"
#include "mpiGlobals.h"
#include "mpi_Demo.h"

//===========================================================================
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;

//===========================================================================
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
        : TForm(Owner)
{
    Application->Title = ProjectCaption;
    this->Caption      = AnsiString(ProjectCaption) + " - []" ;
    pBGround->DoubleBuffered = true;
    frmMain ->DoubleBuffered = true;
}

//===========================================================================
void __fastcall TfrmMain::Idle(System::TObject* Sender, bool &Done)
{
 // ----- ���� ������ ����� ������, ����� ��������� -------
    if ( Chart != NULL && CurrentWorkDevice != NULL ) {
        if ( CurrentWorkDevice->isDataReady == true )
        {

            if (CurrentWorkDevice->DataCounter >= CurrentWorkDevice->WorkTime * CurrentWorkDevice->AcpCanalFreq * CurrentWorkDevice->BaseCanalCountBeforeMath )
            {
                bStopClick(Sender);
            }

            Chart->DrawSeriesValues();

            CurrentWorkDevice->isDataReady = false;
            DrawFPS           = DrawFPS + 1;
          // --------------
            QueryPerformanceCounter(&qPerfCounter2);
            if ( qPerfCounter2.QuadPart - qPerfCounter1.QuadPart >= qPerfFreq.QuadPart )
            {
                mConsole->Lines->BeginUpdate();
                      mConsole->Lines->Clear();
                      mConsole->Lines->Add("DrawFPS   = " + IntToStr(DrawFPS));
                      mConsole->Lines->Add("IrqCounter = " + IntToStr(CurrentWorkDevice->DataCounter));
                mConsole->Lines->EndUpdate();

                qPerfCounter1 = qPerfCounter2;
                DrawFPS = 0;
            }
        }
    }

  // --------------
    Done = false;
}

//===========================================================================
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
   // ----
     ReadDataFromRegistry();

     QueryPerformanceFrequency(&qPerfFreq);
     QueryPerformanceCounter(&qPerfCounter1);
     DrawFPS = 0;
  // ---------
     Chart         = new CChart(pChart);

     rgVoltScales->Top    = Label10->BoundsRect.Bottom;
     rgVoltScales->Height = Chart->VoltScales.Count * 16;
     rgTimeScales->Height = Chart->VoltScales.Count * 16;
     Label1->Top          = rgVoltScales->BoundsRect.Bottom + 14;
     rgTimeScales->Top    = Label1->BoundsRect.Bottom;
     
     rgVoltScales->Items->Clear();
     rgTimeScales->Items->Clear();          
     for ( unsigned int i = 0; i < Chart->VoltScales.Count; i++)
        rgVoltScales->Items->Add(FloatToStr(Chart->VoltScales.Value[i]));

     for ( unsigned int i = 0; i < Chart->VoltScales.Count; i++)
        rgTimeScales->Items->Add(FloatToStr(Chart->TimeScales.Value[i]));
  // ---------

     Application->OnIdle = Idle;
     SetInterfaceParam();
     SetEnableDisabled_VCL_Group(frmMain, "Panel1", false);
}

//===========================================================================
void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
   SaveDataToRegistry();
 // ------  
//   if ( CurrentWorkDevice != NULL ) delete CurrentWorkDevice;  CurrentWorkDevice = NULL;
   if ( Chart             != NULL ) delete Chart;              Chart             = NULL;
 // ------  
}

//===========================================================================
void TfrmMain::SetInterfaceParam()
{
   // ----- ����������� ������ -------
     rgVoltScales->ItemIndex = Chart->VoltScalesIndex;
     rgTimeScales->ItemIndex = Chart->TimeScalesIndex;


     if ( CurrentWorkDevice == NULL ) return;
   // --------
     SetEnableDisabled_VCL_Group(frmMain, "Panel1", false);

    CheckListBox1->Items->BeginUpdate();
    CheckListBox1->Items->Clear();

     for (  int i = 0; i < CurrentWorkDevice->Canals->ItemsCount; i++ )
     {
         CheckListBox1->Items->AddObject(CurrentWorkDevice->Canals->Items[i]->Name, (TObject*)CurrentWorkDevice->Canals->Items[i]);
         CheckListBox1->Checked[i] = CurrentWorkDevice->Canals->Items[i]->gr_Checked;
     }
    CheckListBox1->Items->EndUpdate();

    CheckListBox1->Height = CheckListBox1->Items->Count * CheckListBox1->ItemHeight + 10;
}

//===========================================================================
void __fastcall TfrmMain::bStartClick(TObject *Sender)
{
    if ( CurrentWorkDevice == NULL )
    {
        MessageBox(NULL, "����������� ����� � ��������� ���", "Signal - ������", MB_OK|MB_ICONERROR|MB_TASKMODAL);
        return;
    }

   // ------------ 
    bStart->Enabled = false;
    bStop ->Enabled = true;

    // ------- ������ ��������� ����� -----
    Chart->LastDrawPoint    = 0;
    Chart->sbMain->Position = 0;
    Chart->sbMain->Enabled  = false;
    Chart->BaseADC          = CurrentWorkDevice;

    // ----- ������ ��� -------
    CurrentWorkDevice->Start();

    // ------- ������ ��������� ����� -----
    Chart->pbMain->Repaint();
    Chart->Canals         = CurrentWorkDevice->Canals;
    Chart->isRealTimeWork = true;

    // -----
    SetInterfaceParam();

    // ----- �������������� ������ �� ������������ ��� ----
    bAutoPositionClick(Sender);

    // -----
    rgVoltScalesClick (Sender);
    rgTimeScalesClick (Sender);
}

//===========================================================================
void __fastcall TfrmMain::bStopClick(TObject *Sender)
{
    if ( CurrentWorkDevice == NULL ) return;

    CurrentWorkDevice->Stop();

    // ---- �������� ���������� ������ ���� -------
    Chart->sbMain->Enabled  = true;
    Chart->isRealTimeWork   = false;
    Chart->LastDrawPoint    = Chart->Canals->Items[0]->Count; // ----- ��������� �����, ������� ���� ���������� -----
    Chart->sbMain->Max      = ( Chart->TimeScales.Value[Chart->TimeScalesIndex] * CurrentWorkDevice->AcpCanalFreq > Chart->LastDrawPoint) ? 0 : Chart->LastDrawPoint;
    Chart->sbMain->Position = 1;
    Chart->sbMain->Position = 0;

    // ----
    bStart->Enabled = true;
    bStop ->Enabled = false;
}

//===========================================================================
void __fastcall TfrmMain::miSettingsClick(TObject *Sender)
{
 // ----------
    frmACPSetting->WorkACP = CurrentWorkDevice;
    frmACPSetting->ShowModal();
    if (  frmACPSetting->WorkACP != NULL )
    {
        CurrentWorkDevice = frmACPSetting->WorkACP;
        Chart->Canals = CurrentWorkDevice->Canals;
    }
 // ----------
    SetInterfaceParam();
 // ----------
    Chart->pbMain->Repaint();
}

//===========================================================================
void __fastcall TfrmMain::FormPaint(TObject *Sender)
{
   static bool isFirstInput = true;
   if ( isFirstInput == true)
   {
      isFirstInput = false;
      pBGroundResize(Sender);
   }
}

//===========================================================================
void __fastcall TfrmMain::pBGroundResize(TObject *Sender)
{
 // ---  ��������� ��������� ----
   Label9->Top = mConsole->Top - Label9->Height - 4;
 // ---  ��������� ��������� ----
   pChart->Top    = 10;
   pChart->Left   = pLeft->ClientRect.Right + pChart->Top;
   pChart->Width  = pManager->Left   - pLeft->ClientRect.Right - 2*pChart->Top + 1;
   pChart->Height = pBGround->Height - 2*pChart->Top + 1;
}

//===========================================================================
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
      if ( bStop->Enabled == true ) {
         bStopClick(Sender);
         Sleep(100);
      }
      CanClose = true;
}

//===========================================================================
void __fastcall TfrmMain::N4Click(TObject *Sender)
{
  Close();
}

//===========================================================================
void __fastcall TfrmMain::CheckListBox1Click(TObject *Sender)
{
   SetEnableDisabled_VCL_Group(frmMain, "Panel1", true);

   if ( CheckListBox1->ItemIndex < 0 ) return;
   CCollection<short int> *cc = (CCollection<short int> *)CheckListBox1->Items->Objects[CheckListBox1->ItemIndex];
   if ( cc == NULL ) return;

 // ------ ���� ---
   cbCanal->Selected = cc->gr_Color;

   Chart->Canals->Items[CheckListBox1->ItemIndex]->gr_Checked = CheckListBox1->Checked[CheckListBox1->ItemIndex];
   Chart->pbMain->Repaint();
}

//===========================================================================
void __fastcall TfrmMain::bAutoPositionClick(TObject *Sender)
{
    if ( CheckListBox1->Items->Count == 0 ) return;

    // ----- ���������� ���������� ��������� ---------
    int CountCheckedItems = 0;
    for ( int i = 0; i < CheckListBox1->Items->Count; i++ )
        if ( CheckListBox1->Checked[i] == true ) CountCheckedItems++;
    if ( CountCheckedItems == 0 ) CountCheckedItems = 1;    

    int IntervalHeight    = Chart-> DrawRect.Height() / (double) (CountCheckedItems + 1);

    int Counter = 0;
    for ( int i = 0; i < CheckListBox1->Items->Count; i++ )
    {
        CCollection<short int> *cc = (CCollection<short int> *)CheckListBox1->Items->Objects[i];
        if ( cc == NULL ) continue;

        if ( CheckListBox1->Checked[i] == true )
        {
            double min =  1000000;
            double max = -1000000;
            for ( int k = 0; k < cc->gr_ValuesCount; k++  )
            {
                if ( min > cc->Values[cc->gr_ArrayIndex[k]] ) min = cc->Values[cc->gr_ArrayIndex[k]];
                if ( max < cc->Values[cc->gr_ArrayIndex[k]] ) max = cc->Values[cc->gr_ArrayIndex[k]];
            }
            cc->ValuesOffset = min + (max - min)*0.5;
            cc->gr_YOffset = IntervalHeight * (++Counter);
        }
    }

    if ( Chart->isRealTimeWork == false ) Chart->DrawSeriesValues();
}

//===========================================================================
void __fastcall TfrmMain::cbCanalChange(TObject *Sender)
{
    if ( CheckListBox1->ItemIndex < 0 ) return;
    CCollection<short int> *cc = (CCollection<short int> *)CheckListBox1->Items->Objects[CheckListBox1->ItemIndex];

    cc->gr_Color = cbCanal->Selected;

    if ( Chart->isRealTimeWork == false ) Chart->pbMain->Repaint();
}

//===========================================================================
void __fastcall TfrmMain::FormActivate(TObject *Sender)
{
  DecimalSeparator = '.';
}

//===========================================================================
void __fastcall TfrmMain::rgTimeScalesClick(TObject *Sender)
{
    Chart->TimeScalesIndex = rgTimeScales->ItemIndex;
    Chart->SetChartTime(Chart->TimeScales.Value[Chart->TimeScalesIndex]);
    Chart->sbMain->Position = 1;
    Chart->sbMain->Position = 0;
    Chart->pbMain->Repaint();
}

//===========================================================================
void __fastcall TfrmMain::rgVoltScalesClick(TObject *Sender)
{
    // --------
    Chart->VoltScalesIndex = rgVoltScales->ItemIndex;
    Chart->DrawSeriesValues();

    if ( CheckListBox1->Items->Count != 0 )
    {
        for ( int i = 0; i < CheckListBox1->Items->Count; i++ )
        {
            CCollection<short int> *cc = (CCollection<short int> *)CheckListBox1->Items->Objects[i];
            if ( cc == NULL ) continue;

            cc->gr_MaxY  =  5.0 * Chart->VoltScales.Value[Chart->VoltScalesIndex];
            cc->gr_MinY  = -5.0 * Chart->VoltScales.Value[Chart->VoltScalesIndex];
            cc->gr_CoefY = cc->gr_Height / (cc->gr_MaxY - cc->gr_MinY);
        }
    }
    Chart->pbMain->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::miDataOpenClick(TObject *Sender)
{
    if ( OpenDialog1->Execute() == false ) return;

    FILE *h_file = fopen(OpenDialog1->FileName.c_str(), "rb");

    // ---------------
    CurrentWorkDevice = DemoADC;
    Chart->BaseADC = CurrentWorkDevice;
    // ---------------

    int        grVolt      = -1;
    int        grTime      = -1;
    double     Freq        = -1;
    int        CanalsCount = -1;
    char       CanalName[255];
    short int *vals        = NULL;
    double     gr_YOffset  = -1;
    int        gr_Color    = -1;
    bool       gr_Checked  = false;
    double     Scale       = -1;
    int        DataCount   = -1;

    fread(&grVolt     , sizeof(int   ), 1, h_file); //
    fread(&grTime     , sizeof(int   ), 1, h_file); //
    fread(&Freq       , sizeof(double), 1, h_file); //
    fread(&CanalsCount, sizeof(int   ), 1, h_file);
    
    DemoADC->AcpCanalFreq  = Freq;
    DemoADC->Canals->SetItemsCount(CanalsCount);
    DemoADC->BaseCanalCountBeforeMath = CanalsCount;
    Chart->Canals  = CurrentWorkDevice->Canals;

    for ( int i = 0; i < CanalsCount; i++ )
    {
        fread(&gr_YOffset , sizeof(double), 1, h_file);
        fread(&gr_Color   , sizeof(int   ), 1, h_file);
        fread(&gr_Checked , sizeof(bool  ), 1, h_file);
        fread(&Scale      , sizeof(double), 1, h_file);
        fread( CanalName  , sizeof(char) * 100, 1, h_file);
        fread(&DataCount  , sizeof(int   ), 1, h_file); // unsigned int

        vals = (short int*) malloc(sizeof(short int) * DataCount );
        fread(vals, sizeof(short int)*DataCount, 1, h_file);

        DemoADC->Canals->Items[i]->gr_YOffset            = gr_YOffset;
        DemoADC->Canals->Items[i]->gr_Color              = (TColor)gr_Color;
        DemoADC->Canals->Items[i]->gr_Checked            = gr_Checked;
        DemoADC->Canals->Items[i]->gr_YVoltTransformCoef = Scale;
        DemoADC->Canals->Items[i]->ValueT0               = 1 / (double)Freq;
        DemoADC->Canals->Items[i]->AddArray(vals, DataCount);

        free (vals);
    }

    fclose(h_file);

   // ---------------
    DemoADC->DataCounter = DataCount;

    Chart->VoltScalesIndex = grVolt;
    Chart->TimeScalesIndex = grTime;

    SetInterfaceParam();
    bStopClick(Sender);

    rgVoltScales->ItemIndex = Chart->VoltScalesIndex;
    rgTimeScales->ItemIndex = Chart->TimeScalesIndex;

    Chart->Canals  = CurrentWorkDevice->Canals;
    Chart->pbMain->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::miDataSaveClick(TObject *Sender)
{
    if ( CurrentWorkDevice == NULL ) return;
    if ( SaveDialog1->Execute() == false ) return;

    FILE *h_file = fopen(SaveDialog1->FileName.c_str(), "wb");

    int        grVolt      = Chart->VoltScalesIndex;
    int        grTime      = Chart->TimeScalesIndex;
    double     Freq        = CurrentWorkDevice->AcpCanalFreq;
    int        CanalsCount = CurrentWorkDevice->BaseCanalCountBeforeMath;
    char       CanalName[100];
    short int *vals;
    double     gr_YOffset  = -1;
    int        gr_Color    = -1;
    bool       gr_Checked  = false;
    double     Scale       = -1;
    int        DataCount   = -1;


    fwrite(&grVolt     , sizeof(int   ), 1, h_file); //
    fwrite(&grTime     , sizeof(int   ), 1, h_file); //
    fwrite(&Freq       , sizeof(double), 1, h_file); // double
    fwrite(&CanalsCount, sizeof(int   ), 1, h_file);

    for ( int i = 0; i < CanalsCount; i++ )
    {
        gr_YOffset = CurrentWorkDevice->Canals->Items[i]->gr_YOffset;
        gr_Color   = CurrentWorkDevice->Canals->Items[i]->gr_Color;
        gr_Checked = CurrentWorkDevice->Canals->Items[i]->gr_Checked;
        Scale      = CurrentWorkDevice->Canals->Items[i]->gr_YVoltTransformCoef;
        DataCount  = CurrentWorkDevice->Canals->Items[i]->Count;
        vals       = CurrentWorkDevice->Canals->Items[i]->Values;
        memcpy( CanalName, CurrentWorkDevice->Canals->Items[i]->Name, strlen(CurrentWorkDevice->Canals->Items[i]->Name));
        // --------------
        fwrite(&gr_YOffset , sizeof(double), 1, h_file);
        fwrite(&gr_Color   , sizeof(int   ), 1, h_file);
        fwrite(&gr_Checked , sizeof(bool  ), 1, h_file);
        fwrite(&Scale      , sizeof(double), 1, h_file);
        fwrite( CanalName  , sizeof(char) * 100, 1, h_file);
        fwrite(&DataCount  , sizeof(int   ), 1, h_file); // unsigned int
        fwrite( vals       , sizeof(short int)*DataCount, 1, h_file);
    }

    fclose(h_file);
}


void __fastcall TfrmMain::bAddCanalsClick(TObject *Sender)
{
    int ButtonTag = ((TButton*)Sender)->Tag;
    if ( CheckListBox1->Items->Count == 0 ) return;
    int CountCheckedItems = 0;
    for ( int i = 0; i < CheckListBox1->Items->Count; i++ )
        if ( CheckListBox1->Checked[i] == true ) CountCheckedItems++;

    if (  CountCheckedItems != 2 )
    {
        MessageBox(NULL, "�������� ��� ������� ��� ���������� ��������", "Signal - ������", MB_OK|MB_ICONERROR|MB_TASKMODAL);
        return;
    }

    // ----- ���������� ���������� ��������� ---------
    CurrentWorkDevice->Canals->Add();
    CCollection<short int> *BaseCol = NULL;;
    CCollection<short int> *NewCol  = CurrentWorkDevice->Canals->Items[CurrentWorkDevice->Canals->ItemsCount - 1];

    short int *Base    = NULL;
    short int *Second  = NULL;

    // -----
    for ( int i = 0; i < CheckListBox1->Items->Count; i++ )
        if ( CheckListBox1->Checked[i] == true )
        {
            if ( Base == NULL )
            {
                 BaseCol = (CCollection<short int> *)CheckListBox1->Items->Objects[i];
                 Base    = BaseCol->Values;
            }
            else Second = ((CCollection<short int> *)CheckListBox1->Items->Objects[i])->Values;
        }
    // -----
    NewCol->ValueT0                = BaseCol->ValueT0;
    NewCol->gr_YVoltTransformCoef  = BaseCol->gr_YVoltTransformCoef;
    // -----
    int CountData = ((CCollection<short int> *)CheckListBox1->Items->Objects[0])->Count;
    if (  ButtonTag == 0 )
    {
        for ( int i = 0; i < CountData; i++ )NewCol->Add(Base[i] + Second[i]);
    }
    else
    {
        for ( int i = 0; i < CountData; i++ )NewCol->Add(Base[i] - Second[i]);
    }


    SetInterfaceParam();
    Application->ProcessMessages();
    rgVoltScalesClick(Sender);
    rgTimeScalesClick(Sender);
    // -----
    Chart->DrawSeriesValues();
    Chart->Canals = Chart->Canals;
    Chart->pbMain->Repaint();
    Application->ProcessMessages();
    // -----
    bAutoPositionClick(Sender);

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_main.h"
#include "Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma resource "*.dfm"
TForm1 *Form1;
double ANodeBuffer[64];
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    Chart1->LeftAxis->SetMinMax(-1.2, 1.2);
   UpDown1->Position = 2;
   UpDown2->Position = 0;
   UpDown2Click(NULL, 0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
    Series2->Clear();
    Series3->Clear();
    Series4->Clear();

    int zn = UpDown1->Position;
    for ( double t = -0.0; t <= 3.0; t = t + 0.01)
    {
      /*
      double t2 = t*t;
      double t3 = t*t*t;
      double k  = 1.0/5.618;
      double k1 = k*(1 - 3*t  + 3*t2-t3);
      double k2 = k*(4 - 6*t2 + 3*t3);
      double k3 = k*(1 + 3*t  + 3*t2-3*t3);
      double k4 = k*t3;
      double Dest = k1*ANodeBuffer[zn]+ k2*ANodeBuffer[zn+1] + k3*ANodeBuffer[zn+2]+ k4*ANodeBuffer[zn+3];
      */
/*
      double t2 = t*t;
      double t3 = t*t*t;
      double k  = 1.0/5.618;
      double k1 = k*(1 - 3*t  + 3*t2 -   t3);
      double k2 = k*(4 + 0*t  - 6*t2 + 3*t3);
      double k3 = k*(1 + 3*t  + 3*t2 - 3*t3);
      double k4 = k*(0 + 0*t  + 0*t2 +   t3);
      double Dest = k1*ANodeBuffer[zn]+ k2*ANodeBuffer[zn+1] + k3*ANodeBuffer[zn+2]+ k4*ANodeBuffer[zn+3];
*/
/*
    // ---- ������� � ��������� -----
      double t2 = t*t;
      double t3 = t*t*t;
      double k  = 1.0/5.618;

      double a0 =    ANodeBuffer[zn] + 4*ANodeBuffer[zn+1] + 1*ANodeBuffer[zn+2];
      double a1 = -3*ANodeBuffer[zn]                       + 3*ANodeBuffer[zn+2];
      double a2 =  3*ANodeBuffer[zn] - 6*ANodeBuffer[zn+1] + 3*ANodeBuffer[zn+2];
      double a3 = -  ANodeBuffer[zn] + 3*ANodeBuffer[zn+1] - 3*ANodeBuffer[zn+2] + ANodeBuffer[zn+3];

      double Dest = k*(a0 + a1*t + a2*t2 + a3*t3);
*/

    // ----- ������������ �������� -----
      double a0 =  ANodeBuffer[zn];
      double a1 =  0.16666666 * ( - 11.0*ANodeBuffer[zn] + 18.0*ANodeBuffer[zn+1] - 9.0*ANodeBuffer[zn+2] + 2.0*ANodeBuffer[zn+3] );
      double a2 = -0.5        * ( -  2.0*ANodeBuffer[zn] +  5.0*ANodeBuffer[zn+1] - 4.0*ANodeBuffer[zn+2] +     ANodeBuffer[zn+3] );
      double a3 =  0.16666666 * ( -      ANodeBuffer[zn] +  3.0*ANodeBuffer[zn+1] - 3.0*ANodeBuffer[zn+2] +     ANodeBuffer[zn+3] );
      double Dest1   =  a0 + a1*t + a2*t*t + a3*t*t*t;


    // ----- ������������ �������� (���������� ��� �������������)-----
    double k0 =  1.0 - 1.833333333*t + t*t - 0.1666666667*t*t*t;
    double k1 =  3.0*t - 2.5*t*t + 0.5*t*t*t;
    double k2 = -1.5*t + 2.0*t*t - 0.5*t*t*t;
    double k3 = 0.3333333333*t - 0.5*t*t + 0.1666666667*t*t*t;
    double Dest2 = k0*ANodeBuffer[zn] + k1*ANodeBuffer[zn+1] + k2*ANodeBuffer[zn+2] + k3*ANodeBuffer[zn+3];

    // ----- ����������� (������������ ��� ������) a3*t*t + a2*t + a1 = 0----
      double Dest_p = a1 + 2*a2*t   + 3*a3*t*t;
      a2 = a2 * 2.0;
      a3 = a3 * 3.0;
      double D  = a2*a2 - 4*a3*a1;
      if ( D >= 0 )
      {
          double x1 = (-a2 + sqrt(D)) / (2.0*a3);
          double x2 = (-a2 - sqrt(D)) / (2.0*a3);
          Edit3->Text = x1;
          Edit4->Text = x2;
      }


      Series2->AddXY(zn+t, Dest1);
      Series3->AddXY(zn+t, Dest2);
      Series4->AddXY(zn+t, Dest_p);

    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpDown2Click(TObject *Sender, TUDBtnType Button)
{
    setmem(ANodeBuffer, sizeof(ANodeBuffer), 0);
    Series1->Clear();

    double PushValue, t0 = 1e-5;
    double SinusFreq = 4323.16;
    int    ReliazationLength = 45;
    for ( double z = 0; z < t0*ReliazationLength; z = z + t0 )
    {
         PushValue  = sin(2.0 * M_PI * SinusFreq * z + UpDown2->Position*M_PI / 180.0);
         PushValue += 0.1 * sin(2.0 * M_PI * SinusFreq*2.5 * z + UpDown2->Position*M_PI / 180.0);

         for ( int i = 63; i > 0; i-- ) ANodeBuffer[i] = ANodeBuffer[i-1];
         ANodeBuffer[0] = PushValue;
    }
    Series1->AddArray(ANodeBuffer, ReliazationLength);
    // ------------------------
    UpDown1Click(NULL, btNext);
}
//---------------------------------------------------------------------------


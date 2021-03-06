//===========================================================================
//===========================================================================
//== ����������� ��� ��������������� ���� ������ ���� ����������� �������� ==
//===========================================================================
//===========================================================================
#include <vcl.h>
#include "Math.h"
#pragma hdrstop

#include "M_ImportData.h"
#include "Unit_BaseDialog_ImportData.h"
//===========================================================================
void TImportData_Init() { RegisterElement("DiALab - �������", "�����", (int)&TImportData::Create); }
#pragma startup TImportData_Init 110 // default 100
//===========================================================================
TImportData::TImportData()
{
  // --------- ��������� ������ ���� �� ���� ---------
     ResourseBitmapFaceElement->Handle = LoadBitmap(HInstance, typeid(*this).name());

  // ---------
     DefaultName = "���� ������.";
     Note        = "������ �� �����";
     CreateDate  = "12.11.2005";

  // ---------
     DesignPhase   = dpAllreadyTesting;
     ShowInOutHint = true;

  // ---------
     VisualElementParam->Width  = 2 * 24;
     VisualElementParam->Height = 2 * 16;
  // ---------

     iData.Init();
  // ---------

}

//===========================================================================
TImportData::~TImportData()
{
   iData.ReleaseArrays();
}

//===========================================================================
void TImportData::LoadFromStream(TStream *aStream) { TElement::LoadFromStream(aStream);
    iData.Init();

    aStream->Read(&iData.LengthFileName, sizeof(iData.LengthFileName));
    iData.FileName = (char*) malloc (iData.LengthFileName);

    aStream->Read( iData.FileName      , iData.LengthFileName);

    aStream->Read(&iData.CoundDataInOneCurve   , sizeof(iData.CoundDataInOneCurve   ));
    aStream->Read(&iData.T0, sizeof(iData.T0));
    aStream->Read(&iData.SeriesCount   , sizeof(iData.SeriesCount));

    iData.Series = (TmySeries*) malloc (sizeof(TmySeries)*iData.SeriesCount);

    for (int i = 0; i < iData.SeriesCount; i++){
        iData.Series[i].Value = (long  double*) malloc (sizeof(long double) * iData.CoundDataInOneCurve);

        aStream->Read(&iData.Series[i].Name   , sizeof(iData.Series[i].Name));
        aStream->Read( iData.Series[i].Value   , sizeof(long double) * iData.CoundDataInOneCurve);
    }

    iData.FileLoaded = true;
}

//===========================================================================
void TImportData::SaveToStream(TStream *aStream) { TElement::SaveToStream(aStream);
    aStream->Write(&iData.LengthFileName, sizeof(iData.LengthFileName));
    aStream->Write( iData.FileName      , iData.LengthFileName);

    aStream->Write(&iData.CoundDataInOneCurve   , sizeof(iData.CoundDataInOneCurve   ));
    aStream->Write(&iData.T0, sizeof(iData.T0));
    aStream->Write(&iData.SeriesCount   , sizeof(iData.SeriesCount));

    for (int i = 0; i < iData.SeriesCount; i++){
        aStream->Write(&iData.Series[i].Name   , sizeof(iData.Series[i].Name));
        aStream->Write( iData.Series[i].Value   , sizeof(long double) * iData.CoundDataInOneCurve);
    }
}

//===========================================================================
void TImportData::DrawElementFace()
{
      TCanvas    *dbCanvas = VisualElementParam->Bitmap->Canvas;
      int         aw       = VisualElementParam->Width ;
      int         ah       = VisualElementParam->Height;
      AnsiString  as       = "Data";
   // ----------
      dbCanvas->Brush->Color = clWhite;
      dbCanvas->Pen  ->Color = clBlack;

      dbCanvas->Rectangle(0, 0, aw, ah);

   // --------- ��������� �������������� --- (������������� �������� ����������) -----
     if (DesignPhase != dpAllreadyTesting) {
         if (DesignPhase == dpNotTesting  ) {dbCanvas->Pen  ->Color = clRed;}
         if (DesignPhase == dpNeedModified) {dbCanvas->Pen  ->Color = clRed; dbCanvas->Pen  ->Style = psDot;}

         dbCanvas->Rectangle(2, 2, VisualElementParam->Width - 2, VisualElementParam->Height - 2);

         dbCanvas->Pen->Color = clBlack;
         dbCanvas->Pen->Style = psSolid;
      }

   // ----------  --------
      dbCanvas->TextOut(6, (ah - dbCanvas->TextHeight(as)) / 2, as);

   // ----------
      dbCanvas->Pen->Color = clBlack;
}

//===========================================================================
void TImportData::Edit()
{
   TfrmBaseDialog_ImportData *dlg;

   try {
        dlg = new TfrmBaseDialog_ImportData(NULL);
        dlg->Element            = this;
        dlg->GroupBox1->Caption = DefaultName;
        dlg->lTitle   ->Caption = Note;

        dlg->ImportData = this;
     // ------
        /*dlg->ValueArray[0]          = &A;
        dlg->ValueArray[1]          = &Alfa;
        dlg->ValueArray[2]          = &F;
        dlg->ValueArray[3]          = &aFi;
        dlg->LabelArray[0]->Caption = "��������� ��������� [A]";
        dlg->LabelArray[1]->Caption = "��������� ��������� [Alfa]";
        dlg->LabelArray[2]->Caption = "������� [F, ����]";
        dlg->LabelArray[3]->Caption = "���� [Fi, ������]";*/
        //dlg->ParameteresCount       = 0;
        //dlg->BaseDialogFormStyle    = bdfsFloatValues;
     // ------
        dlg->ShowModal();

     // ------
   } __finally { delete dlg; }
}

//===========================================================================
void TImportData::Copy(void *aElement) { TElement::Copy(aElement);

//COPY_COPY
}

//===========================================================================
void TImportData::Init() { TElement::Init();
    ArrayIndex = 0;
    FixedStep = false;

    if (iData.T0 == 0 || fabs(iData.T0 - pProjectParam->AnalogT0) < 1e-12){
            ArrayIndexStep = 1;  FixedStep = true;
    } else
            ArrayIndexStep = pProjectParam->AnalogT0 / iData.T0;;
}

//===========================================================================
void TImportData::InitBeforeRun()
{
/*   for (int i = Order - 1; i >= 0; i--){
       *pCurModelTime = (- 1 - i) * pProjectParam->AnalogT0;
       Run();
   }*/
}

//===========================================================================
void TImportData::Run()
{     if (!isEnabledCalc()) return; TElement::Run();
   // --------------


     ///---- ���������� ������� ������� � ������ -
       for (int i = 0 ; i < iData.SeriesCount; i++)
          if (ArrayIndex < iData.CoundDataInOneCurve) {
              if (!FixedStep) {
                    int UpIndex   = ceil (ArrayIndex);
                    int DownIndex = floor(ArrayIndex);
                    oPoints->Items[i]->Queue[0] = iData.Series[i].Value[DownIndex] +
                                                 (iData.Series[i].Value[UpIndex] - iData.Series[i].Value[DownIndex]) * (ArrayIndex - DownIndex);;
              } else oPoints->Items[i]->Queue[0] = iData.Series[i].Value[(int)ArrayIndex];
          } else oPoints->Items[i]->Queue[0] = 0;


      ArrayIndex    = ArrayIndex + ArrayIndexStep;
}


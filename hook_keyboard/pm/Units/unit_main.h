//---------------------------------------------------------------------------

#ifndef unit_mainH
#define unit_mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class Tform_Main : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *StringGrid1;
    TMemo *Memo1;
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
    void MainTable_draw();
public:		// User declarations
	__fastcall Tform_Main(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tform_Main *form_Main;
//---------------------------------------------------------------------------
#endif